#ifndef WEB_CONN_HPP
#define WEB_CONN_HPP
#include <sys/sendfile.h>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include "locker.hpp"

const char *kir = "/home/kiosk/Socket编程那些事/TCP_buffer";
// GET url HTTP/1.0
//User-Agent: Wget/1.12 (linux-gnu)
//Host: www.baidu.com
//Connection: close

//  "POST /[req] HTTP/1.1\r\n"
//  "Connection:Keep-Alive\r\n"
//  "Accept-Encoding:gzip, deflate\r\n"
//  "Accept-Language:zh-CN,en,*\r\n"
//  "Content-Length:[len]\r\n"
//  "Content-Type:application/x-www-form-urlencoded; charset=UTF-8\r\n"
//  "User-Agent:Mozilla/5.0\r\n\r\n"
//  "[data]\r\n\r\n";
class web_conn
{
    public:
        //文件名字的最大长度
        static const int filename_length = 100;
        //读取缓冲区的大小
        static const int read_buffer_size = 4096;
        //写缓冲区的大小
        static const int write_buffer_size = 2048;
        //HTTP请求方式，目前只支持GET,HEAD,OPTIONS,TRACH方法
        //GET方法申请获取资源，不对服务器产生任何的影响
        //HEAD方法仅仅要求服务器返回头部信息，不需要任何实际内容
        //OPTIONS方法查看服务器对于某个特定的url都支持那些请求方法
        //TRACH方法要求目标服务器返回原始的HTTP请求的内容
        enum METHOD { GET = 0,HEAD = 1,OPTIONS = 2,TRACE = 3,POST,PUT,DELETE,CONNECT,PATCH};
        //需要解析web客户端的请求，即http请求
        //行，头部字段，连接方式
        enum CHECK_STATA { CHECK_STATE_REQUESTLINE = 0,CHECK_STATE_HEADER,CHECK_STATE_CONTENT,POST_FILE};
        //服务器的处理结果
        enum HTTP_CODE { NO_REQUEST,GET_REQUEST,BAD_REQUEST,NO_RESOURCE,FORBIDDEN_REQUEST,FILE_REQUEST,INTERNAL_ERROR,CLOSED_CONNECTION,POST_REQUEST};

        //行读取状态
        //读取失败，完成，正在读取
        enum LINE_STATUS {LINE_OK = 0,LINE_BAD,LINE_OPEN };

      public:
        //所有socket上的事件都被注册到同一个epoll内核事件表中,epoll文件描述符设置为静态的
        int epollfd;
        //统计用户数量
        int user_number;
        //状态
        CHECK_STATA m_check_state;
        //客户请求的目标文件
        char file_address[100];
        //
        string body_first;
        //
        string body_second;
        //
        string body_third;
      private:
        //响应的头部内容
        char request_header_buf[1000];
        //该HTTP连接的socket和对方的地址信息
        int sockfd;
        sockaddr_in address;
        //TCP连接的读缓冲区
        char m_read_buf[read_buffer_size];
        //分析的字节在读缓冲区的位置
        int check_buf;
        //已经读取的客户内容的位置
        int read_buf;
        //正在分析的行的位置
        int start_line;
        //写缓冲区
        char m_write_buf[write_buffer_size];
        //待发送的字节数
        int write_buf;

        char header_buf[1000];
        //head方法buf
        char options[1000];
        //options
        char trace[1000];
        //trace
        //请求方法
        METHOD method;

        //客户请求的目标文件
        const char *url;
        //HTTP的协议号，在http请求和应答中都需要
        const char *version;
        //连接的时候需要主机名
        char *host;
        //请求体的消息长度
        int m_content_length;
        //HTTP的tcp连接是不是要求保持连接
        bool m_linger;
        
        // string body;
        //目标文件的状态获取
        struct stat file_stat;
        char post_address[2000];
        //
        int file_size;
      public:
        web_conn(){};
        ~web_conn(){};

      public:
        //初始化接收新的连接//web接收新的连接,sockaddr_in套接字
        void web_init(int m_sockfd,  sockaddr_in &m_addr,int p_epollfd);
        //关闭连接
        void close_conn();
        //写函数，非阻塞式
        bool write_file();
        //读函数,非阻塞是
        bool read_file();
        //处理客户请求
        void client_process();
        //
        void doit();
        //
        void head_process(); //header的填充函数
        //
        void options_process(); //options的填充函数
        //
        void trace_process(); //trace的填充函数
        //
        bool post_respond();//post的方法响应函数
        //
        HTTP_CODE do_header(); //对于head的请求参数进行解析

        HTTP_CODE do_options(); //对于options请求行参数进行解析

