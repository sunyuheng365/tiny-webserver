//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef CHANNEL_H
#define CHANNEL_H
#include <cstdint>
#include <functional>

class EventLoop;

class Channel {
public:
  Channel(EventLoop *loop, int fd);
  ~Channel();

  // 回调函数
  auto SetReadCallback(std::function<void()> callback) -> void;
  auto SetWriteCallback(std::function<void()> callback) -> void;
  auto SetErrorCallback(std::function<void()> callback) -> void;
  auto SetCloseCallback(std::function<void()> callback) -> void;

  // 获取/修改成员
  auto GetFd() const -> int;
  auto GetEvents() const -> uint32_t;
  auto GetReadyEvents() const -> uint32_t;
  auto SetReadyEvents(uint32_t ready_events) -> void;
  auto IsInEpoll() const -> bool;
  auto EnableInEpoll()
      -> void; // 一个 Channel 只会加一次 Epoll, 所以关闭后就让它死亡

  // 修改属性
  auto EnableRead() -> void;
  auto DisableRead() -> void;
  auto EnableWrite() -> void;
  auto DisableWrite() -> void;
  auto DisableAll() -> void; // 关闭所有监听
  auto EnableET() -> void;   // 启用边缘触发(默认水平)

  // 核心函数: 根据 ready_events_ 处理 Event
  auto HandleEvent() -> void;

private:
  EventLoop *loop_;

  int fd_;
  uint32_t events_;
  uint32_t ready_events_;
  bool in_epoll_;

  std::function<void()> read_callback_;
  std::function<void()> write_callback_;
  std::function<void()> error_callback_;
  std::function<void()> close_callback_;
};

#endif // CHANNEL_H
