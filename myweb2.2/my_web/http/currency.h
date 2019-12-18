//
// Created by kiosk on 19-11-25.
//

#ifndef MYWEB_CURRENCY_H
#define MYWEB_CURRENCY_H

#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <sstream>
#include "../util/sparsepp/spp.h"
#include <iostream>
using spp::sparse_hash_map;
// 限定范围的methond

enum class ParserState : uint8_t {
    NONE,
    PARSING_START_LINE,
    START_LINE_REQUEST,
    HEADER_KEY,
    HEADER_VALUE,
    PARSING_BODY,
};

static std::string  HTTP_STATUS_MAP (int code){
       switch (code) {
           case 200:{
               return "OK";
           }
           case 404:{
               return "Not Found";
           }
           default:
               return " ";
       }

};

//This class returns as many methods as possible with a
//reference to the current
//This class is based  on http request and http response
class HTTPMessage {
public:
    explicit HTTPMessage() : m_method("NONE"), m_statusCode(0),
                             m_path(), m_version("HTTP/1.1"), m_body(){
    }
    //std::string & or std::string &&
    void SetHeader( std::string  && name, std::string && value) {
            m_headers_[name] = std::move(value);
        //emplace 使用完美转发
    }

    void SetMethond(std::string &&methond) {
        this->m_method = std::move(methond);
    }

    void Setpath( std::string &&path) {
        this->m_path = std::move(path);
    }

    void SetVersion(std::string &&version) {
        this->m_version = std::move(version);
    }

    void SetStatusCode(uint16_t Code) {
        this->m_statusCode = Code;
    }


    std::string &Getpath()  noexcept {
        return m_path;
    }

    void SetMessageBody( std::string &&body) {
        this->m_body = std::move(body);
    }

    std::string ToString() {
        std::stringstream output;
        // begin by forming the start line of the message
        if (m_method == "NONE") {
            output << "HTTP/1.1" << " " << m_statusCode << " ";
            output << HTTP_STATUS_MAP(m_statusCode);
        } else {
            output << m_method << " ";
            output << m_path << " ";
            output << "HTTP/1.1";
        }
        output << "\r\n";
        for(auto && c :m_headers_){
              output << c.first << ": " <<c.second<< "\r\n";
          }
        if (!m_body.empty())
            output << "Content-Length: " << m_body.size() << "\r\n";
        output << "\r\n";
        output << m_body;
        return output.str();
    }

    void clear(bool response) {
        m_method = "NONE";
        m_statusCode = 0;
    }


private:
    std::string m_method;
    // a status code for this Message
    //if this is a request ,you can ignored this
    uint16_t m_statusCode;
    std::string m_path; // Only used for a request
    std::string m_version;
    sparse_hash_map<std::string,std::string> m_headers_;
    std::string m_body; // 用于二进制安全

    // used to store message bodies
};

#endif //MYWEB_CURRENCY_H
