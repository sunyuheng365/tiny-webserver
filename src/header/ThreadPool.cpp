//
// Created by sunyuheng365 on 25-4-4.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int num)
    : thread_num_(static_cast<int>(num)), stop_(false) {
  start();
}

ThreadPool::~ThreadPool() {
  stop_.store(true);
  cv_.notify_all();
  for (auto &thread : pool_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

auto ThreadPool::start() -> void {
  for (int i = 0; i < thread_num_; i++) {
    pool_.emplace_back([this]() {
      while (!this->stop_.load()) {
        Task task;
        {
          std::unique_lock cv_lock(cv_mtx_);
          this->cv_.wait(cv_lock, [this] {
            return this->stop_.load() || !this->tasks_.empty();
          });
          if (this->stop_.load()) {
            return;
          }
          task = std::move(this->tasks_.front());
          this->tasks_.pop();
        }
        task();
      }
    });
  }
}
