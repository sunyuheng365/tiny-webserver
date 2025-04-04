//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef REACTORMAINSERVER_H
#define REACTORMAINSERVER_H

#include <map>
#include <memory>
#include <vector>

class ReactorFollowServer;
class Socket;
class InetAddress;
class ThreadPool;
class Connection;
class Accepter;
class EventLoop;

class ReactorMainServer {
public:
  ReactorMainServer(const InetAddress &address, int sub_server_num);
  ~ReactorMainServer() = default;

  auto Start() -> void;
  auto NewConnection(std::unique_ptr<Socket> socket);

private:
  std::unique_ptr<EventLoop> loop_;
  std::unique_ptr<Accepter> accepter_;
  std::map<int, std::shared_ptr<Connection>> connections_map_;
  std::unique_ptr<ThreadPool> thread_pool_;
  std::vector<std::unique_ptr<ReactorFollowServer>> sub_servers_;
};

#endif // REACTORMAINSERVER_H
