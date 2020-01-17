cmake_minimum_required(VERSION 3.15)
project(my_web)

set(CMAKE_CXX_STANDARD 17)

include_directories(include http util)

aux_source_directory(include SOURCE_INCLUDE)
aux_source_directory(http SOURCE_HTTP)
aux_source_directory(util SOURCE_UTIL)
LINK_LIBRARIES(pthread)
add_definitions("--std=c++17")
add_definitions("-g")
add_definitions("-O3")
#add_executable(my_web main.cpp include/poll.h
#        include/channel.h   include/Buffer.h
#        include/Socket.h include/Socket.cc include/Acceptor.h include/httpserver.h  http/http_request.h
#        http/currency.h http/http_response.h test/add.cc include/Epoll_event.h
#        include/channel.cc  util/threads.h
#        include/Acceptor.cc include/httpserver.cc
#        http/http_msg_handler.h http/http_msg_handler.cc util/Singleton.h util/timer.h util/timer.cc )

add_executable(my_web main.cpp ${SOURCE_INCLUDE}
        ${SOURCE_HTTP}
        ${SOURCE_UTIL}
        )