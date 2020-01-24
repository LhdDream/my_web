//
// Created by kiosk on 2020/1/24.
//
#include "Currency.h"
std::unique_ptr<m_FacgiHeader> MakeHeader(m_FCgiType type,int request_id , int content_length , int padding_length){
    auto re = std::make_unique<m_FacgiHeader>();
    re->K_Version = static_cast<unsigned  char>(m_FcgiVersion);
    re->K_Type = static_cast<unsigned  char> (type);
    re->K_RequestIdB1 = static_cast<unsigned  char>((request_id >> 8) & 0xff);
    re->K_RequestIdB0 = static_cast<unsigned  char>(request_id & 0xff);
    re->K_ContentLengthB1 = static_cast<unsigned  char> ((content_length >> 8) & 0xff );
    re->K_ContentLengthB0 = static_cast<unsigned  char> (content_length & 0xff);
    re->K_PaddingLength = static_cast<unsigned  char> (padding_length);
    re->K_Reserved = 0;
    return   re;
}

 std::unique_ptr<m_FcgiBeginRequestBody> MakeBeginRequest(m_FcgiRole role , bool Keep_Alive) {
    auto re = std::make_unique<m_FcgiBeginRequestBody>();
    re->K_RoleB1 = static_cast<unsigned  char> ((role >> 8) & 0xff) ;
    re->K_RoleB0 = static_cast<unsigned  char> (role & 0xff);
    re->K_Flags = Keep_Alive ? 1 : 0;
    bzero(re->K_Reserved, sizeof re->K_Reserved);
    return re;
}

std::unique_ptr<unsigned char []> MakeData(const char * name , size_t name_length ,const  char * value , size_t value_length){
    auto re = std::make_unique<unsigned  char []>(sizeof(m_FcgiData) + name_length + value_length);
    //最大只能这样大
    auto body = reinterpret_cast<m_FcgiData *>(re.get());

    body->K_NameLengthB3 = static_cast<unsigned  char >((name_length >> 24) | 0x80);
    body->K_NameLengthB2 = static_cast<unsigned  char>(name_length >> 16);
    body->K_NameLengthB1 = static_cast<unsigned  char> (name_length >> 8);
    body->K_NameLengthB0 = static_cast<unsigned  char> (name_length);

    body->K_ValueLengthB3 = static_cast<unsigned  char> ((value_length >> 24)| 0x80);
    body->K_ValueLengthB2 = static_cast<unsigned  char> (value_length >> 16);
    body->K_ValueLengthB1 = static_cast<unsigned  char> (value_length >> 8);
    body->K_ValueLengthB0 = static_cast<unsigned  char> (value_length);

    std::memcpy(re.get() + sizeof (m_FcgiData) , name, name_length);
    std::memcpy(re.get() + sizeof(m_FcgiData) + name_length , value ,value_length);
    return re;
}

 std::unique_ptr<unsigned char []> MakeData(std::string_view  name ,std::string_view value){
    return MakeData(name.data(),name.length(),value.data(),value.length());
}

