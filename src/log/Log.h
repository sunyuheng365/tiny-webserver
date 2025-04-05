//
// Created by sunyuheng365 on 25-4-5.
//

#ifndef LOG_H
#define LOG_H

#define LOG_DEBUG(...) Log::GetLogger().LogMessage(LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) Log::GetLogger().LogMessage(LogLevel::INFO, __VA_ARGS__)
#define LOG_WARN(...)                                                          \
  Log::GetLogger().LogMessage(LogLevel::WARNING, __VA_ARGS__)
#define LOG_ERROR(...) Log::GetLogger().LogMessage(LogLevel::ERROR, __VA_ARGS__)

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

template <typename T> std::string to_string_helper(T &&arg) {
  std::ostringstream os;
  os << std::forward<T>(arg);
  return os.str();
}

class LogQueue {
public:
  auto push(const std::string &msg) -> void {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(msg);
    if (queue_.size() == 1) {
      cv_.notify_one();
    }
  }
  auto pop(std::string &msg) -> bool {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&] { return !queue_.empty() || is_shutdown_; });
    if (is_shutdown_ && queue_.empty()) {
      return false;
    }
    msg = std::move(queue_.front());
    queue_.pop();
    return true;
  }
  auto shutdown() -> void {
    std::lock_guard<std::mutex> lock(mutex_);
    is_shutdown_ = true;
    cv_.notify_all();
  }

private:
  std::queue<std::string> queue_;
  std::mutex mutex_;
  std::condition_variable cv_;
  bool is_shutdown_{false};
};

enum class LogLevel {
  DEBUG = 0,
  INFO = 1,
  WARNING = 2,
  ERROR = 3,
  DISABLE_LOG = 4
};

class Log {
public:
  ~Log() {
    is_exit_ = true;
    queue_.shutdown();
    if (worker_thread_.joinable()) {
      worker_thread_.join();
    }
    if (log_file_.is_open()) {
      log_file_.close();
    }
  }

  static Log &GetLogger(const std::string &file_name = "log.txt") {
    static Log logger(file_name);
    return logger;
  };

  template <typename... Args>
  auto LogMessage(LogLevel level, const std::string &format, Args... args)
      -> void {
    if (level < min_log_level_) {
      return;
    }
    std::string level_str;
    switch (level) {
    case LogLevel::INFO:
      level_str = "[INFO]    ";
      break;
    case LogLevel::WARNING:
      level_str = "[WARNING] ";
      break;
    case LogLevel::ERROR:
      level_str = "[ERROR]   ";
      break;
    case LogLevel::DEBUG:
      level_str = "[DEBUG]   ";
      break;
    default:
      level_str = "[UNKNOWN] ";
      break;
    }

    queue_.push(level_str + GetCurrentTime() + " " +
                FormatString(format, std::forward<Args>(args)...));
  }

  // 只有 >= level 的日志会被允许输出
  // level 等级:  DEBUG < INFO < WARNING < ERROR < DISABLE_LOG
  // 其中 DISABLE_LOG 仅用于禁用日志输出, 无其他用途
  auto SetMinLogLevel(LogLevel level) -> void { min_log_level_ = level; }

  auto SetOutputFile(const std::string &file_name) -> void {
    log_file_.close();
    log_file_.open(file_name, std::ios::out | std::ios::app);
    if (!log_file_.is_open()) {
      std::cerr << "Log File Open Failed: " << file_name << std::endl;
      throw std::runtime_error("Failed to open log file.");
    }
  }

private:
  std::string GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S",
                  std::localtime(&time));
    return buffer;
  }

  explicit Log(const std::string &file_name)
      : log_file_(file_name, std::ios::out | std::ios::app), is_exit_(false),
        min_log_level_(LogLevel::DEBUG) {
    if (!log_file_.is_open()) {
      // 日志系统必须成功启动
      // 核心类禁止兼容, 应以强硬态度退出
      std::cerr << "Log File Open Failed: " << file_name << std::endl;
      throw std::runtime_error("Failed to open log file.");
    }
    worker_thread_ = std::thread([this]() {
      std::string msg;
      while (queue_.pop(msg)) {
        log_file_ << msg << std::endl;
      }
    });
  }

  template <typename... Args>
  auto FormatString(const std::string &format, Args... args) -> std::string {
    std::vector<std::string> arg_strings = {
        to_string_helper(std::forward<Args>(args))...};
    std::ostringstream os;
    size_t pos = 0;
    size_t arg_index = 0;
    size_t place_holder = 0;
    while ((place_holder = format.find("{}", pos)) != std::string::npos) {
      os << format.substr(pos, place_holder - pos);
      if (arg_index < static_cast<int>(arg_strings.size())) {
        os << arg_strings[arg_index++];
      } else {
        os << "{}";
      }
      pos = place_holder + 2;
    }
    os << format.substr(pos);
    while (arg_index < static_cast<int>(arg_strings.size())) {
      os << arg_strings[arg_index++];
    }
    return os.str();
  }

  LogQueue queue_;
  std::thread worker_thread_;
  std::ofstream log_file_;
  std::atomic<bool> is_exit_;
  LogLevel min_log_level_;
};

#endif // LOG_H
