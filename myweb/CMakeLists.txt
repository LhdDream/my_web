cmake_minimum_required(VERSION 3.14)
project(myweb)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

LINK_LIBRARIES(-lpthread --std=c++11)
add_executable(myweb main.cpp include/Socket.h include/Socket.cc include/poll.h include/Eventloop.h
        include/Eventloop.cc include/channel.h include/channel.cc include/poll.cc include/threadpool.hpp include/Buffer.h
        include/Buffer.cc)
