//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef ACCEPTER_H
#define ACCEPTER_H

#include "const.h"

#include <functional>
#include <memory>

class Channel;
class InetAddress;
class Socket;
class EventLoop;

class Accepter {
public:
  explicit Accepter(EventLoop *loop, const InetAddress &addr,
                    int listen_n = LISTEN_MAX);
  ~Accepter();

  auto SetNewConnectionCallback(
      std::function<void(std::unique_ptr<Socket>)> callback) -> void;
  auto AcceptConnection() -> void;

private:
  EventLoop *loop_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<InetAddress> address_;
  std::unique_ptr<Channel> channel_;

  std::function<void(std::unique_ptr<Socket>)> new_connection_callback_;
};

#endif // ACCEPTER_H
