//
// Created by kiosk on 2020/1/23.
//

#ifndef MY_WEB_PROVIDER_H
#define MY_WEB_PROVIDER_H

#include <memory>
#include <thread>
#include <atomic>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

class Provider {
protected:
    Provider();

public:
    int ServerPort() const {
        return m_port;
    }

    unsigned int ThreadsNumber() const {
        return m_threads;
    }

    int User_Keep_Connection() const {
        return m_keep_connection_ms;
    }

    std::string ServerIp() const {
        return m_ip;
    }

    std::string ServerWwwRoot() const {
        return m_wwwroot;
    }

    std::string ServerDefault_file() const {
        return m_default_file;
    };

    inline static Provider& Get() {
        static Provider cp;
        return cp;
    }

    std::string FastCgiIP() const {
        return m_FastCgi_ip;
    }

    unsigned int FastCgiPort() const {
        return m_FastCgi_Port;
    }

    int KeepConnectionNumber() const {
        return m_keep_connection_number;
    }
    inline bool GetQuit() const {
        return m_quit;
    }
    void Quit() {
        m_quit = true;
    }
private:
    int m_port = 8080;
    int m_keep_connection_ms = 800;
    int m_keep_connection_number = 10;  // 多少用户开启定时器淘汰措施
    unsigned int m_threads = 4;
    std::string m_ip = "127.0.0.1";
    std::string m_wwwroot = "";
    std::string m_default_file = "index.html";
    std::string m_FastCgi_ip = "127.0.0.1";
    unsigned int m_FastCgi_Port = 9000;

    bool m_quit = false; // 标志是不是回收
};

#endif
