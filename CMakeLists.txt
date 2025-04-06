cmake_minimum_required(VERSION 3.30)
project(tiny_webserver)

set(CMAKE_CXX_STANDARD 20)

add_executable(tiny-webserver
        src/server/EchoServer.cpp
        src/header/Accepter.h
        src/header/Accepter.cpp
        src/header/Channel.h
        src/header/Channel.cpp
        src/header/Connection.h
        src/header/Connection.cpp
        src/header/const.h
        src/header/Epoll.h
        src/header/Epoll.cpp
        src/header/InetAddress.h
        src/header/InetAddress.cpp
        src/header/Socket.h
        src/header/Socket.cpp
        src/header/EventLoop.cpp
        src/header/EventLoop.h
        src/header/ThreadPool.cpp
        src/header/ThreadPool.h
        src/header/Buffer.cpp
        src/header/Buffer.h
        src/header/ReactorMainServer.cpp
        src/header/ReactorMainServer.h
        src/header/ReactorSubServer.cpp
        src/header/ReactorSubServer.h
        src/log/Log.h

)

add_executable(http_server
        src/server/HttpServer.cpp
        src/header/Accepter.h
        src/header/Accepter.cpp
        src/header/Channel.h
        src/header/Channel.cpp
        src/header/Connection.h
        src/header/Connection.cpp
        src/header/const.h
        src/header/Epoll.h
        src/header/Epoll.cpp
        src/header/InetAddress.h
        src/header/InetAddress.cpp
        src/header/Socket.h
        src/header/Socket.cpp
        src/header/EventLoop.cpp
        src/header/EventLoop.h
        src/header/ThreadPool.cpp
        src/header/ThreadPool.h
        src/header/Buffer.cpp
        src/header/Buffer.h
        src/header/ReactorMainServer.cpp
        src/header/ReactorMainServer.h
        src/header/ReactorSubServer.cpp
        src/header/ReactorSubServer.h
        src/log/Log.h
        src/http/HttpContext.h
        src/http/HttpContext.cpp
        src/http/HttpRequest.h
        src/http/HttpRequest.cpp
        src/http/HttpResponse.cpp
        src/http/HttpResponse.h
)

add_executable(client
        src/client/EchoClient.cpp
)

add_executable(log_test
        test/log/log_tester.cpp
        src/log/Log.h
)
