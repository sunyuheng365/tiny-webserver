//
// Created by sunyuheng365 on 25-4-4.
//

#include "Connection.h"
#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Connection::Connection(EventLoop *loop, std::unique_ptr<Socket> socket)
    : event_loop_(loop), socket_(std::move(socket)),
      address_(
          std::make_unique<InetAddress>(socket->GetIp(), socket->GetPort())),
      channel_(std::make_unique<Channel>(loop, socket->GetFd())),
      send_buffer_(std::make_unique<Buffer>()) {
  channel_->SetReadCallback([this]() {
    recv_buffer_ = std::make_unique<Buffer>();
    RecvMessage();
    message_callback_(shared_from_this(), std::move(recv_buffer_));
  });
}

Connection::~Connection() { event_loop_->RemoveChannel(channel_.get()); }

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
    ssize_t n = send(socket_->GetFd(), data, size, 0);
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      }
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
      close_callback_(shared_from_this());
      return;
    }
    data += n;
    size -= static_cast<int>(n);
  }
}

auto Connection::RecvMessage() -> void {
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
        break;
      }
      if (errno == EINTR) {
        continue;
      }
    }
    // len == 0 || len < 0 And errno Invaild
    close_callback_(shared_from_this());
    break;
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