        HTTP_CODE do_trace(); //对于trace请求行参数进行解析
      private:
        //初始化连接
        void init();
        //解析HTTP连接请求
        HTTP_CODE read_process();
        //填充HTTP的应答
        bool process_write(HTTP_CODE ret);
        //返回处理器处理的结果
        //下面这一组函数被process_read调用分析HTTP请求
        HTTP_CODE parse_request_line(char *temp);
        HTTP_CODE read_headers(char *temp);
        HTTP_CODE do_post(char *temp);
        HTTP_CODE do_requetst();
        HTTP_CODE parse_content(char * temp);
        char* get_line()
        {
            //返回已经读取的位置
            return m_read_buf + start_line;
        }
            LINE_STATUS read_line();
            //HTTP应答方式
            // HTTP/1.0 200 OK
            // Server: BWS/1.0
            // Content-Length: 8024
            // Content-Type: text/html;charest = gbk
            // Set-Cookie: ........................
            // via: 1.0 localhost (squid/3.0 STABLE18)
            //下面这一组函数被process_write调用填充HTTP应答
            bool bad_respond();         //语法错误请求响应填充
            bool forbiden_respond();    //资源权限限制请求响应的填充
            bool succeessful_respond(); //解析成功请求响应填充
            bool not_found_request();   //资源不存在请求响应填充
};

void web_conn::init()
{
    m_check_state = CHECK_STATE_REQUESTLINE;//主状态机的状态,表示先解析请求行，再解析头部，最后解析conncetion连接方法
    m_linger = false; // tcp和服务器之间保持长连接状态

    method = GET;
    user_number = 0;
    epollfd = -1;
    sockfd = -1;
    url = NULL;
    version = NULL;
    m_content_length = 0 ;
    host = NULL;
    start_line = 0;
    check_buf = read_buf = write_buf = 0;
    memset(m_read_buf,'\0',read_buffer_size);
    memset(m_write_buf,'\0',write_buffer_size);
    memset(header_buf,'\0',sizeof(header_buf));
    memset(options,'\0',sizeof(options));
    memset(trace,'\0',sizeof(trace));
} 

void web_conn::close_conn()
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, 0);
    close(sockfd);
    sockfd = -1;
    user_number--;//连接的客户端-1
}
web_conn::HTTP_CODE web_conn::parse_content(char * temp)
{
    cout << "parse_content :          " << endl;
    if(read_buf >= (m_content_length + check_buf))
    {
        temp[m_content_length] = '\0';
        return GET_REQUEST;
    }
    return NO_REQUEST;
}
bool web_conn::read_file()
{
    while (1)
    {
        int ret = recv(sockfd,read_buf + m_read_buf,read_buffer_size -read_buf,0);
        // cout << " ssssssssssssss  "<< m_read_buf << endl;
        // cout << "ret   " << ret << endl;
        if( ret == -1)
        {
            if( errno == EAGAIN || errno == EWOULDBLOCK)//读取结束
            {
                break;
            }
            return false;
        }
        else if(ret == 0)
        {
            return false;
        }
        read_buf += ret ;//一共读取了多少字节数
    }
    return true;//读取彻底结束
}
bool web_conn::post_respond()
{
    //生成一个进程，把标准输出重定向给客户端输出内容
    if(fork() == 0)
    {
        dup2(sockfd, STDOUT_FILENO);
        char re_head[1000];
        char message[1000];
        int a = atoi(body_second.c_str());
        int b = atoi(body_third.c_str());
        int result;
        result = a + b;
        /*返回正确信息*/
        sprintf(message, "<html><body>\r\n");
        sprintf(message, "%s<p>%d + %d = %d</p><br>\r\n", message, a, b, result);
        sprintf(message, "%s</body></html>\r\n", message);

        sprintf(re_head, "HTTP/1.1 200 ok\r\n");
        sprintf(re_head, "%sContent-length: %d\r\n", re_head, (int)strlen(message));
        sprintf(re_head, "%scontent-type: text/html\r\n\r\n", re_head);
       
        printf("%s",re_head);
        printf("%s",message);
    }
    // wait(NULL);
    return true;
}
bool web_conn::write_file()
{
    sprintf(file_address,"%s%s",kir,url);
    cout << "read    ggggggggggggggggg"
       << file_address << endl;
    
    int fd = open(file_address, O_RDONLY);
    assert(fd != -1);
    int ret;
    ret = write(sockfd, request_header_buf, strlen(request_header_buf));
    cout << "quest.........................   " << request_header_buf << endl;
    if (ret < 0)
    {
        close(fd);
        return false;
    }
    ret = sendfile(sockfd, fd, NULL, file_size);
    if (ret < 0)
    {
        close(fd);
        return false;
    }
    close(fd);
    return true;
}

#endif //WEBCONN_HPP
