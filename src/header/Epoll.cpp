//
// Created by sunyuheng365 on 25-4-4.
//

#include "Epoll.h"
#include "Channel.h"
#include "const.h"
#include <iostream>
#include <regex>
#include <unistd.h>

Epoll::Epoll()
    : epoll_fd_(epoll_create1(EPOLL_CLOEXEC)),
      events_(std::make_unique<epoll_event[]>(EPOLL_MAX_EVENTS)) {
  if (epoll_fd_ < 0) {
    std::cerr << "epoll_create1() failed" << std::endl;
    throw std::runtime_error("epoll_create1() failed");
  }
}

Epoll::~Epoll() {
  if (epoll_fd_ >= 0) {
    ::close(epoll_fd_);
    epoll_fd_ = -1;
  }
}

auto Epoll::UpdateChannel(Channel *channel) -> void {
  int fd = channel->GetFd();
  epoll_event event{channel->GetEvents(), channel};
  if (channel->IsInEpoll()) {
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event) < 0) {
      std::cerr << "epoll_ctl() failed" << std::endl;
      throw std::runtime_error("epoll_ctl() failed");
    }
  } else {
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
      std::cerr << "epoll_ctl() failed" << std::endl;
      throw std::runtime_error("epoll_ctl() failed");
    }
    channel->EnableInEpoll();
  }
}

auto Epoll::RemoveChannel(Channel *channel) -> void {
  int fd = channel->GetFd();
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) < 0) {
    std::cerr << "epoll_ctl() failed" << std::endl;
    throw std::runtime_error("epoll_ctl() failed");
  }
}

auto Epoll::Start(int timeout) -> std::vector<Channel *> {
  while (true) {
    int n = epoll_wait(epoll_fd_, events_.get(), EPOLL_MAX_EVENTS, timeout);
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      }
      std::cerr << "epoll_wait() failed" << std::endl;
      std::cerr << errno << std::endl;
      throw std::runtime_error("epoll_wait() failed");
    }
    if (n == 0) {
      return {};
    }
    std::vector<Channel *> active_channels;
    active_channels.reserve(n);
    for (int i = 0; i < n; ++i) {
      Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
      channel->SetReadyEvents(events_[i].events);
      active_channels.emplace_back(channel);
    }
    return active_channels;
  }
}
