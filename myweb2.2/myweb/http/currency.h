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
#include "memorypool.h"
// 限定范围的methond
enum  class Methond : uint8_t
{
    NONE,
    GET,
    POST
};
enum class ParserState :uint8_t
{
    NONE,
    PARSING_START_LINE,
    START_LINE_REQUEST,
    HEADER_KEY,
    HEADER_VALUE,
    PARSING_BODY,
};

inline std::string MethondTostring(const Methond& methond)
{
    switch (methond)
    {
        case  Methond ::NONE:
            return "NONE";
        case  Methond ::GET :
            return "GET";
        case  Methond :: POST:
            return "POST";
        default:
            return "";
    }
}

inline Methond  StringtoMethod(const std::string & methond)
{
    if(methond == "NONE")
        return Methond::NONE;
    if(methond == "GET")
        return Methond::GET;
    if(methond == "POST")
        return Methond::POST;

    return Methond ::NONE;
}
inline std::string StatusFromStatusCode(const uint16_t statusCode)
{
    switch (statusCode)
    {
        case 200:
            return "OK";
        case 400:
            return "Bad Request";
        case 404:
            return "Not Found";
        default:
            return "Undefined";
    }
}


//This class returns as many methods as possible with a
//reference to the current
//This class is based  on http request and http response
class HTTPMessage
{
public:
    explicit HTTPMessage() : m_method(Methond::NONE),m_statusCode(0),
    m_path(),m_version("HTTP/1.1"),m_body()
    {
    }
    void SetHeader(const std::string & name,const std::string &value)
    {
        m_headers[name] = value;
    }
    void SetMethond(const Methond& methond)
    {
        this->m_method = methond;
    }
    void Setpath(const std::string &path)
    {
        this->m_path = path;
    }
    void SetVersion(const std::string &version){
        this->m_version = version;
    }
    void SetStatusCode(uint16_t Code)
    {
        this->m_statusCode = Code;
    }

    inline  std::string GetstatusMessage() const noexcept {
        return StatusFromStatusCode(m_statusCode);
    }
    inline size_t  ContentLength() noexcept {
        return m_body.size();
    }
    inline  size_t  HeaderCount() noexcept {
        return m_headers.size();
    }
    inline  std::string Getpath() noexcept {
        return m_path;
    }
    void SetMessageBody(const std::string & body)
    {
        this->m_body  = body ;
    }

    std::string ToString() const
    {
        std::stringstream output;
        // begin by forming the start line of the message
        if (m_method == Methond ::NONE)
        {
            output << "HTTP/1.1" << " " << m_statusCode << " ";
            output  << GetstatusMessage() ;
        }
        else
        {
            output << MethondTostring(m_method) << " ";
            output << m_path << " ";
            output << "HTTP/1.1";
        }
        output << "\r\n";
        for (auto& header : m_headers)
            output << header.first << ": " << header.second << "\r\n";
        if (!m_body.empty())
            output << "Content-Length: " << m_body.size() << "\r\n";
        output << "\r\n";
        output << m_body;
        return output.str();
    }
    void clear(bool response)
    {
        if(response)
            m_path.clear();
        m_method = Methond ::NONE;
        m_statusCode = 0;
        m_headers.clear();
        m_version.clear();
        m_body.clear();
    }
private:
    Methond m_method ;
    // a status code for this Message
    //if this is a request ,you can ignored this
    uint16_t  m_statusCode  ;
    std::string m_path ; // Only used for a request
    std::string m_version;
    std::unordered_map<std::string,std::string> m_headers;
    std::string m_body ; // 用于二进制安全
    // used to store message bodies
};

#endif //MYWEB_CURRENCY_H
