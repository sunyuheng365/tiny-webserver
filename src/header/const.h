//
// Created by sunyuheng365 on 25-4-3.
//

#ifndef CONST_H
#define CONST_H

constexpr int TCP_SERVER_MAX_RECV = 1024;

constexpr int LISTEN_MAX = 128; // 用于 ::listen() 中设置半+全连接队列数量

constexpr int EPOLL_MAX_EVENTS = 65536;

#endif // CONST_H
