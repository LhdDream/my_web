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

constexpr std::string_view  OK  = "200";
constexpr std::string_view  Not_Found = "404";

constexpr  std::string_view m_Ok = "OK";
constexpr  std::string_view m_Not_Found = "Not Found";
constexpr  std::string_view m_No = " ";
static std::string_view  HTTP_STATUS_MAP (std::string_view code){
           if(!code.compare(OK))
               return m_Ok;
           else if(!code.compare(Not_Found))
               return m_Not_Found;
           else
               return m_No;
};

//This class returns as many methods as possible with a
//reference to the current
//This class is based  on http request and http response
class HTTPMessage {
public:
    explicit HTTPMessage() : m_method("NONE"),
                             m_path(), m_version("HTTP/1.1"){
    }
    //std::string & or std::string &&
    void SetHeader( std::string_view &&name, std::string_view &&value) {

           m_headers_name.emplace_back(name);
           m_headers_value.emplace_back(value);
        //emplace 使用完美转发
    }

    void SetMethond(const std::string_view &methond) {
        m_method = methond;
    }

    void Setpath( const std::string_view &path) {
        m_path = path;
    }

    void SetVersion(const std::string_view  &version) {
        m_version = version;
    }

    void SetStatusCode(const std::string_view &Code) {
        m_statusCode = Code;
    }


    std::string Getpath()   {
        return std::string {m_path.substr(0,m_path.size())};
    }
    void SetMessageBody(const std::string_view &body) {
        m_body = body;
    }

    void ToString(std::string_view *p) {

        // begin by forming the start line of the message
        if (m_method == "GET" || m_method == "POST") {
            m_output += "HTTP/1.1" ;
            m_output += " ";
            m_output  += m_statusCode;
            m_output += " ";
            m_output += HTTP_STATUS_MAP(m_statusCode);
        }
        m_output += "\r\n";
        for(int i = 0 ; i <m_headers_name.size() ; i++){
              m_output += m_headers_name[i] ;
              m_output += ": ";
              m_output += m_headers_value[i];
              m_output += "\r\n";
          }
        if (!m_body.empty())
            m_output += "Content-Length: " + std::to_string(m_body.size())+ "\r\n";
        m_output += "\r\n";
        m_output += m_body;
        *p = m_output;
    }

    void Clear(bool response) {
        if(response) {
            m_headers_name.clear();
            m_headers_value.clear();
            m_output.clear();
        }
    }


private:
    std::string_view m_method;
    // a status code for this Message
    //if this is a request ,you can ignored this
    std::string_view m_statusCode;
    std::string_view m_path; // Only used for a request
    std::string_view m_version;
    std::vector<std::string_view> m_headers_name;
    std::vector<std::string_view> m_headers_value;
    std::string_view m_body; // 用于二进制安全
    std::string m_output;
    // used to store message bodies
};

#endif //MYWEB_CURRENCY_H