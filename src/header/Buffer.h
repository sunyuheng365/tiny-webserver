//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef BUFFER_H
#define BUFFER_H
#include <string>

class Buffer {
public:
  Buffer() = default;
  ~Buffer() = default;

  template <typename T>
    requires std::is_constructible_v<std::string, T>
  explicit Buffer(T &&val) : buffer_(std::forward<T>(val)) {}

  template <typename T>
    requires std::is_assignable_v<std::string &, T>
  auto SetData(T &&val) -> void {
    buffer_ = std::forward<T>(val);
  }

  template <typename T>
    requires std::is_constructible_v<std::string, T>
  auto Append(T &&val) -> void {
    buffer_ += std::string{std::forward<T>(val)};
  }

  auto Append(const char *ptr, int len) -> void;

  auto RecvAsString(int len) -> std::string;

  auto RecvAsStringAll() -> std::string;

  auto Clear() -> void;

  // NoFind 时返回 -1, 否则返回它是缓冲区第几个字符
  auto TryFind(char ch) -> int;

  auto Size() const -> int;

  auto Data() const -> const char *;

private:
  std::string buffer_;
};

#endif // BUFFER_H
