//
// Created by sunyuheng365 on 25-4-4.
//

#include "Accepter.h"

#include "../log/Log.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"

Accepter::Accepter(EventLoop *loop, const InetAddress &addr, int listen_n)
    : loop_(loop), socket_(std::make_unique<Socket>()),
      address_(std::make_unique<InetAddress>(addr)) {
  socket_->SetIpAndPort(addr);
  socket_->Bind(addr);
  socket_->Listen(listen_n);
  socket_->SetNonBlocking();
  socket_->SetReuseAddr();
  channel_ = std::make_unique<Channel>(loop_, socket_->GetFd());
  channel_->EnableRead();
  channel_->EnableET();
  channel_->SetReadCallback([this]() { AcceptConnection(); });
  loop_->UpdateChannel(channel_.get());
}

auto Accepter::SetNewConnectionCallback(
    std::function<void(std::unique_ptr<Socket>)> callback) -> void {
  new_connection_callback_ = std::move(callback);
}

auto Accepter::AcceptConnection() -> void {
  while (true) {
    InetAddress new_address{};
    socklen_t len = sizeof(new_address);
    int client_fd = ::accept4(socket_->GetFd(), new_address.GetSockaddr(), &len,
                              SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (client_fd < 0) {
      if (errno == EINTR) {
        continue;
      }
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
      LOG_ERROR("Accepter::AcceptConnection() failed, errno is : ", errno);
      break;
    }
    if (new_connection_callback_ != nullptr) {
      std::unique_ptr<Socket> socket = std::make_unique<Socket>(client_fd);
      socket->SetIpAndPort(new_address);
      new_connection_callback_(std::move(socket));
      static int x = 0;
      LOG_INFO("New connection :{}.", ++x);
    }
  }
}
