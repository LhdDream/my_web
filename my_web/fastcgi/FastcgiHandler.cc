//
// Created by kiosk on 2020/1/23.
//
#include "FastcgiHandler.h"
#include "../http/Currency.h"
void FastCgiHandler::SendFastCgi(const std::unique_ptr<HTTPMessage> &conn) {

    m_FastCgi_Socket.Connect();


    static const std::string_view param[] = {"SCRIPT_FILENAME", "REQUEST_METHOD", "CONTENT_LENGTH", "CONTENT_TYPE",
                                             "QUERY_STRING"};

    auto BeginRequest = MakeBeginRequest(m_FcgiRole::K_Responder, false);

    auto Header = MakeHeader(K_Begin_Request, 1, sizeof(*BeginRequest), 0);

    //组成头部
    auto bytes = m_buffer.Write(Header.get(), sizeof(*Header));
    //把头部放入
    bytes += m_buffer.Write(BeginRequest.get(), sizeof(*BeginRequest));

    auto flag = m_FastCgi_Socket.Write(m_buffer.BeginRead(), bytes, 0);
    if (flag >= 0)
        m_buffer.ReadOffest_Move(flag);
    bytes = 0;

    auto len = param[0].length() + conn->Getpath().length() + sizeof(m_FcgiData);
    auto body = MakeData(param[0], conn->Getpath());
    Header = MakeHeader(K_Params, 1, len, 0);
    bytes += m_buffer.Write(Header.get(), sizeof(*Header));
    bytes += m_buffer.Write(body.get(), len);

    flag = m_FastCgi_Socket.Write(m_buffer.BeginRead(), bytes, 0);
    if (flag >= 0)
        m_buffer.ReadOffest_Move(flag);
    bytes = 0;

    len = param[1].length() + conn->GETMethod().length() + sizeof(m_FcgiData);
    body = MakeData(param[1], conn->GETMethod());
    Header = MakeHeader(K_Params, 1, len, 0);
    bytes += m_buffer.Write(Header.get(), sizeof(*Header));
    bytes += m_buffer.Write(body.get(), len);
    flag = m_FastCgi_Socket.Write(m_buffer.BeginRead(), bytes, 0);
    if (flag >= 0)
        m_buffer.ReadOffest_Move(flag);
    bytes = 0;

    auto val = std::to_string(conn->GETBody().size());
    len = param[2].length() + val.length() + sizeof(m_FcgiData);
    Header = MakeHeader(K_Params, 1, len, 0);
    body = MakeData(param[2], val);
    bytes += m_buffer.Write(Header.get(), sizeof(*Header));
    bytes += m_buffer.Write(body.get(), len);
    flag = m_FastCgi_Socket.Write(m_buffer.BeginRead(), bytes, 0);
    if (flag >= 0)
        m_buffer.ReadOffest_Move(flag);
    bytes = 0;

    val = conn->GETMethod() == "GET" ? conn->GetQuery() : "application/x-www-form-urlencoded";
    len = param[3].length() + val.length() + sizeof(m_FcgiData);
    Header = MakeHeader(K_Params, 1, len, 0);
    if (conn->GETMethod() == "POST")
        body = MakeData(param[3], val);
    else
        body = MakeData(param[4], val);
    bytes += m_buffer.Write(Header.get(), sizeof(*Header));
    bytes += m_buffer.Write(body.get(), len);
    flag = m_FastCgi_Socket.Write(m_buffer.BeginRead(), bytes, 0);
    if (flag >= 0)
        m_buffer.ReadOffest_Move(flag);
    bytes = 0;

    Header = MakeHeader(K_Params, 0, 0, 1);
    bytes += m_buffer.Write(Header.get(), sizeof(*Header));
    flag = m_FastCgi_Socket.Write(m_buffer.BeginRead(), bytes, 0);
    if (flag >= 0)
        m_buffer.ReadOffest_Move(flag);
    bytes = 0;

    if (!conn->GETBody().empty()) {
        bytes += m_buffer.Write(Header.get(), sizeof(*Header));
        flag = m_FastCgi_Socket.Write(m_buffer.BeginRead(), bytes, 0);
        if (flag >= 0)
            m_buffer.ReadOffest_Move(flag);
    }
}


void FastCgiHandler::ReadFromPHP(Socket &sockfd) {
    m_FacgiHeader responderHeader{};
    int Len{};
    char temp[8]; //用来暂存padding
    char content[655350];
    int ret = 0;
    while (recv(m_FastCgi_Socket.Fd(), &responderHeader, 8, 0) > 0) {
        if (responderHeader.K_Type == K_Stdout) {
            //获取内容长度
            Len = (responderHeader.K_ContentLengthB1 << 8) + (responderHeader.K_ContentLengthB0);
            bzero(content, 655350);
            //读取获取的内容
            ret = recv(m_FastCgi_Socket.Fd(), content, Len, 0);
            assert(ret == Len);

            auto size = std::find(content, content + 65535, '<');

            sockfd.Write(size, ret - (size - content), 0);
            //跳过填充部分
            if (responderHeader.K_PaddingLength > 0) {
                ret = read(m_FastCgi_Socket.Fd(), temp, responderHeader.K_PaddingLength);
                assert(ret == responderHeader.K_PaddingLength);
            }
        } else if (responderHeader.K_Type == K_Stderr) {
            Len = (responderHeader.K_ContentLengthB1 << 8) + (responderHeader.K_ContentLengthB0);
            bzero(content, 655350);

            ret = recv(m_FastCgi_Socket.Fd(), content, Len, 0);
            assert(ret == Len);

            fprintf(stdout, "error:%s\n", content);

            //跳过填充部分

            if (responderHeader.K_PaddingLength > 0) {
                ret = read(m_FastCgi_Socket.Fd(), temp, responderHeader.K_PaddingLength);
                assert(ret == responderHeader.K_PaddingLength);
            }

        } else if (responderHeader.K_Type == K_End_Request) {
            m_FcgiEndRequestBody endRequest{};

            ret = recv(m_FastCgi_Socket.Fd(), &endRequest, sizeof(endRequest), 0);
            assert(ret == sizeof(endRequest));
        }
    }
    close(m_FastCgi_Socket.Fd());
}