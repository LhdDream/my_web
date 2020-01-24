//
// Created by kiosk on 19-11-26.
//

#ifndef MYWEB_HTTP_RESPONSE_H
#define MYWEB_HTTP_RESPONSE_H

#include <cstdint>
#include <climits>
#include "Currency.h"
#include <fcntl.h>
#include <functional>
#include <sys/sendfile.h>
#include "../include/Socket.h"
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_map>

constexpr std::string_view html = "text/html";
constexpr std::string_view avi = "video/x-msvideo";
constexpr std::string_view png = "image/png";
constexpr std::string_view txt = "text/plain";
constexpr std::string_view mp3 = "audio/mp3";
constexpr std::string_view pdf = "application/pdf";

inline static std::string_view Type(const std::string_view &mime) {
    if (!mime.compare("html")) {
        return html;
    } else if (!mime.compare("avi")) {
        return avi;
    } else if (!mime.compare("png")) {
        return png;
    } else if (!mime.compare("txt")) {
        return txt;
    } else if (!mime.compare("mp3")) {
        return mp3;
    } else if (!mime.compare("pdf")) {
        return pdf;
    }

}


class Http_Response {
public:
    //组装response
    int Response(const std::unique_ptr<HTTPMessage> &httpMessage, const Socket &fd) {
        bool isPHP = false;
        struct stat64 st{};
        if (stat64(httpMessage->Getpath().data(), &st) < 0) {
            httpMessage->SetStatusCode(Not_Found);
            return -2;
        }
        std::string_view temp = httpMessage->Getpath();
        if(temp.find("php") > 0 ){
            isPHP = true;
        }
        auto it = temp.find('.');
        httpMessage->SetHeader("Content-Type", Type(temp.data() + it + 1));
        httpMessage->SetStatusCode(OK);
        std::string size = std::to_string(st.st_size);
        httpMessage->SetHeader("Content-Length", size);
        m_filesize = st.st_size;
        //接下来文件的读取和fast-cgi的处理
        //文件接口的处理
        // bug 如果多次处理index.html  pdf 出现 basc_string 问题
        std::string_view p;

        httpMessage->ToString(&p);
        if (fd.Write(p.data(), p.size(), 0) < 0) {
            if (errno != EAGAIN && errno != EINTR) {
                return -1;//出现错误关闭
            } else {
                return -2;//发送缓冲区已经满
            }
        }
        if(isPHP)
        {
            return 3;
        }else
            return 0;
    }

    //发送文件使用sendfile 循环
    void ActSendfile(std::string_view path, int fd) {
        //发送文件的处理
        //必须先组成在发送文件
        int fd_ = open64(path.data(), O_RDONLY);
        off64_t offest = 0LL;
        while (offest < m_filesize) {
            size_t count;
            off64_t remain = m_filesize - offest;
            if (remain > SSIZE_MAX) {
                count = SSIZE_MAX;
            } else {
                count = remain;
            }
            auto it = sendfile64(fd, fd_, &offest, count);
            if (it == -1) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    break;
                }
            }
        }
        close(fd_);
    }

private:
    size_t m_filesize = 0;

};

#endif //MYWEB_HTTP_RESPONSE_H
