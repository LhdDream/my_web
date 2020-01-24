//
// Created by kiosk on 2020/1/19.
//

#ifndef MY_WEB_CURRENCY_H
#define MY_WEB_CURRENCY_H

#include <memory>
#include <cstring>

//
constexpr  uint8_t  m_FcgiVersion = 1;  //fastcgi版本
//fastcgi的状态
enum m_FCgiType : uint8_t {
    K_Begin_Request = 1, // 请求开始
    K_ABORT_Request = 2, //请求结束
    K_End_Request = 3, //请求处理完毕
    K_Params = 4 , //表示环境变量
    K_Stdin = 5 , //表示标准输入
    K_Stdout = 6 ,//表示标准输出
    K_Stderr = 7 ,//表示错误输出
    K_Data = 8 , // 向CGI传递的额外数据
    K_Get_Values = 9 , //询问环境变量
    K_Get_Values_Result = 10 , //询问结果
    K_Unknown_Type = 11
};
enum m_FcgiRole : uint8_t {
    K_Responder = 1 , //相应器
    K_Authorizer = 2 , //认证器
    K_Filter = 3 //过滤器
};
//设置标志位是不是keep-alive
enum m_FcgiFlag : uint8_t {
    K_Keep_alive = 1
};
//fastcgi 使用状态
enum m_FcgiStatus {
    K_Request_Complete = 0,
    K_Cant_Mpx_Conn = 1 , //不支持并发处理
    K_Overloaded = 2 , // fastcgi不支持并发
    K_Unkown_Role = 3 // 请求已经被拒绝
};
//fastcgi 头部组成
struct m_FacgiHeader {
    unsigned char K_Version;
    unsigned char K_Type;
    unsigned char K_RequestIdB1;
    unsigned char K_RequestIdB0;
    unsigned char K_ContentLengthB1;
    unsigned char K_ContentLengthB0;
    unsigned char K_PaddingLength;
    unsigned char K_Reserved;
};
//Fcgi的请求体
struct m_FcgiBeginRequestBody{
    unsigned  char K_RoleB1;
    unsigned  char K_RoleB0;
    unsigned  char K_Flags;
    unsigned  char K_Reserved[0];
};
//Fcgi的请求结束
struct m_FcgiEndRequestBody{
    unsigned char K_AppStatusB3;
    unsigned char K_AppStatusB2;
    unsigned char K_AppStatusB1;
    unsigned char K_AppStatusB0;
    unsigned char K_ProtocolStaus;
    unsigned  char K_Reserved[3];
};

struct m_FcgiData{
    unsigned  char K_NameLengthB3;
    unsigned  char K_NameLengthB2;
    unsigned  char K_NameLengthB1;
    unsigned  char K_NameLengthB0;
    unsigned  char K_ValueLengthB3;
    unsigned  char K_ValueLengthB2;
    unsigned  char K_ValueLengthB1;
    unsigned  char K_ValueLengthB0;
};
struct m_FcgiPre {
    m_FCgiType K_Type;
    int  K_RequestID;
    int K_ContentLength;
    int K_PaddingLength;
};

std::unique_ptr<m_FacgiHeader> MakeHeader(m_FCgiType type,int request_id , int content_length , int padding_length);
std::unique_ptr<m_FcgiBeginRequestBody> MakeBeginRequest(m_FcgiRole role , bool Keep_Alive);
std::unique_ptr<unsigned char []> MakeData(const char * name , size_t name_length ,const  char * value , size_t value_length);
std::unique_ptr<unsigned char []> MakeData(std::string_view  name ,std::string_view value);



#endif //MY_WEB_CURRENCY_H
