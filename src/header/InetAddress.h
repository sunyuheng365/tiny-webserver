//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef INETADDRESS_H
#define INETADDRESS_H
#include <netinet/in.h>
#include <string>

class InetAddress {
public:
  InetAddress() : address_() {};
  InetAddress(const std::string &ip, uint16_t port);
  InetAddress(const InetAddress &address) = default;
  ~InetAddress();

  auto GetIp() const -> std::string;
  auto GetPort() const -> uint16_t;
  auto GetSockaddr() -> sockaddr *;
  auto GetSockaddr() const -> const sockaddr *;

private:
  sockaddr_in address_;
};

#endif // INETADDRESS_H
