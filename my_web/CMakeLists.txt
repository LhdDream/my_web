cmake_minimum_required(VERSION 3.15)
project(my_web)

set(CMAKE_CXX_STANDARD 17)

include_directories(include http util fastcgi config )

aux_source_directory(include/net SOURCE_NET)
aux_source_directory(include/core SOURCE_CORE)
aux_source_directory(http SOURCE_HTTP)
aux_source_directory(util SOURCE_UTIL)
aux_source_directory(fastcgi SOURCE_CGI)
aux_source_directory(config SOURCE_CONFIG)


LINK_LIBRARIES(pthread)
add_definitions("--std=c++17")
add_definitions("-g")
add_definitions("-O3")

add_executable(my_web main.cc ${SOURCE_NET}
        ${SOURCE_CORE}
        ${SOURCE_HTTP}
        ${SOURCE_UTIL}
        ${SOURCE_CGI}
        ${SOURCE_CONFIG}
        )
