cmake_minimum_required(VERSION 3.14)
project(myweb)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

LINK_LIBRARIES(-lpthread --std=c++14 -g )
add_executable(myweb main.cpp include/poll.h include/Eventloop.h include/newthread.h
         include/channel.h   include/Buffer.h
        include/Socket.h include/Socket.cc include/Acceptor.h include/httpserver.h  http/http_request.h
        http/memorypool.h http/currency.h http/http_response.h test/add.cc include/Epoll_event.h
        util/Timer.h include/Buffer.cc include/channel.cc
        )
