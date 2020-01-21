//
// Created by kiosk on 19-11-25.
//

#ifndef MYWEB_CURRENCY_H
#define MYWEB_CURRENCY_H

#include <cstdint>
#include <list>
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_map>
// 限定范围的methond

enum class ParserState : uint8_t {
    NONE,
    PARSING_START_LINE,
    START_LINE_REQUEST,
    HEADER_KEY,
    HEADER_VALUE,
    PARSING_BODY,
};

constexpr std::string_view OK = "200";
constexpr std::string_view Not_Found = "404";

constexpr std::string_view m_Ok = "OK";
constexpr std::string_view m_Not_Found = "Not Found";
constexpr std::string_view m_No = " ";

static std::string_view HTTP_STATUS_MAP(std::string_view code) {
    if (code.compare(OK) == 0)
        return m_Ok;
    else if (code.compare(Not_Found) == 0)
        return m_Not_Found;
    else
        return m_No;
};


class HTTPMessage {
public:
    explicit HTTPMessage() : m_keep_alive(false),
                             m_method("NONE"),
                             m_path(), m_version("HTTP/1.1") {
    }

    //std::string & or std::string &&
    void SetHeader(std::string_view &&name, std::string_view &&value) {
        if (value.compare("Keep-Alive") == 0) {
            Set_Keep_Alive(true);
        }
        m_headers_name.emplace_back(name);
        m_headers_value.emplace_back(value);
        //emplace 使用完美转发
    }

    void SetMethond(const std::string_view &methond) {
        m_method = methond;
    }

    void Setpath(const std::string_view &path) {
        m_path = path;
    }

    void SetVersion(const std::string_view &version) {
        m_version = version;
    }

    void SetStatusCode(const std::string_view &Code) {
        m_statusCode = Code;
    }


    std::string Getpath() {
        return std::string{m_path.substr(0, m_path.size())};
    }

    void SetMessageBody(const std::string_view &body) {
        m_body = body;
    }

    void ToString(std::string_view *p) {

        if (m_method == "GET" || m_method == "POST") {
            m_output += "HTTP/1.1";
            m_output += " ";
            m_output += m_statusCode;
            m_output += " ";
            m_output += HTTP_STATUS_MAP(m_statusCode);
        }
        m_output += "\r\n";
        auto value = m_headers_value.begin();
        for (auto &c : m_headers_name) {
            m_output += c;
            m_output += ": ";
            m_output += *value;
            value++;
            m_output += "\r\n";
        }
        if (!m_body.empty())
            m_output += "Content-Length: " + std::to_string(m_body.size()) + "\r\n";
        m_output += "\r\n";
        m_output += m_body;
        *p = m_output;
    }

    void Clear() {
        m_headers_name.clear();
        m_headers_value.clear();
        m_output.clear();
    }

    bool Keep_Alive() {
        return m_keep_alive;
    }

    void Set_Keep_Alive(bool state) {
        m_keep_alive = state;
    }

private:
    bool m_keep_alive;
    std::string_view m_method;
    std::string_view m_statusCode;
    std::string_view m_path;
    std::string_view m_version;
    std::list<std::string_view> m_headers_name;
    std::list<std::string_view> m_headers_value;
    std::string_view m_body;
    std::string m_output;

};

#endif //MYWEB_CURRENCY_H
