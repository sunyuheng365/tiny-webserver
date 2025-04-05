//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef REACTORSUBSERVER_H
#define REACTORSUBSERVER_H

#include "ThreadPool.h"

#include <map>
#include <memory>

class Buffer;
class Socket;
class EventLoop;
class Connection;

class ReactorSubServer {
public:
  ReactorSubServer(ThreadPool *thread_pool);
  ~ReactorSubServer() = default;

  auto SetMessageCallback(
      std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
          callback) -> void;
  auto
  SetCloseCallback(std::function<void(std::shared_ptr<Connection>)> callback)
      -> void;
  auto
  SetAcceptCallback(std::function<void(std::shared_ptr<Connection>)> callback)
      -> void;

  auto AcceptConnection(std::unique_ptr<Socket> socket) -> void;
  auto Start() -> void;

private:
  std::unique_ptr<EventLoop> loop_;
  ThreadPool *thread_pool_;

  std::function<void(std::shared_ptr<Connection>)> close_callback_;
  std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
      message_callback_;
  std::function<void(std::shared_ptr<Connection>)> accept_callback_;
};

#endif // REACTORSUBSERVER_H
