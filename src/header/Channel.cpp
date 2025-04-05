//
// Created by sunyuheng365 on 25-4-4.
//

#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>
#include <unistd.h>

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), ready_events_(0), in_epoll_(false) {}

auto Channel::SetReadCallback(std::function<void()> callback) -> void {
  read_callback_ = std::move(callback);
}

auto Channel::SetWriteCallback(std::function<void()> callback) -> void {
  write_callback_ = std::move(callback);
}

auto Channel::SetErrorCallback(std::function<void()> callback) -> void {
  error_callback_ = std::move(callback);
}

auto Channel::SetCloseCallback(std::function<void()> callback) -> void {
  close_callback_ = std::move(callback);
}

auto Channel::GetFd() const -> int { return fd_; }

auto Channel::GetEvents() const -> uint32_t { return events_; }

auto Channel::GetReadyEvents() const -> uint32_t { return ready_events_; }

auto Channel::SetReadyEvents(uint32_t ready_events) -> void {
  ready_events_ = ready_events;
}

auto Channel::IsInEpoll() const -> bool { return in_epoll_; }

auto Channel::EnableInEpoll() -> void { in_epoll_ = true; }

auto Channel::EnableRead() -> void {
  events_ |= EPOLLIN | EPOLLPRI;
  loop_->UpdateChannel(this);
}

auto Channel::DisableRead() -> void {
  events_ &= ~EPOLLIN & ~EPOLLPRI;
  loop_->UpdateChannel(this);
}

auto Channel::EnableWrite() -> void {
  events_ |= EPOLLOUT;
  loop_->UpdateChannel(this);
}

auto Channel::DisableWrite() -> void {
  events_ &= ~EPOLLOUT;
  loop_->UpdateChannel(this);
}

auto Channel::DisableAll() -> void {
  events_ = 0;
  loop_->UpdateChannel(this);
}

auto Channel::EnableET() -> void {
  events_ |= EPOLLET;
  loop_->UpdateChannel(this);
}

auto Channel::HandleEvent() -> void {
  // 对端关闭链接且无需要读入数据
  if ((ready_events_ & EPOLLHUP) && !(ready_events_ & EPOLLIN)) {
    if (close_callback_ != nullptr) {
      close_callback_();
      return;
    }
  }
  // 错误
  if (ready_events_ & EPOLLERR) {
    if (error_callback_ != nullptr) {
      error_callback_();
      return;
    }
  }
  // 有需要读入
  if (ready_events_ & (EPOLLIN | EPOLLPRI)) {
    if (read_callback_ != nullptr) {
      read_callback_();
    }
  }
  // 有需要输出
  if (ready_events_ & EPOLLOUT) {
    if (write_callback_ != nullptr) {
      write_callback_();
    }
  }
}
