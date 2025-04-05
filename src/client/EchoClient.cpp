#include <arpa/inet.h> // For socket functions
#include <cstring>     // For memset
#include <iostream>
#include <string>
#include <unistd.h> // For close()

int main() {
  const char *server_ip = "127.0.0.1";
  const int server_port = 5050;
  const int buffer_size = 1024;

  // 创建 socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("❌ 创建 socket 失败");
    return 1;
  }

  // 设置服务器地址
  sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr)); // 清零
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(server_port);
  if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
    std::cerr << "❌ 无效的 IP 地址\n";
    return 1;
  }

  // 连接服务器
  if (connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("❌ 连接服务器失败");
    close(sock);
    return 1;
  }

  std::cout << "✅ 成功连接到 " << server_ip << ":" << server_port << std::endl;

  // 向服务器发送消息
  std::string message = "Hello Echo Server!";
  send(sock, message.c_str(), message.size(), 0);
  std::cout << "📤 已发送: " << message << std::endl;

  // 接收服务器返回的数据
  char buffer[buffer_size];
  int received = recv(sock, buffer, buffer_size - 1, 0);
  if (received < 0) {
    perror("❌ 接收失败");
  } else {
    buffer[received] = '\0'; // 添加字符串终止符
    std::cout << "📥 收到响应: " << buffer << std::endl;
  }

  // 关闭 socket
  close(sock);
  std::cout << "🔌 断开连接\n";

  return 0;
}