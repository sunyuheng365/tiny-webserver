//
// Created by sunyuheng365 on 25-4-5.
//
#include "../../src/log/Log.h"

int main() {
  Log::GetLogger().LogMessage(LogLevel::DEBUG, "start log...");
  int abc = 1024;
  Log::GetLogger().LogMessage(LogLevel::ERROR, "int abc = {}", abc);
  std::string text = "hello world";
  Log::GetLogger().LogMessage(LogLevel::DEBUG, "string text = {}", text);
  Log::GetLogger().LogMessage(LogLevel::INFO, "string text = {}, {}", text);
  Log::GetLogger().LogMessage(LogLevel::WARNING, "string text = {}", text);
  Log::GetLogger().LogMessage(LogLevel::ERROR,
                              "many element: {}, {}, {}, {, {}", text, 1, 102,
                              998244353, 114514);
}