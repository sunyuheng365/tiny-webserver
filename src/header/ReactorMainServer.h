//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef REACTORMAINSERVER_H
#define REACTORMAINSERVER_H

#include "Accepter.h"
#include "Buffer.h"
#include "Connection.h"
#include "EventLoop.h"
#include "ReactorSubServer.h"
#include "ThreadPool.h"
#include <functional>
#include <map>
#include <memory>
#include <vector>

class Socket;
class InetAddress;

class ReactorMainServer {
public:
  ReactorMainServer(const InetAddress &address, int sub_server_num);
  ~ReactorMainServer() = default;

  auto Start() -> void;
  auto NewConnection(std::unique_ptr<Socket> socket) -> void;
  auto SetMessageCallback(
      std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
          callback) -> void;

  auto Hash(int socket_fd) -> int;

private:
  std::unique_ptr<EventLoop> loop_;
  std::unique_ptr<Accepter> accepter_;
  std::map<int, std::shared_ptr<Connection>> connections_map_;
  std::unique_ptr<ThreadPool> thread_pool_;
  std::vector<std::unique_ptr<ReactorSubServer>> sub_servers_;

  std::function<void(std::shared_ptr<Connection>, std::unique_ptr<Buffer>)>
      message_callback_;
};

#endif // REACTORMAINSERVER_H
