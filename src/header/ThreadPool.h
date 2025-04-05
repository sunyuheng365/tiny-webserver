//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
  // static ThreadPool &GetThreadPool();

  explicit ThreadPool(unsigned int num = std::thread::hardware_concurrency());
  ~ThreadPool();

  template <class F, class... Args>
  auto commit(F &&f, Args &&...args) -> std::future<
      decltype(std::forward<F>(f)(std::forward<Args>(args)...))> {
    using ReturnType =
        decltype(std::forward<F>(f)(std::forward<Args>(args)...));

    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<ReturnType> ret = task->get_future();
    {
      std::unique_lock cv_lock(cv_mtx_);
      tasks_.emplace([task]() { (*task)(); });
      cv_.notify_one();
    }
    return ret;
  }

private:
  using Task = std::function<void()>;

  auto start() -> void;

  const int thread_num_;
  std::atomic<bool> stop_;
  std::queue<Task> tasks_;
  std::vector<std::thread> pool_;
  std::mutex cv_mtx_;
  std::condition_variable cv_;
};

#endif // THREADPOOL_H
