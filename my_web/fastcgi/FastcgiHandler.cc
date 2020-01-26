//
// Created by kiosk on 2020/1/23.
//
#include "FastcgiHandler.h"
#include "../http/Currency.h"

enum state{
    S_NOPE,
    S_CR,
    S_CRLF,
    S_CRLFCR,
    S_CRLFCRLF
};

template<typename T>
unsigned char Type(T n) {
    return static_cast<unsigned char>(n);
}


void FastCgiHandler::SendFastCgi(const std::unique_ptr<HTTPMessage> &conn) {

    m_FastCgi_Socket.CreateFd(Provider::Get().FastCgiIP().c_str(), Provider::Get().FastCgiPort());
    m_FastCgi_Socket.Connect();
    static const std::string params[] = {"SCRIPT_FILENAME", "REQUEST_METHOD", "CONTENT_LENGTH", "CONTENT_TYPE"};
   SendBeginRequestBody();
    //body
    auto val = Provider::Get().ServerWwwRoot() + "/" + conn->Getpath();
    auto length =  params[0].length() + val.length() + sizeof(FcgiParamsBody);
    auto body = MakeParam(params[0],val);
    auto  head = MakeHeader(FcgiType::k_Params,1,length,0);
    auto bytes = m_buffer.Write(head.get(), sizeof(*head));
    bytes += m_buffer.Write(body.get(),length);
    m_FastCgi_Socket.Write(m_buffer.BeginRead(),bytes);
    m_buffer.ReadOffest_Move(bytes);
    bytes = 0;

    val = conn->GETMethod();
    length = params[1].length() + val.length() + sizeof(FcgiParamsBody);
    body = MakeParam(params[1],val);
    head = MakeHeader(FcgiType::k_Params,1,length,0);
    bytes +=  m_buffer.Write(head.get(), sizeof(*head));
    bytes += m_buffer.Write(body.get(),length);
    m_FastCgi_Socket.Write(m_buffer.BeginRead(),bytes);
    m_buffer.ReadOffest_Move(bytes);
    bytes = 0;

   val = conn->GETBody().empty() ? "0" : std::to_string(conn->GETBody().size());
    length = params[2].length() + val.length() + sizeof(FcgiParamsBody);
    body = MakeParam(params[2],val);
    head = MakeHeader(FcgiType::k_Params,1,length,0);
    bytes +=  m_buffer.Write(head.get(), sizeof(*head));
    bytes += m_buffer.Write(body.get(),length);
    m_FastCgi_Socket.Write(m_buffer.BeginRead(),bytes);
    m_buffer.ReadOffest_Move(bytes);
    bytes = 0;

    val = conn->GETMethod() == "GET" ? "text/html" :  "application/x-www-form-urlencoded";
    length = params[3].length() + val.length() + sizeof(FcgiParamsBody);
    body = MakeParam(params[3],val);
    head = MakeHeader(FcgiType::k_Params,1,length,0);
    bytes +=  m_buffer.Write(head.get(), sizeof(*head));
    bytes += m_buffer.Write(body.get(),length);
    m_FastCgi_Socket.Write(m_buffer.BeginRead(),bytes);
    m_buffer.ReadOffest_Move(bytes);
    bytes = 0;

    SendEndRequestRecord();
    if(!conn->GETBody().empty()){
        bytes += m_buffer.Write(conn->GETBody().data(),conn->GETBody().size());
        m_FastCgi_Socket.Write(m_buffer.BeginRead(),bytes);
        m_buffer.ReadOffest_Move(bytes);
    }
}


char * FastCgiHandler::ReadFromPHP(int & size) {

    char * cont = nullptr;
    int contentLen = 0 ,ret =  0;
    FcgiHeader responderHeader{};
    auto content = std::make_unique<char [] >(65535);

    char tmp[8];    //用来暂存padding字节的
    //先将头部8个字节度出来
    while(recv(m_FastCgi_Socket.Fd(),&responderHeader,8,0) > 0)
    {
        if(responderHeader.type == FcgiType::k_Stdout)
        {
            //获取内容长度
            contentLen = (responderHeader.contentLengthB1 << 8) + (responderHeader.contentLengthB0);
            bzero(content.get(),65535);
            //读取获取的内容
            ret = recv(m_FastCgi_Socket.Fd(),content.get(),contentLen,0);
            assert(ret == contentLen);
            cont = GetHtmlFromContent(content.get() );
            size = ret - (cont- content.get());
            if(responderHeader.paddingLength > 0)
            {
                ret = recv(m_FastCgi_Socket.Fd(),tmp,responderHeader.paddingLength,0);
                assert(ret == responderHeader.paddingLength);
            }
        }
        else if(responderHeader.type == FcgiType::k_Stderr)
        {
            contentLen = (responderHeader.contentLengthB1 << 8) + (responderHeader.contentLengthB0);
            bzero(content.get(),65535);

            ret = recv(m_FastCgi_Socket.Fd(),content.get(),contentLen,0);
            assert(ret == contentLen);

            //跳过填充部分

            if(responderHeader.paddingLength > 0)
            {
                ret = recv(m_FastCgi_Socket.Fd(),content.get(),contentLen,0);
                assert(ret == responderHeader.paddingLength);
            }

        }
        else if(responderHeader.type == FcgiType::k_EndRequest)
        {
            FcgiEndRequestBody endRequest{};

            ret = recv(m_FastCgi_Socket.Fd(),content.get(),contentLen,0);
            assert(ret == sizeof(endRequest));
        }
    }

    close(m_FastCgi_Socket.Fd());
    m_buffer.Reset();
    return cont;
}



