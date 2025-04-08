//
// Created by sunyuheng365 on 25-4-4.
//

#include "ReactorSubServer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"

ReactorSubServer::ReactorSubServer(ThreadPool *thread_pool)
    : loop_(std::make_unique<EventLoop>()), thread_pool_(thread_pool) {}

auto ReactorSubServer::SetMessageCallback(
    std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
        callback) -> void {
  message_callback_ = std::move(callback);
}

auto ReactorSubServer::SetCloseCallback(
    std::function<void(std::shared_ptr<Connection>)> callback) -> void {
  close_callback_ = std::move(callback);
}

auto ReactorSubServer::SetAcceptCallback(
    std::function<void(std::shared_ptr<Connection>)> callback) -> void {
  accept_callback_ = std::move(callback);
}

auto ReactorSubServer::AcceptConnection(std::unique_ptr<Socket> socket)
    -> void {
  auto connection =
      std::make_shared<Connection>(loop_.get(), std::move(socket));
  connection->SetMessageCallback(message_callback_);
  connection->SetCloseCallback(close_callback_);
  accept_callback_(connection);
}

auto ReactorSubServer::Start() -> void { loop_->Loop(thread_pool_); }
