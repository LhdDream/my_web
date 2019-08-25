//
// Created by kiosk on 19-8-5.
//
#include "Tcpconnection.h"
#include "channel.h"
#include "httpresponse.h"
#include "httprequest.h"
#include "HttpContext.h"
void Tcpconnection::handleread()
{
        buf.readfd(sockfd_->fd());
        context.parseRequest(&buf);
        channel_->enable_write();
}
void Tcpconnection::connectget()
{
    channel_->enable_read();
}
void Tcpconnection::handlewrite()
{
    channel_->set_events(0);
    loop_->update(channel_);
    HttpResponse resp(context.path(), context.getcgi(), context.query(), context.body());
    resp.getfile(sockfd_->fd());
}