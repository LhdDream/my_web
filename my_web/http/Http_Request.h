//
// Created by kiosk on 19-11-25.
//

#ifndef MYWEB_HTTP_REQUEST_H
#define MYWEB_HTTP_REQUEST_H

#include "Currency.h"

class HTTPMessageParser {
public:
    void Parse(const std::unique_ptr<HTTPMessage> &httpMessage, const std::vector<char> &buffer, int readsize) {
        ParserState state = ParserState::PARSING_START_LINE;
        //解析头一行
        size_t begin = 0;
        size_t end = 0;
        size_t headerkeybegin = 0;
        size_t headerkeyend = 0;
        size_t bodyStartIndex = -1;
        size_t isQuery = -1; // ?

        bool skipNext = false;

        for (auto &&c :buffer) {
            bodyStartIndex++;
            if (skipNext) // 跳过下一行
            {
                skipNext = false;
                continue;
            }
            if (state == ParserState::PARSING_BODY) {
                httpMessage->SetMessageBody({buffer.data() + bodyStartIndex, readsize - bodyStartIndex});
                break;
            }
            if (state == ParserState::PARSING_START_LINE) {
                if (c == ' ') {
                    httpMessage->SetMethond({buffer.data() + begin, end - begin});
                    state = ParserState::START_LINE_REQUEST;
                    //开始解析请求
                    end = end + 1;
                    begin = end;
                    continue;
                }
            } else if (state == ParserState::START_LINE_REQUEST)
                //解析行请求
            {
                if (c == ' ') {
                    if (isQuery == -1) {
                        httpMessage->Setpath({buffer.data() + begin + 1, end - begin - 1});
                    } else {
                        httpMessage->Setpath({buffer.data() + begin + 1, isQuery - begin - 1});
                        httpMessage->SetQuery({buffer.data() + isQuery - begin - 1, end - begin - 1});
                        isQuery = -1;
                    }
                    end = end + 1;
                    begin = end;
                    continue;
                } else if (c == '?') {
                    isQuery = end ;
                } else if (c == '\r') {
                    httpMessage->SetVersion({buffer.data() + begin, end - begin});
                    end = end + 2;
                    begin = end;
                    state = ParserState::HEADER_KEY;
                    skipNext = true; // 跳过行数
                    continue;
                }
            } else if (state == ParserState::HEADER_KEY && c == ':') {
                headerkeybegin = begin;
                headerkeyend = end;
                end = end + 2;
                begin = end;
                state = ParserState::HEADER_VALUE;
                skipNext = true;
                continue;
                // : 之后会是一个空格
            } else if (state == ParserState::HEADER_VALUE && c == '\r') {
                httpMessage->SetHeader({buffer.data() + headerkeybegin, headerkeyend - headerkeybegin},
                                       {buffer.data() + begin, end - begin});
                end = end + 2;
                begin = end;
                state = ParserState::HEADER_KEY;
                //跳过下一个字符\n
                skipNext = true;
                continue;
            }
                //判断结束条件
            else if (state == ParserState::HEADER_KEY && c == '\r') // 标志已经解析完成
            {
                end = end + 2;
                begin = end;
                state = ParserState::PARSING_BODY;
                skipNext = true;
                continue;
            }
            end++;
        }
    }
};

#endif //MYWEB_HTTP_REQUEST_H
