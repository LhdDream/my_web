//
// Created by kiosk on 19-11-25.
//

#ifndef MYWEB_HTTP_REQUEST_H
#define MYWEB_HTTP_REQUEST_H

#include "currency.h"

class HTTPMessageParser {
public:
    void Parse(HTTPMessage *httpMessage, const std::vector<char> &buffer, int readsize) {
        ParserState state = ParserState::PARSING_START_LINE;
        //解析头一行
        std::string temp;
        bool skipNext = false;
        std::string headerkey;
        bool hasMessageBody = false;

        size_t bodyStartIndex = 0;

        for (std::string::size_type i = 0; i < buffer.size(); ++i) {

            if (skipNext) // 跳过下一行
            {
                skipNext = false;
                continue;
            }
            if (state == ParserState::PARSING_BODY) {
                hasMessageBody = true;
                bodyStartIndex = i;
                break;
            }
            if (state == ParserState::PARSING_START_LINE) {
                if (buffer[i] == ' ') {
                    httpMessage->SetMethond(StringtoMethod(temp));
                    state = ParserState::START_LINE_REQUEST;
                    //开始解析请求
                    temp.clear();
                    continue;
                }
            } else if (state == ParserState::START_LINE_REQUEST)
                //解析行请求
            {
                if (buffer[i] == ' ') {
                    httpMessage->Setpath(temp);
                    temp.clear();
                    continue;
                } else if (buffer[i] == '\r') {
                    httpMessage->SetVersion(temp);
                    temp.clear();
                    state = ParserState::HEADER_KEY;
                    skipNext = true; // 跳过行数
                    continue;
                }
            } else if (state == ParserState::HEADER_KEY && buffer[i] == ':') {
                headerkey = temp;
                temp.clear();
                state = ParserState::HEADER_VALUE;
                skipNext = true;
                continue;
                // : 之后会是一个空格
            } else if (state == ParserState::HEADER_VALUE && buffer[i] == '\r') {

                httpMessage->SetHeader(headerkey, temp);
                headerkey.clear();
                temp.clear();
                state = ParserState::HEADER_KEY;
                //跳过下一个字符\n
                skipNext = true;
                continue;
            }
                //判断结束条件
            else if (state == ParserState::HEADER_KEY && buffer[i] == '\r') // 标志已经解析完成
            {
                temp.clear();
                state = ParserState::PARSING_BODY;

                skipNext = true;
                continue;
            }
            temp += buffer[i];
        }
        // 如果有请求的话携带进去
        if (hasMessageBody) {
            httpMessage->SetMessageBody({buffer.begin() + bodyStartIndex, buffer.begin() + readsize});
        }
    }
};

#endif //MYWEB_HTTP_REQUEST_H
