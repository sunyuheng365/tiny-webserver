//
// Created by sunyuheng365 on 25-4-3.
//
#include "../header/Buffer.h"
#include "../header/Connection.h"
#include "../header/InetAddress.h"
#include "../header/ReactorMainServer.h"
#include "../log/Log.h"

int main() {
  ReactorMainServer server(InetAddress{"0.0.0.0", 5050}, 2);
  std::atomic<int> cnt = 0;
  server.SetMessageCallback([&cnt](std::shared_ptr<Connection> connection,
                                   std::unique_ptr<Buffer> buffer) {
    LOG_DEBUG(buffer->Data());
    // std::cout << buffer->Data() << std::endl;
    Buffer buf{std::string("Echo: ") + std::to_string(++cnt) + " "};

    buf.Append(buffer->RecvAsStringAll());
    connection->SendMessage(buf);
  });
  server.Start();
}