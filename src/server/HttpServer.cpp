//
// Created by sunyuheng365 on 25-4-3.
//
#include "../header/Buffer.h"
#include "../header/Connection.h"
#include "../header/InetAddress.h"
#include "../header/ReactorMainServer.h"
#include "../http/HttpContext.h"
#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"
#include "../log/Log.h"

#include <fstream>

std::unique_ptr<HttpResponse> RequestFile(const std::string &filename) {
  // std::ifstream file(filename);
  // if (!file.is_open()) {
  // LOG_ERROR("RequestFile could not be opened for reading");
  // return std::make_unique<HttpResponse>();
  // }

  std::string content("hello world");

  std::unique_ptr<HttpResponse> response = std::make_unique<HttpResponse>();
  response->SetBody(content);
  return response;
}

std::unique_ptr<HttpResponse> Repaly(HttpRequest *request) {
  HttpRequest::Method method = request->GetMethod();
  switch (method) {
  case HttpRequest::Method::GET: {
    std::string url = request->GetUrl();
    if (url == "/" || url == "/index.html") {
      url = "/home/sunyuheng365/CLionProjects/tiny-webserver/files/index.html";
      std::unique_ptr<HttpResponse> response = RequestFile(url);
      response->SetStatusCode(HttpResponse::HttpStatusCode::k200K);
      return response;
    }
  }
  default: {
    LOG_ERROR("Waiting to be...");
    std::unique_ptr<HttpResponse> response = RequestFile(
        "/home/sunyuheng365/CLionProjects/tiny-webserver/files/html404.html");
    response->SetStatusCode(HttpResponse::HttpStatusCode::k400BadRequest);
    return response;
  }
  }
}

int main() {
  ReactorMainServer server(InetAddress{"0.0.0.0", 1234}, 4);
  // std::atomic<int> cnt = 0;

  server.SetMessageCallback([](std::shared_ptr<Connection> connection,
                               std::unique_ptr<Buffer> buffer) {
    LOG_DEBUG(buffer->Data());

    HttpContext context;
    context.ParaseRequest(*buffer);
    std::unique_ptr<HttpResponse> response{nullptr};
    if (!context.GetCompleteRequest()) {
      response = RequestFile(
          "/home/sunyuheng365/CLionProjects/tiny-webserver/files/html404.html");
      response->SetStatusCode(HttpResponse::HttpStatusCode::k400BadRequest);
    } else {
      response = Repaly(context.Request());
    }
    LOG_DEBUG("!!response : \n{}", response->GetAsString());
    connection->SendMessage(Buffer{response->GetAsString()});
  });
  server.Start();
}