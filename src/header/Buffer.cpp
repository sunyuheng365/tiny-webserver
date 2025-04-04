//
// Created by sunyuheng365 on 25-4-4.
//

#include "Buffer.h"

#include <iostream>

auto Buffer::Append(const char *ptr, int len) -> void {
  buffer_.append(ptr, len);
}

auto Buffer::RecvAsString(int len) -> std::string {
  if (len > static_cast<int>(buffer_.size())) {
    std::cerr << "Bad RecvAsString Using" << buffer_.size() << std::endl;
    return RecvAsStringAll();
  }
  std::string result{buffer_.begin(), buffer_.begin() + len};
  buffer_.erase(buffer_.begin(), buffer_.begin() + len);
  return result;
}

auto Buffer::RecvAsStringAll() -> std::string {
  std::string result = std::move(buffer_);
  buffer_.clear();
  return result;
}

auto Buffer::Clear() -> void { buffer_.clear(); }

auto Buffer::TryFind(char ch) -> int {
  size_t pos = buffer_.find(ch);
  if (pos == std::string::npos) {
    return -1;
  }
  // 很显然从 ull -> int 是安全的, 因为没想过存超过 INTMAX 的缓冲区
  // 这可能吗?
  return static_cast<int>(pos);
}

auto Buffer::Size() const -> int { return static_cast<int>(buffer_.size()); }

auto Buffer::Data() const -> const char * { return buffer_.data(); }
