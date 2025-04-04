//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef SOCKET_H
#define SOCKET_H

#include "InetAddress.h"
#include "Socket.h"
#include "const.h"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/tcp.h>
#include <string>
#include <unistd.h>

class Socket {
public:
  Socket();
  explicit Socket(int socket_fd);
  ~Socket();

  auto Listen(int n = LISTEN_MAX) -> bool;
  auto Connect(const InetAddress &address) -> bool;
  auto Bind(const InetAddress &address) -> bool;
  auto Close() -> bool;
  auto Accept(InetAddress &address) -> int;
  auto SetIpAndPort(const InetAddress &address) -> void;
  auto GetFd() const -> int;
  auto GetIp() const -> std::string;
  auto GetPort() const -> uint16_t;

  auto SetNonBlocking() -> bool; // 设置非阻塞
  auto SetReuseAddr() -> bool;   // 设置可复用 TimeWait
  auto SetReusePort() -> bool;   // 设置可复用端口
  auto SetKeepAlive() -> bool;   // 设置 TCP 保活
  auto SetTcpNoDelay() -> bool;  // 设置禁用 Nagle

private:
  int fd_;
  std::string ip_;
  uint16_t port_;
};

#endif // SOCKET_H
