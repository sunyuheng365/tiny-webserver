//
// Created by sunyuheng365 on 25-4-5.
//

#include "HttpContext.h"

#include "../header/Buffer.h"
#include "HttpRequest.h"

#define CR '\r'
#define LF '\n'

HttpContext::HttpContext()
    : request_(std::make_unique<HttpRequest>()),
      state_(HttpRequestParaseState::START) {}

bool HttpContext::ParaseRequest(const char *begin, const char *end) {
  const char *start = begin;
  const char *cur = start;
  const char *colon =
      cur; // 对于 URL:PARAMS 和 HeaderKey: HeaderValue保存中间的位置
  while (state_ != HttpRequestParaseState::kINVALID &&
         state_ != HttpRequestParaseState::COMPLETE && cur < end) {

    char ch = *cur; // 当前字符
    // std::cout << ch;
    switch (state_) {
    case HttpRequestParaseState::START: {
      if (ch == CR || ch == LF || isblank(ch)) {
        // 遇到空格，换行和回车都继续。
      } else if (isupper(ch)) {
        // 遇到大写字母，说明遇到了METHOD
        state_ = HttpRequestParaseState::METHOD;
      } else {
        state_ = HttpRequestParaseState::kINVALID;
      }
      break;
    }
    case HttpRequestParaseState::METHOD: {
      if (isupper(ch)) {
        // 如果是大写字母，则继续
      } else if (isblank(ch)) {
        // 遇到空格表明，METHOD方法解析结束，当前处于即将解析URL，start进入下一个位置
        request_->SetMethod(std::string(start, cur));
        state_ = HttpRequestParaseState::BEFORE_URL;
        start = cur + 1; // 更新下一个指标的位置
      } else {
        state_ = HttpRequestParaseState::kINVALID;
      }
      break;
    }
    case HttpRequestParaseState::BEFORE_URL: {
      // 对请求连接前的处理，请求连接以'/'开头
      if (ch == '/') {
        // 遇到/ 说明遇到了URL，开始解析
        state_ = HttpRequestParaseState::IN_URL;
      } else if (isblank(ch)) {

      } else {
        state_ = HttpRequestParaseState::kINVALID;
      }
      break;
    }
    case HttpRequestParaseState::IN_URL: {
      // 进入url中
      if (ch == '?') {
        // 当遇到?时，表明进入了request params的处理。
        request_->SetUrl(std::string(start, cur));
        start = cur + 1;
        state_ = HttpRequestParaseState::BEFORE_URL_PARAM_KEY;

      } else if (isblank(ch)) {
        // 说明没有请求参数，请求路径完成
        request_->SetUrl(std::string(start, cur));
        start = cur + 1;
        state_ = HttpRequestParaseState::BEFORE_PROTOCOL;
      }
      break;
    }
    case HttpRequestParaseState::BEFORE_URL_PARAM_KEY: {
      // std::cout << ch << std::endl;
      if (isblank(ch) || ch == CR || ch == LF) {
        // 当开始进入url params时，遇到了空格，换行等，则不合法
        // std::cout << ch << std::endl;
        state_ = HttpRequestParaseState::kINVALID;
      } else {
        state_ = HttpRequestParaseState::URL_PARAM_KEY;
      }
      break;
    }
    case HttpRequestParaseState::URL_PARAM_KEY: {
      if (ch == '=') {
        // 遇到= 说明key解析完成
        colon = cur;
        state_ = HttpRequestParaseState::BEFORE_URL_PARAM_VALUE;
      } else if (isblank(ch)) {
        state_ = HttpRequestParaseState::kINVALID;
      }
      break;
    }
    case HttpRequestParaseState::BEFORE_URL_PARAM_VALUE: {
      if (isblank(ch) || ch == LF || ch == CR) {
        state_ = HttpRequestParaseState::kINVALID;
      } else {
        state_ = HttpRequestParaseState::URL_PARAM_VALUE;
      }
      break;
    }

    case HttpRequestParaseState::URL_PARAM_VALUE: {
      if (ch == '&') {
        // 说明遇到了下一个请求参数
        state_ = HttpRequestParaseState::BEFORE_URL_PARAM_KEY;
        request_->SetRequestParams(std::string(start, colon),
                                   std::string(colon + 1, cur));
        start = cur + 1;
      }
      if (isblank(ch)) {
        // 遇到空格，说明解析结束。
        request_->SetRequestParams(std::string(start, colon),
                                   std::string(colon + 1, cur));
        start = cur + 1;
        state_ = HttpRequestParaseState::BEFORE_PROTOCOL;
      } else {
      }
      break;
    }
    case HttpRequestParaseState::BEFORE_PROTOCOL: {
      // std::cout << std::string(start, end) << std::endl;
      if (isblank(ch)) {
        // nothing
      } else {
        state_ = HttpRequestParaseState::PROTOCOL;
      }
      break;
    }
    case HttpRequestParaseState::PROTOCOL: {
      if (ch == '/') {
        request_->SetProtocol(std::string(start, cur));
        start = cur + 1;
        state_ = HttpRequestParaseState::BEFORE_VERSION;
      } else {
      }
      break;
    }

    case HttpRequestParaseState::BEFORE_VERSION: {
      if (isdigit(ch)) {
        state_ = HttpRequestParaseState::VERSION;
      } else {
        state_ = HttpRequestParaseState::kINVALID;
      }
      break;
    }

    case HttpRequestParaseState::VERSION: {
      if (ch == CR) {
        // 说明结束了
        request_->SetVersion(std::string(start, cur));
        start = cur + 1;
        state_ = HttpRequestParaseState::WHEN_CR;
      } else if (isdigit(ch) || ch == '.') {

      } else {
        state_ = HttpRequestParaseState::kINVALID;
      }
      break;
    }

    // 需要注意的是，对header的解析并不鲁棒
    case HttpRequestParaseState::HEADER_KEY: {
      if (ch == ':') {
        colon = cur;
        state_ = HttpRequestParaseState::HEADER_VALUE;
      }
      break;
    }
    case HttpRequestParaseState::HEADER_VALUE: {
      if (isblank(ch)) {

      } else if (ch == CR) {
        request_->SetHeaders(std::string(start, colon),
                             std::string(colon + 2, cur));
        start = cur + 1;
        state_ = HttpRequestParaseState::WHEN_CR;
      }
      break;
    }

    case HttpRequestParaseState::WHEN_CR: {
      if (ch == LF) {
        // 如果遇到了'\n'之后遇到了'\r'，那就意味着这一行结束了
        start = cur + 1;
        state_ = HttpRequestParaseState::CR_LF;
      } else {
        state_ = HttpRequestParaseState::kINVALID;
      }
      break;
    }
    case HttpRequestParaseState::CR_LF: {
      if (ch == CR) {
        // 说明遇到了空行，大概率时结束了
        state_ = HttpRequestParaseState::CR_LF_CR;
      } else if (isblank(ch)) {
        state_ = HttpRequestParaseState::kINVALID;
      } else {
        state_ = HttpRequestParaseState::HEADER_KEY;
      }
      break;
    }
    case HttpRequestParaseState::CR_LF_CR: {
      // 判断是否需要解析请求体
      //
      if (ch == LF) {
        // 这就意味着遇到了空行，要进行解析请求体了
        if (std::string length;
            request_->GetHeaders("Content-Length", length)) {
          if (atoi(length.c_str()) > 0) {
            state_ = HttpRequestParaseState::BODY;
          } else {
            state_ = HttpRequestParaseState::COMPLETE;
          }
        } else {
          if (cur < end) {
            state_ = HttpRequestParaseState::BODY;
          } else {
            state_ = HttpRequestParaseState::COMPLETE;
          }
        }
        start = cur + 1;
      } else {
        state_ = HttpRequestParaseState::kINVALID;
      }
      break;
    }

    case HttpRequestParaseState::BODY: {

      int bodylength = static_cast<int>(end - cur);
      request_->SetBody(std::string(start, start + bodylength));

      if (std::string length; request_->GetHeaders("Content-Length", length) &&
                              bodylength >= atoi(length.c_str())) {
        state_ = HttpRequestParaseState::COMPLETE;
      }
      break;
    }

    default:
      state_ = HttpRequestParaseState::kINVALID;
      break;
    }

    cur++;
  }
  return state_ == HttpRequestParaseState::COMPLETE ||
         state_ == HttpRequestParaseState::BODY;
}

bool HttpContext::ParaseRequest(const Buffer &buf) {
  return ParaseRequest(buf.Data(), buf.Data() + buf.Size());
}

bool HttpContext::GetCompleteRequest() {
  return state_ == HttpRequestParaseState::COMPLETE ||
         state_ == HttpRequestParaseState::BODY;
}

void HttpContext::ResetContextStatus() {
  request_ = std::make_unique<HttpRequest>();
  state_ = HttpRequestParaseState::START;
}

HttpRequest *HttpContext::Request() { return request_.get(); }