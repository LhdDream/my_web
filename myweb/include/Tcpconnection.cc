//
// Created by kiosk on 19-8-5.
//
#include "Tcpconnection.h"
#include "Buffer.h"
#include <sys/sendfile.h>
#include "httpresponse.h"
#include "httprequest.h"
#include "HttpContext.h"
void Tcpconnection::handleread()
{
    if(fd() > 0) {
        Buffer buf;
        buf.readfd(sockfd_);
        HttpContext context;
        context.parseRequest(&buf);
        HttpResponse resp(context.path(),context.getcgi());
        resp.getfile(sockfd_);
    }
}

void Tcpconnection::connectget()
{
    assert(loop_->isthisthread());
    setState_(Connected);
    channel_->enable_read();
    channel_->handleEvent();
}
