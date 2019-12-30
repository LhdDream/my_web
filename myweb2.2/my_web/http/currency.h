//
// Created by kiosk on 19-11-25.
//

#ifndef MYWEB_CURRENCY_H
#define MYWEB_CURRENCY_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <sstream>

#include <iostream>

// 限定范围的methond

enum class ParserState : uint8_t {
    NONE,
    PARSING_START_LINE,
    START_LINE_REQUEST,
    HEADER_KEY,
    HEADER_VALUE,
    PARSING_BODY,
};

constexpr  std::string_view OK = "OK";
constexpr  std::string_view Not_Found = "Not Found";
constexpr  std::string_view No = " ";
static std::string_view  HTTP_STATUS_MAP (int code){
       switch (code) {
           case 200:{
               return OK;
           }
           case 404:{
               return Not_Found;
           }
           default:
               return No;
       }

};

//This class returns as many methods as possible with a
//reference to the current
//This class is based  on http request and http response
class HTTPMessage {
public:
    explicit HTTPMessage() : m_method("NONE"), m_statusCode(0),
                             m_path(), m_version("HTTP/1.1"){
    }
    //std::string & or std::string &&
    void SetHeader( std::string_view &&name, std::string_view &&value) {
           m_headers_name.emplace_back(name);
           m_headers_value.emplace_back(value);
        //emplace 使用完美转发
    }

    void SetMethond(std::string_view methond) {
        this->m_method = methond;
    }

    void Setpath( std::string_view path) {
        this->m_path = path;
    }

    void SetVersion(std::string_view version) {
        this->m_version = version;
    }

    void SetStatusCode(uint16_t Code) {
        this->m_statusCode = Code;
    }


    std::string Getpath()  noexcept {
        return std::string {m_path.substr(0,m_path.size())};
    }

    void SetMessageBody(std::string_view body) {
        m_body = body;
    }

    void ToString(std::string_view *p) {

        // begin by forming the start line of the message
        if (m_method == "NONE") {
            output += "HTTP/1.1" ;
            output += " ";
            output  += std::to_string (m_statusCode);
            output += " ";
            output += HTTP_STATUS_MAP(m_statusCode);
        } else {
              output += m_method ;
              output += " ";
              output += m_path;
              output += " ";
              output += "HTTP/1.1";
        }
          output += "\r\n";
        for(int i = 0 ; i <m_headers_name.size() ; i++){
              output += m_headers_name[i] ;
              output += ": ";
              output += m_headers_value[i];
              output += "\r\n";
          }
        if (!m_body.empty())
            output += "Content-Length: " + std::to_string(m_body.size())+ "\r\n";
        output += "\r\n";
        output += m_body;
        *p = output;
    }

    void clear(bool response) {
        m_method = "NONE";
        m_statusCode = 0;
        if(response) {
            m_headers_name.clear();
            m_headers_value.clear();
            output.clear();
        }
    }


private:
    std::string_view m_method;
    // a status code for this Message
    //if this is a request ,you can ignored this
    uint16_t m_statusCode;
    std::string_view m_path; // Only used for a request
    std::string_view m_version;
    std::vector<std::string_view> m_headers_name;
    std::vector<std::string_view> m_headers_value;
    std::string_view m_body; // 用于二进制安全
    std::string output;
    // used to store message bodies
};

#endif //MYWEB_CURRENCY_H