std::unique_ptr<FcgiHeader> FastCgiHandler::MakeHeader(int type, int requestId, int contentLength, int paddingLength) {

    auto header = std::make_unique<FcgiHeader>();

    header->version = '1';

    header->type = Type(type);

    header->requestIdB1 = Type((requestId >> 8) & 0xff);  //用连个字段保存请求ID
    header->requestIdB0 = Type(requestId & 0xff);

    header->contentLengthB1 = Type((contentLength >> 8) & 0xff);//用俩个字段保存内容长度
    header->contentLengthB0 = Type(contentLength & 0xff);

    header->paddingLength = Type(paddingLength);        //填充字节的长度

    header->reserved = 0;    //保留字节赋为0

    return header;
}

std::unique_ptr<FcgiBeginRequestBody> FastCgiHandler::MakeBeginRequestBody(int role, bool keepConnection) {
    auto body = std::make_unique<FcgiBeginRequestBody>();
    body->roleB1 = Type((role >> 8) & 0xff);//俩个字节保存我们期望php-fpm扮演的角色
    body->roleB0 = Type(role & 0xff);
    body->flags = Type(keepConnection ? 1 : 0);//大于0常连接，否则短连接
    bzero(&body->reserved, sizeof(body->reserved));
    return body;
}

bool FastCgiHandler::SendEndRequestRecord() {
    auto Header= MakeHeader(FcgiType:: k_Params,1,0,0);

    m_buffer.Write(Header.get(), sizeof(*Header));
    int it = m_FastCgi_Socket.Write(m_buffer.BeginRead(), sizeof(*Header));
    m_buffer.ReadOffest_Move(it);
    return true;
}

void FastCgiHandler::SendBeginRequestBody() {
    auto Begin = MakeBeginRequestBody(FcgiRole::k_Responder,false);
    auto header = MakeHeader(FcgiType::k_BeginRequest,1,sizeof(*Begin),0);
    auto it = m_buffer.Write(header.get(), sizeof(*header));
    it += m_buffer.Write(Begin.get(), sizeof(*Begin));
    auto flag = m_FastCgi_Socket.Write(m_buffer.BeginRead(), it);
    m_buffer.ReadOffest_Move(flag);
}

char * FastCgiHandler::GetHtmlFromContent( char * content ) {
      if(auto pt = FindHtmlStart(content) ; pt != nullptr) {
          return pt;
      }
      return nullptr;
}

char* FastCgiHandler::FindHtmlStart(char *content) {
    state begin = S_NOPE;
    for(char * p = content; *p != '\0'; p++){
        switch (begin){
            case S_NOPE: {
                if (*p == '\r')
                    begin = S_CR;
                break;
            }
            case S_CR:{
                begin = (*p == '\n') ? S_CRLF :S_NOPE;
                break;
            }
            case S_CRLF:{
                begin = (*p == '\r') ? S_CRLFCR : S_NOPE;
                break;
            }
            case S_CRLFCR :{
                begin = (*p == '\n') ? S_CRLFCRLF : S_NOPE;
                break;
            }
            case S_CRLFCRLF:
                return p;
            default:
                return content;
        }
    }
    return content;
}

std::unique_ptr<unsigned  char []> FastCgiHandler::MakeParam(int name_size, int value_size ,const char *name, const char *value) {
    auto ret = std::make_unique<unsigned char[]>(sizeof(FcgiParamsBody) + name_size + value_size);
    auto base = reinterpret_cast<FcgiParamsBody *>(ret.get());

    base->nameLengthB3 = Type((name_size >> 24) | 0x80);
    base->nameLengthB2 = Type(name_size>> 16);
    base->nameLengthB1 = Type(name_size >> 8);
    base->nameLengthB0 = Type(name_size);
    base->valueLengthB3 = Type((value_size >> 24) | 0x80);
    base->valueLengthB2 = Type(value_size >> 16);
    base->valueLengthB1 = Type(value_size  >> 8);
    base->valueLengthB0 = Type(value_size );

    std::memcpy(ret.get() + sizeof(FcgiParamsBody), name, name_size);
    std::memcpy(ret.get() + sizeof(FcgiParamsBody) + name_size, value, value_size );
    return ret;
}
std::unique_ptr<unsigned  char []> FastCgiHandler::MakeParam(std::string name, std::string value) {
    return MakeParam(name.length(),value.size(),name.c_str(),value.c_str());
}