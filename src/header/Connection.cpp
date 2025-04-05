//
// Created by sunyuheng365 on 25-4-4.
//

#include "Connection.h"
#include "../log/Log.h"
#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Connection::Connection(EventLoop *loop, std::unique_ptr<Socket> socket)
    : event_loop_(loop), socket_(std::move(socket)),
      address_(
          std::make_unique<InetAddress>(socket_->GetIp(), socket_->GetPort())),
      channel_(std::make_unique<Channel>(event_loop_, socket_->GetFd())),
      send_buffer_(std::make_unique<Buffer>()) {

  channel_->EnableRead();
  channel_->SetReadCallback([this]() {
    recv_buffer_ = std::make_unique<Buffer>();
    if (RecvMessage()) {
      message_callback_(shared_from_this(), std::move(recv_buffer_));
    }
  });
  event_loop_->UpdateChannel(channel_.get());
}

auto Connection::RemvoeFromEpoll() -> void {
  event_loop_->RemoveChannel(channel_.get());
}

auto Connection::GetFd() const -> int { return socket_->GetFd(); }

auto Connection::GetInetAddress() const -> InetAddress * {
  return address_.get();
}

auto Connection::GetIp() -> std::string { return address_->GetIp(); }

auto Connection::GetPort() const -> uint16_t { return address_->GetPort(); }

auto Connection::SendMessage(const Buffer &buf) -> void {
  int size = buf.Size();
  const char *data = buf.Data();
  while (size > 0) {
    ssize_t n = ::send(socket_->GetFd(), data, size, 0);
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      }
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
      LOG_ERROR("Connection::SendMessage Failed , errno is :{}", errno);
      close_callback_(shared_from_this());
      return;
    }
    data += n;
    size -= static_cast<int>(n);
  }
}

auto Connection::RecvMessage() -> bool {
  char buf[TCP_SERVER_MAX_RECV];
  while (true) {
    int len =
        static_cast<int>(::recv(socket_->GetFd(), buf, TCP_SERVER_MAX_RECV, 0));
    // 正在读
    if (len > 0) {
      recv_buffer_->Append(buf, len);
      continue;
    }
    // 读完了
    if (len < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return true;
      }
      if (errno == EINTR) {
        continue;
      }
      LOG_ERROR("Connection::RecvMessage Failed , errno is :{}", errno);
      close_callback_(shared_from_this());
      return false;
    }
    // len == 0
    LOG_INFO("Client Close Socket.");
    close_callback_(shared_from_this());
    return false;
  }
}

auto Connection::SetMessageCallback(
    std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
        callback) -> void {
  message_callback_ = std::move(callback);
}

auto Connection::SetCloseCallback(
    std::function<void(std::shared_ptr<Connection>)> callback) -> void {
  close_callback_ = std::move(callback);
}
