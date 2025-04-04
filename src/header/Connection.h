//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef CONNECTION_H
#define CONNECTION_H

#include <functional>
#include <memory>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Buffer;

class Connection : public std::enable_shared_from_this<Connection> {
public:
  Connection(EventLoop *loop, std::unique_ptr<Socket> socket);
  ~Connection();

  auto GetFd() const -> int;
  auto GetInetAddress() const -> InetAddress *;
  auto GetIp() -> std::string;
  auto GetPort() const -> uint16_t;

  // 缓存到 write_buffer 然后发送
  auto SendMessage(const Buffer &buf) -> void;
  // 接收到 read_buffer 然后调用回调函数
  auto RecvMessage() -> void;

  auto SetMessageCallback(
      std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
          callback) -> void;
  auto SetCloseCallback(std::function<void(std::shared_ptr<Connection>)>)
      -> void;

private:
  EventLoop *event_loop_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<InetAddress> address_;
  std::unique_ptr<Channel> channel_;
  std::unique_ptr<Buffer> recv_buffer_;
  std::unique_ptr<Buffer> send_buffer_;

  std::function<void(std::shared_ptr<Connection>)> close_callback_;
  std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
      message_callback_;
  // std::function<void(std::shared_ptr<Connection>)> write_callback;
};

#endif // CONNECTION_H
