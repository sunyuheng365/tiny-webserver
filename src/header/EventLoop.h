//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include "Epoll.h"
#include "ThreadPool.h"

class EventLoop {
public:
  EventLoop();
  ~EventLoop();

  auto Loop() -> void;
  auto Loop(ThreadPool *thread_pool) -> void;
  auto UpdateChannel(Channel *channel) -> void;
  auto RemoveChannel(Channel *channel) -> void;

private:
  std::unique_ptr<Epoll> epoll_;
};

#endif // EVENTLOOP_H
