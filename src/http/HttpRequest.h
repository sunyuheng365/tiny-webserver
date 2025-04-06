//
// Created by sunyuheng365 on 25-4-5.
//

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <map>
#include <string>
#include <vector>

class HttpRequest {

public:
  enum class Method { INVALID, GET, POST, HEAD, PUT, DELETE };
  enum class Version {
    INVALID,
    HTTP_1_0,
    HTTP_1_1,
  };

  HttpRequest();
  ~HttpRequest() = default;

  auto SetVersion(const std::string &version) -> void;
  auto GetVersion() const -> Version;
  auto GetVersionAsStr() const -> std::string;

  auto SetMethod(const std::string &method) -> void;
  auto GetMethod() const -> Method;
  auto GetMethodAsStr() const -> std::string;

  auto SetUrl(const std::string &url) -> void;
  auto GetUrl() const -> std::string;

  auto SetRequestParams(const std::string &key, const std::string &value)
      -> void;
  auto GetRequestParams(const std::string &key, std::string &val) const -> bool;
  auto GetAllRequestParams() const
      -> const std::map<std::string, std::string> &;

  auto SetProtocol(const std::string &protocol) -> void;
  auto GetProtocol() const -> std::string;

  auto SetHeaders(const std::string &key, const std::string &value) -> void;
  auto GetHeaders(const std::string &key, std::string &val) -> bool;
  auto GetAllHeaders() const -> const std::map<std::string, std::string> &;

  auto SetBody(const std::string &body) -> void;
  auto GetBody() const -> std::string;

private:
  Method method_;
  Version version_;

  std::string url_;
  std::map<std::string, std::string> request_params_;
  std::string protocol_;                       // 协议
  std::map<std::string, std::string> headers_; // 请求头
  std::string body_;                           // 请求体
};

#endif // HTTPREQUEST_H
