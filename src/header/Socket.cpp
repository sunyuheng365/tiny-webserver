//
// Created by sunyuheng365 on 25-4-4.
//

#include "Socket.h"

#include <iostream>

#include <ostream>
#include <stdexcept>
#include <unistd.h>

Socket::Socket() {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ < 0) {
    std::cerr << "Socket creation failed." << std::endl;
  }
}
Socket::Socket(int socket_fd) {
  fd_ = socket_fd;
  if (fd_ < 0) {
    std::cerr << "Socket creation failed." << std::endl;
  }
}

Socket::~Socket() { Close(); }

auto Socket::Listen(int n) -> bool {
  if (::listen(fd_, n) < 0) {
    std::cerr << "Failed to listen on socket" << std::endl;
    return false;
  }
  return true;
}

auto Socket::Connect(const InetAddress &address) -> bool {
  if (::connect(fd_, address.GetSockaddr(), sizeof(sockaddr)) < 0) {
    std::cerr << "Failed to connect on socket" << std::endl;
    return false;
  }
  return true;
}

auto Socket::Bind(const InetAddress &address) -> bool {
  if (::bind(fd_, address.GetSockaddr(), sizeof(sockaddr)) < 0) {
    std::cerr << "Failed to bind on socket" << std::endl;
    return false;
  }
  return true;
}

auto Socket::Close() -> bool {
  if (fd_ >= 0) {
    ::close(fd_);
    fd_ = -1;
    return true;
  }
  return false;
}

auto Socket::Accept(InetAddress &address) -> int {
  socklen_t addrlen = sizeof(sockaddr);
  int connfd = ::accept(fd_, address.GetSockaddr(), &addrlen);
  if (connfd < 0) {
    std::cerr << "Failed to accept on socket" << std::endl;
    return -1;
  }
  return connfd;
}

auto Socket::SetIpAndPort(const InetAddress &address) -> void {
  ip_ = address.GetIp();
  port_ = address.GetPort();
}

auto Socket::GetFd() const -> int { return fd_; }

auto Socket::GetIp() const -> std::string { return ip_; }

auto Socket::GetPort() const -> uint16_t { return port_; }

auto Socket::SetNonBlocking() -> bool {
  int flags = fcntl(fd_, F_GETFL, 0);
  if (flags < 0) {
    std::cerr << "Failed to get socket flags" << std::endl;
    return false;
  }

  if (fcntl(fd_, F_SETFL, flags | O_NONBLOCK) < 0) {
    std::cerr << "Failed to set socket flags" << std::endl;
    return false;
  }

  return true;
}

auto Socket::SetReuseAddr() -> bool {
  int optval = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    std::cerr << "Failed to set socket options" << std::endl;
    return false;
  }
  return true;
}

auto Socket::SetReusePort() -> bool {
  int optval = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0) {
    std::cerr << "Failed to set socket options" << std::endl;
    return false;
  }
  return true;
}

auto Socket::SetKeepAlive() -> bool {
  int optval = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
    std::cerr << "setsockopt(SO_KEEPALIVE)" << std::endl;
    return false;
  }
  return true;
}

auto Socket::SetTcpNoDelay() -> bool {
  int optval = 1;
  if (setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)) < 0) {
    std::cerr << "setsockopt(TCP_NODELAY)" << std::endl;
    return false;
  }
  return true;
}
