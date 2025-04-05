//
// Created by sunyuheng365 on 25-4-3.
//
#include "../header/Buffer.h"
#include "../header/Connection.h"
#include "../header/InetAddress.h"
#include "../header/ReactorMainServer.h"

int main() {
  ReactorMainServer server(InetAddress{"127.0.0.1", 5050}, 2);
  std::atomic<int> cnt = 0;
  server.SetMessageCallback([&cnt](std::shared_ptr<Connection> connection,
                                   std::unique_ptr<Buffer> buffer) {
    Buffer buf{std::string("Echo: ") + std::to_string(++cnt) + " "};
    buf.Append(buffer->RecvAsStringAll());
    connection->SendMessage(buf);
  });
  server.Start();
}