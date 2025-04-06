//
// Created by sunyuheng365 on 25-4-5.
//

#include "HttpRequest.h"

#include "../log/Log.h"

HttpRequest::HttpRequest()
    : method_(Method::INVALID), version_(Version::INVALID) {}

auto HttpRequest::SetVersion(const std::string &version) -> void {
  if (version == "1.0") {
    version_ = Version::HTTP_1_0;
  } else if (version == "1.1") {
    version_ = Version::HTTP_1_1;
  } else {
    LOG_ERROR("HttpRequest::SetVersion: INVALID HTTP VERSION");
    version_ = Version::INVALID;
  }
}

auto HttpRequest::GetVersion() const -> Version { return version_; }

auto HttpRequest::GetVersionAsStr() const -> std::string {
  if (version_ == Version::HTTP_1_0) {
    return "HTTP/1.0";
  }
  if (version_ == Version::HTTP_1_1) {
    return "HTTP/1.1";
  }

  LOG_ERROR("HttpRequest::GetVersion: INVALID HTTP VERSION");
  return "Invalid";
}

auto HttpRequest::SetMethod(const std::string &method) -> void {
  if (method == "GET") {
    method_ = Method::GET;
  } else if (method == "POST") {
    method_ = Method::POST;
  } else if (method == "HEAD") {
    method_ = Method::HEAD;
  } else if (method == "PUT") {
    method_ = Method::PUT;
  } else if (method == "DELETE") {
    method_ = Method::DELETE;
  } else {
    LOG_ERROR("HttpRequest::SetMethod: INVALID METHOD");
    method_ = Method::INVALID;
  }
}

auto HttpRequest::GetMethod() const -> Method { return method_; }

auto HttpRequest::GetMethodAsStr() const -> std::string {
  if (method_ == Method::GET) {
    return "GET";
  }
  if (method_ == Method::POST) {
    return "POST";
  }
  if (method_ == Method::HEAD) {
    return "HEAD";
  }
  if (method_ == Method::PUT) {
    return "PUT";
  }
  if (method_ == Method::DELETE) {
    return "DELETE";
  }
  LOG_ERROR("HttpRequest::GetMethod: INVALID METHOD");
  return "Invalid";
}

auto HttpRequest::SetUrl(const std::string &url) -> void { url_ = url; }

auto HttpRequest::GetUrl() const -> std::string { return url_; }

auto HttpRequest::SetRequestParams(const std::string &key,
                                   const std::string &value) -> void {
  request_params_[key] = value;
}

auto HttpRequest::GetRequestParams(const std::string &key,
                                   std::string &val) const -> bool {
  auto it = request_params_.find(key);
  if (it == request_params_.end()) {
    val.clear();
    return false;
  }
  val = it->second;
  return true;
}

auto HttpRequest::GetAllRequestParams() const
    -> const std::map<std::string, std::string> & {
  return request_params_;
}

auto HttpRequest::SetProtocol(const std::string &protocol) -> void {
  protocol_ = protocol;
}

auto HttpRequest::GetProtocol() const -> std::string { return protocol_; }

auto HttpRequest::SetHeaders(const std::string &key, const std::string &value)
    -> void {
  headers_[key] = value;
}

auto HttpRequest::GetHeaders(const std::string &key, std::string &val) -> bool {
  auto it = headers_.find(key);
  if (it == headers_.end()) {
    val.clear();
    return false;
  }
  val = it->second;
  return true;
}

auto HttpRequest::GetAllHeaders() const
    -> const std::map<std::string, std::string> & {
  return headers_;
}

auto HttpRequest::SetBody(const std::string &body) -> void {
  body_ = body;
}

auto HttpRequest::GetBody() const -> std::string {
  return body_;
}
