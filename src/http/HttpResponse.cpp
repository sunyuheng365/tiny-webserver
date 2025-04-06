//
// Created by sunyuheng365 on 25-4-5.
//

#include "HttpResponse.h"
#include <sstream>

HttpResponse::HttpResponse() : is_closed(false) {
  SetStatusCode(HttpStatusCode::kUnknown);
}

auto HttpResponse::SetStatusCode(HttpStatusCode code) -> void {
  status_code_ = code;
  switch (status_code_) {
  case HttpStatusCode::k100Continue:
    status_message_ = "Continue";
    break;
  case HttpStatusCode::k200K:
    status_message_ = "OK";
    break;
  case HttpStatusCode::k301K:
    status_message_ = "Multiple Choices";
    break;
  case HttpStatusCode::k302K:
    status_message_ = "Moved Permanently";
    break;
  case HttpStatusCode::k400BadRequest:
    status_message_ = "Bad Request";
    break;
  case HttpStatusCode::k403Forbidden:
    status_message_ = "Forbidden";
    break;
  case HttpStatusCode::k404NotFound:
    status_message_ = "Not Found";
    break;
  case HttpStatusCode::k500InternalServerError:
    status_message_ = "Internal Server Error";
    break;
  case HttpStatusCode::kUnknown:
  default:
    status_message_ = "Unknown";
    break;
  }
}

auto HttpResponse::SetBody(std::string body) -> void {
  body_ = std::move(body);
}

auto HttpResponse::GetContentLength() const -> int {
  return static_cast<int>(body_.size());
}

auto HttpResponse::SetHeader(const std::string &key, const std::string &value)
    -> void {
  headers_[key] = value;
}

auto HttpResponse::SetClose(bool close) -> void { is_closed = close; }

auto HttpResponse::GetIsClose() const -> bool { return is_closed; }

auto HttpResponse::GetAsStringHeader() const -> std::string {
  std::ostringstream os;
  os << "HTTP/1.0 " << static_cast<int>(status_code_) << " " << status_message_
     << "\r\n";

  os << "Connection: close\r\n";
  // os << "Connection: " << (is_closed ? "close" : "Keep-Alive") << "\r\n";

  os << "Content-Length: " << body_.size() << "\r\n";

  // os << "Cache-Control: no-cache, no-store, must-revalidate\r\n";

  os << "Content-Type: text/html\r\n";

  for (const auto &[key, value] : headers_) {
    os << key << ": " << value << "\r\n";
  }

  os << "\r\n";

  return os.str();
}

auto HttpResponse::GetAsStringBody() const -> const std::string & {
  return body_;
}

auto HttpResponse::GetAsString() const -> std::string {
  return GetAsStringHeader() + GetAsStringBody();
}
