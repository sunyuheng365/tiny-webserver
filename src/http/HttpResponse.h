//
// Created by sunyuheng365 on 25-4-5.
//

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include <map>
#include <string>

class HttpResponse {
public:
  enum class HttpStatusCode {
    kUnknown = 1,
    k100Continue = 100,
    k200K = 200,
    k301K = 301,
    k302K = 302,
    k400BadRequest = 400,
    k403Forbidden = 403,
    k404NotFound = 404,
    k500InternalServerError = 500
  };
  // enum class HttpBodyType {
  // HTML_TYPE,
  // FILE_TYPE,
  // };

  HttpResponse();
  ~HttpResponse() = default;

  auto SetStatusCode(HttpStatusCode code) -> void;

  auto SetBody(std::string body) -> void;

  auto GetContentLength() const -> int;

  auto SetHeader(const std::string &key, const std::string &value) -> void;

  auto SetClose(bool close) -> void;
  auto GetIsClose() const -> bool;

  auto GetAsStringHeader() const -> std::string;

  auto GetAsStringBody() const -> const std::string &;

  auto GetAsString() const -> std::string;

private:
  std::map<std::string, std::string> headers_;

  HttpStatusCode status_code_;
  std::string status_message_;

  std::string body_;

  // HttpBodyType body_type_;

  bool is_closed;
};

#endif // HTTPRESPONSE_H
