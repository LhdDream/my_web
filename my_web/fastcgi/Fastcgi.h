//
// Created by kiosk on 2020/1/26.
//

#ifndef MY_WEB_FASTCGI_H
#define MY_WEB_FASTCGI_H

#include <memory>

constexpr uint8_t FcgiVersion = 1;
constexpr int Fcgi_KeepConnection = 1;

struct FcgiHeader {
    unsigned char version;
    unsigned char type;
    unsigned char requestIdB1;
    unsigned char requestIdB0;
    unsigned char contentLengthB1;
    unsigned char contentLengthB0;
    unsigned char paddingLength;
    unsigned char reserved;
};

enum FcgiType {
    k_BeginRequest = 1,
    k_AbortRequest = 2,
    k_EndRequest = 3,
    k_Params = 4,
    k_Stdin = 5,
    k_Stdout = 6,
    k_Stderr = 7,
    k_Data = 8,
    k_GetValues = 9,
    k_GetValuesResult = 10,
    k_Unknown = 11,
};
struct FcgiPreread {
    FcgiType type;
    int requestId;
    int contentLength;
    int paddingLength;
};

enum FcgiProtocolStatus : uint8_t {
    k_RequestComplete = 0,
    k_CantMultiplexConnection = 1,
    k_Overloaded = 2,
    k_UnknownRole = 3,
};



enum FcgiRole : uint16_t {
    k_Responder = 1,
    k_Authorizer = 2,
    k_Filter = 3,
};



struct FcgiBeginRequestBody {
    unsigned char roleB1;
    unsigned char roleB0;
    unsigned char flags;
    unsigned char reserved[5];
};

struct FcgiEndRequestBody {
    unsigned char appStatusB3;
    unsigned char appStatusB2;
    unsigned char appStatusB1;
    unsigned char appStatusB0;
    unsigned char protocolStatus;
    unsigned char reserved[3];
};

struct FcgiParamsBody {
    unsigned char nameLengthB3;
    unsigned char nameLengthB2;
    unsigned char nameLengthB1;
    unsigned char nameLengthB0;
    unsigned char valueLengthB3;
    unsigned char valueLengthB2;
    unsigned char valueLengthB1;
    unsigned char valueLengthB0;
};


#endif //MY_WEB_FASTCGI_H
