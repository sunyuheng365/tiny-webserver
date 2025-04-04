//
// Created by sunyuheng365 on 25-4-4.
//

#include "EventLoop.h"

#include "Channel.h"

EventLoop::EventLoop() : epoll_(std::make_unique<Epoll>()) {}

EventLoop::~EventLoop() {}

auto EventLoop::Loop() -> void {
  while (true) {
    for (Channel *active_channel : epoll_->Start()) {
      active_channel->HandleEvent();
    }
  }
}

auto EventLoop::UpdateChannel(Channel *channel) -> void {
  epoll_->UpdateChannel(channel);
}
auto EventLoop::RemoveChannel(Channel *channel) -> void {
  epoll_->RemoveChannel(channel);
}
