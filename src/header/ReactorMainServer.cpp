//
// Created by sunyuheng365 on 25-4-4.
//

#include "ReactorMainServer.h"

#include "Accepter.h"
#include "Connection.h"
#include "EventLoop.h"
#include "ReactorSubServer.h"
#include "Socket.h"
#include "ThreadPool.h"

ReactorMainServer::ReactorMainServer(const InetAddress &address,
                                     int sub_server_num)
    : loop_(std::make_unique<EventLoop>()),
      accepter_(std::make_unique<Accepter>(loop_.get(), address)),
      thread_pool_(std::make_unique<ThreadPool>()) {
  // Accepter:
  accepter_->SetNewConnectionCallback([this](std::unique_ptr<Socket> socket) {
    NewConnection(std::move(socket));
  });
  // SubServer:
  for (int i = 0; i < sub_server_num; ++i) {
    sub_servers_.emplace_back(
        std::make_unique<ReactorSubServer>(thread_pool_.get()));
    sub_servers_[i]->SetAcceptCallback(
        [this](std::shared_ptr<Connection> connection) {
          this->connections_map_.insert(
              std::make_pair(connection->GetFd(), connection));
        });
    sub_servers_[i]->SetCloseCallback(
        [this](std::shared_ptr<Connection> connection) {
          connection->RemvoeFromEpoll();
          this->connections_map_.erase(connection->GetFd());
        });
  }
}

auto ReactorMainServer::Start() -> void {
  for (auto &sub_server : sub_servers_) {
    thread_pool_->commit([&sub_server]() { sub_server->Start(); });
  }
  loop_->Loop();
}

auto ReactorMainServer::NewConnection(std::unique_ptr<Socket> socket) -> void {
  auto &sub_server = sub_servers_[Hash(socket->GetFd())];
  sub_server->AcceptConnection(std::move(socket));
}

auto ReactorMainServer::SetMessageCallback(
    std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
        callback) -> void {
  message_callback_ = std::move(callback);
  for (std::unique_ptr<ReactorSubServer> &sub_server : sub_servers_) {
    sub_server->SetMessageCallback(message_callback_);
  }
}

auto ReactorMainServer::Hash(int socket_fd) -> int {
  return socket_fd % sub_servers_.size();
}
