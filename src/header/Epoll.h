//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef EPOLL_H
#define EPOLL_H

#include <memory>
#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll {
public:
  Epoll();
  ~Epoll();

  auto UpdateChannel(Channel *channel) -> void;
  auto RemoveChannel(Channel *channel) -> void;
  auto Start(int timeout = -1) -> std::vector<Channel *>;

private:
  int epoll_fd_;
  std::unique_ptr<epoll_event[]> events_;
};

#endif // EPOLL_H
