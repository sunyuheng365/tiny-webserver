//
// Created by sunyuheng365 on 25-4-5.
//

#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H
#include <memory>

class Buffer;
class HttpRequest;

class HttpContext {
public:
  enum class HttpRequestParaseState {
    kINVALID,         // 无效
    kINVALID_METHOD,  // 无效请求方法
    kINVALID_URL,     // 无效请求路径
    kINVALID_VERSION, // 无效的协议版本号
    kINVALID_HEADER,  // 无效请求头

    START,  // 解析开始
    METHOD, // 请求方法

    BEFORE_URL, // 请求连接前的状态，需要'/'开头
    IN_URL,     // url处理

    BEFORE_URL_PARAM_KEY,   // URL请求参数键之前
    URL_PARAM_KEY,          // URL请求参数键
    BEFORE_URL_PARAM_VALUE, // URL请求参数值之前
    URL_PARAM_VALUE,        // URL请求参数值

    BEFORE_PROTOCOL, // 协议解析之前
    PROTOCOL,        // 协议

    BEFORE_VERSION, // 版本开始前
    VERSION_SPLIT,  // 版本分割符
    VERSION,        // 版本

    HEADER,
    HEADER_KEY, //

    HEADER_BEFORE_COLON, // 请求头冒号之前
    HEADER_AFTER_COLON,  // 请求头冒号
    HEADER_VALUE,        // 请求值

    WHEN_CR, // 遇到一个回车

    CR_LF, // 回车换行

    CR_LF_CR, // 回车换行之后的状态

    BODY = 100, // 请求体

    COMPLETE = 101, // 完成

  };
  HttpContext();
  ~HttpContext() = default;

  bool ParaseRequest(const char *begin, const char *end);
  bool ParaseRequest(const Buffer &buf);
  bool GetCompleteRequest();
  void ResetContextStatus();
  HttpRequest *Request();

private:
  // HttpContext 类管理 Http 报文的生命周期
  std::unique_ptr<HttpRequest> request_;
  HttpRequestParaseState state_;
};

#endif // HTTPCONTEXT_H
