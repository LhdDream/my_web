//
// Created by kiosk on 2020/2/5.
//

#ifndef My_Web_LOG_H
#define My_Web_LOG_H


//使用
#include <cstdio>
#include <memory>
#include <sstream>

class Logger {
public:

    Logger();

    ~Logger();


    static void init(const std::string& filename, bool append = false);

    std::ostringstream &info();

private:

    Logger(const Logger &r) = delete;

    Logger &operator=(const Logger &r) = delete;

    class Impl;

    std::unique_ptr<Impl> m_impl;
};






#endif 
