//
// Created by sunyuheng365 on 25-4-4.
//

#ifndef REACTORFOLLOWSERVER_H
#define REACTORFOLLOWSERVER_H
#include "EventLoop.h"

#include <memory>

class ReactorFollowServer {

public:

private:
  std::unique_ptr<EventLoop> loop_;

};

#endif // REACTORFOLLOWSERVER_H
