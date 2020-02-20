//
// Created by kiosk on 2020/1/23.
//

#ifndef MY_WEB_PROVIDER_H
#define MY_WEB_PROVIDER_H

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <memory>
#include <thread>


class Provider {
protected:
    Provider();

public:
    int ServerPort() const { return m_port; }

    int ThreadsNumber() const { return m_threads; }

    int User_Keep_Connection() const { return m_keep_connection_ms; }

    std::string ServerIp() const {
        return m_ip;
    }

    std::string ServerWwwRoot() const {
        return m_wwwroot;
    }

    std::string ServerDefault_file() const {
        return m_default_file;
    };

    static Provider &Get() {
        static Provider cp;
        return cp;
    }

    std::string FastCgiIP() const {
        return m_FastCgi_ip;
    }

     int  FastCgiPort() const {
        return m_FastCgi_Port;
    }
    int KeepConnectionNumber() const {
        return m_keep_connection_number;
    }
private:
    int m_port = 8080;
    int m_keep_connection_ms = 800;
    int m_keep_connection_number = 10 ; // 多少用户开启定时器淘汰措施
    unsigned int m_threads = 4;
    std::string m_ip = "127.0.0.1";
    std::string m_wwwroot = "";
    std::string m_default_file = "index.html";
    std::string m_FastCgi_ip = "127.0.0.1";
    int m_FastCgi_Port = 9000;
};

#endif
