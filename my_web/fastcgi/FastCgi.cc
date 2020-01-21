//
// Created by kiosk on 2020/1/21.
//
#include "Currency.h"

auto MakeHeader(m_FCgiType type,int request_id , int content_length , int padding_length){
    auto re = std::make_unique<m_FacgiHeader>();
    re->K_Version = static_cast<unsigned  char>(m_FcgiVersion);
    re->K_Type = static_cast<unsigned  char> (type);
    re->K_RequestIdB1 = static_cast<unsigned  char>((request_id >> 8) & 0xff);
    re->K_RequestIdB0 = static_cast<unsigned  char>(request_id & 0xff);
    re->K_ContentLengthB1 = static_cast<unsigned  char> ((content_length >> 8) & 0xff );
    re->K_ContentLengthB0 = static_cast<unsigned  char> (content_length & 0xff);
    re->K_PaddingLength = static_cast<unsigned  char> (padding_length);
    re->K_Reserved = 0;
    return re;
}

auto MakeBeginRequest(m_FcgiRole role , bool Keep_Alive) {
    auto re = std::make_unique<m_FcgiBeginRequestBody>();
    re->K_RoleB1 = static_cast<unsigned  char> ((role >> 8) & 0xff) ;
    re->K_RoleB0 = static_cast<unsigned  char> (role & 0xff);
    re->K_Flags = Keep_Alive ? 1 : 0;
    bzero(re->K_Reserved, sizeof re->K_Reserved);
    return re;
}

auto MakeData(const char * name , size_t name_length ,const  char * value , size_t value_length){
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

auto MakeData(const std::string& name , const std::string& value){
    return MakeData(name.data(),name.length(),value.data(),value.length());
}

auto  FcgiGetHeader(const void *src) {
    auto &&head = *(static_cast<const m_FacgiHeader *>(src));
    auto ret = std::make_unique<m_FcgiPre>();
    ret->K_Type = static_cast<m_FCgiType>(head.K_Type);
    ret->K_RequestID = (head.K_RequestIdB1 << 8) + head.K_RequestIdB0;
    ret->K_ContentLength = (head.K_ContentLengthB1 << 8) + head.K_ContentLengthB0;
    ret->K_PaddingLength = head.K_PaddingLength;

    return ret;
}
