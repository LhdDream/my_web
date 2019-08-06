cmake_minimum_required(VERSION 3.14)
project(myweb)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

LINK_LIBRARIES(-lpthread --std=c++11)
add_executable(myweb main.cpp include/poll.h include/Eventloop.h
        include/Eventloop.cc include/channel.h include/channel.cc include/poll.cc include/Buffer.h
        include/Buffer.cc include/httprequest.h include/HttpContext.h include/HttpContext.cc include/httpresponse.h include/httpresponse.cc include/Socket.h include/Socket.cc include/Acceptor.h include/Acceptor.cc include/Tcpconnection.h include/Tcpconnection.cc include/httpserver.h include/httpserver.cc include/threadpool.h include/threadpoll.cc)
