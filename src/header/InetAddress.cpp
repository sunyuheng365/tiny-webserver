//
// Created by sunyuheng365 on 25-4-4.
//

#include "InetAddress.h"

#include <iostream>

#include <arpa/inet.h>
#include <ostream>

InetAddress::InetAddress(const std::string &ip, uint16_t port) {
  address_.sin_family = AF_INET;
  address_.sin_port = htons(port);
  if (inet_pton(AF_INET, ip.c_str(), &address_.sin_addr) < 0) {
    std::cerr << "Address conversion failed." << std::endl;
  }
}

InetAddress::~InetAddress() {}

auto InetAddress::GetIp() const -> std::string {
  char buf[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &address_.sin_addr, buf, INET_ADDRSTRLEN);
  return buf;
}

auto InetAddress::GetPort() const -> uint16_t {
  return ntohs(address_.sin_port);
}

auto InetAddress::GetSockaddr() -> sockaddr * {
  return reinterpret_cast<sockaddr *>(&address_);
}

auto InetAddress::GetSockaddr() const -> const sockaddr * {
  return reinterpret_cast<const sockaddr *>(&address_);
}
