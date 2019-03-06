#include "web_conn.hpp"
#include <string>
#include <algorithm>
using namespace std;
//定义http的状态码
// const char *ok_20_title = "OK";
// const char* error_301_title = "Moved Permanently";
// const char* error_304_title = "NOT Modified";
// const char* error_500_title = "Internal Server Error";
// const char* error_400_form = "Your request has bad syntax or is inherently\n";
// const char* error_500_form = "There was an unusual problem serving the requested file.\n";
//网站的根目录，一般Linux的web.http都在/var/www/html中
//在这里设置为本地目录
const char *dir = "/home/kiosk/Socket编程那些事/TCP_buffer";

int setnoblock(int fd)
{
    int old_option = fcntl(fd, F_GETFL);      //得到文件状态
    int new_option = old_option | O_NONBLOCK; //设置为非阻塞
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addshot(int m_epollfd, int fd, bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP; //EPOLLRDHUP表示对端关闭措施
    if (one_shot)
    {
        event.events |= EPOLLONESHOT; //仅仅只触发一次，调用epolloneshot事件
    }
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event);
    setnoblock(fd);
}

void modfd(int m_epollfd, int fd, int ev) //改变事件的属性//ev指的是解析请求行之后的结果,改变的属性,epoolIN什么的
{
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &event);
}

void web_conn::web_init(int m_sockfd, sockaddr_in &m_addr, int p_epollfd)
{
    //在这里把客户端的信息和套接字修改为非阻塞方式
    init();
    sockfd = m_sockfd;
    address = m_addr;
    epollfd = p_epollfd;
    addshot(epollfd, sockfd, true);
    user_number++;
}
web_conn::LINE_STATUS web_conn::read_line()
{
    char test;
    //  printf("%s\n       ggggggg",m_read_buf);
    // body += m_read_buf;
    for(;check_buf < read_buf ; ++check_buf)
    {
        test = m_read_buf[check_buf];
        // cout << "kkkkkkkkkkkkkkk     " << test << "    ggggggggg" << endl;
        if (test == '\r')
        {
            if ((check_buf + 1) == read_buf)
            {
                return LINE_OPEN;
            }
            else if (m_read_buf[check_buf + 1] == '\n')
            { 
                 m_read_buf[check_buf++] = '\0';
                 m_read_buf[check_buf++] = '\0';
                 return LINE_OK;
            }
            return LINE_BAD;
        }
        else if (test == '\n')
        {
            if ((check_buf > 1) && (m_read_buf[check_buf - 1] == '\r'))
            {
                m_read_buf[check_buf - 1] = '\0';
                m_read_buf[check_buf++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        // cout << "kkkkkkkkkkkkkkk     " << test << "    ggggggggg" << endl;
    }
    printf("\n");
    // cout << "             sssssssssssssgggggggggggsssssssssss " << endl;
    return LINE_OPEN;
}
//解析HTTP请求行
web_conn::HTTP_CODE web_conn::parse_request_line(char *temp)
{
    string sa = temp;
    // cout << "sa     " << sa << "    fffffffffffff" << endl;
    int size = sa.find('\r');
    string sb = sa.substr(0, size);
    size = sb.find(' ');

    string sc = sb.substr(0, size);
    if (sc == "GET")
    {
        method = GET;
    }
    else if (sc == "POST")
    {
        method = POST;
    }
    else if (sc != "GET" && sc != "POST")
    {
        return BAD_REQUEST;
    }
    string sd = sb.substr(size + 1);
    size = sd.find(' ');
    url = sd.substr(0, size).c_str();
    

    //如果提交的话会出现/add?a=6&b=3这种形式
    version = sd.substr(size + 1).c_str();
    if (strcmp(version, "HTTP/1.1") != 0)
    {
        return BAD_REQUEST;
    }
    if(method == GET)
        m_check_state = CHECK_STATE_HEADER;
    else if(method == POST)
        m_check_state = POST_FILE;
    if(method == GET)
        return NO_REQUEST;

}
//解析POST请求中的信息
web_conn::HTTP_CODE web_conn::do_post(char * temp)
{
    // printf("  %s\n", temp);
    if(temp[0] == '\0')
    {
        if(m_content_length != 0)
        {
            m_check_state = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }
    char *substr = "Content-Length";
    char * s = strstr(temp,substr);
    string content_length;
    char test;
    if(s != NULL)
    {
        for(int i = 0;i<strlen(s);i++)
        {
            test = s[i];
            if(test >='0' && test <='9')
            {
                content_length += test;
                cout << content_length << endl;
            }
        }
    }
    string data = body_first.substr(body_first.size()-atoi(content_length.c_str()));
    string data_test = data.substr(0,atoi(content_length.c_str()));
    int size = data_test.find('&');
    // cout  << "size "<< size << endl;
    cout << data_test << endl;
    if(size > 0 )
    {
        body_second = data_test.substr(2,size-2);
        body_third = data_test.substr(size+3);
        cout << "body_second" <<  body_second << endl;
    }
    return NO_REQUEST;
}
//解析HTTp请求的头部信息
web_conn::HTTP_CODE web_conn::read_headers(char *temp)
{
    // cout << "t l      " << *temp << " ff  g j h " << endl;
    //遇到空行说明解析结束
    // string test_temp =  temp;
    // cout << "ggg jj " << test_temp << " hh kkk "<<endl;
    if (temp[0] == '\0')
    {
        if (m_content_length != 0)
        {
            m_check_state = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }
    else if (strncasecmp(temp, "Connection:", 11) == 0)
    {
        temp += 11;
        temp += strspn(temp, " \t");
        if (strcasecmp(temp, "keep-alive") == 0)
        {
            m_linger = true; //持续连接
        }
    }
    else if (strncasecmp(temp, "Content-Length", 15) == 0)
    {
        temp += 15;
        temp += strspn(temp, " \t");
        m_content_length = atoi(temp);
    }
    //处理Host字段
    else if (strncasecmp(temp, "Host:", 5) == 0)
    {
        temp += 5;
        temp += strspn(temp, " \t");
        host = temp;
    }
    return NO_REQUEST;
}
//主状态机函数
web_conn::HTTP_CODE web_conn::read_process()
{
    HTTP_CODE ret = NO_REQUEST;
    LINE_STATUS line_status = LINE_OK; //读取完成一行内容
    char *temp = NULL;
    body_first = m_read_buf;
    // cout << body_first << endl;
    while (((line_status = read_line()) == LINE_OK)||((m_check_state == CHECK_STATE_CONTENT) && (line_status == LINE_OK)))
    {
        temp = get_line();
        start_line = check_buf;
        // printf("ffffffff%s\n",temp);
        switch (m_check_state)
        {
            case CHECK_STATE_REQUESTLINE:
          {
            // cout << "1111111111111111111" << endl;
            ret = parse_request_line(temp);
            if (ret == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            break;
          }
          case POST_FILE:
          {
              ret = do_post(temp);
              if(ret == BAD_REQUEST)
              {
                  return BAD_REQUEST;
              }
              else if(ret == GET_REQUEST)
              {
                  return POST_REQUEST;
              }
              break;
          }
          case CHECK_STATE_HEADER:
          {
                // cout << "222222222222222" << endl;
            ret = read_headers(temp);
            if (ret == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if (ret == GET_REQUEST)
            {
                return do_requetst();
            }
            break;
          }
          case CHECK_STATE_CONTENT:
          {
              ret = parse_content(temp);
              if(ret == GET_REQUEST)
              {
                  return do_post(temp);
              }
              line_status = LINE_OPEN;
              break;
          }
           default:
          {
              return INTERNAL_ERROR;
          }
        }
    }
    return NO_REQUEST;
}
//分析文件的属性
web_conn::HTTP_CODE web_conn::do_requetst()
{
    strcpy(web_conn::file_address, dir);
    strcat(web_conn::file_address, url);
    if (stat(web_conn::file_address, &file_stat) < 0)
    {
        cout << "no permionss" << endl;
    }

    if (!(web_conn::file_stat.st_mode & S_IROTH))
    {
        return web_conn::FORBIDDEN_REQUEST;
    }

    if (S_ISDIR(web_conn::file_stat.st_mode))
    {
        return BAD_REQUEST; //表示错误请求404请求地方是一个目录
    }

    file_size = web_conn::file_stat.st_size;
    return FILE_REQUEST;
}

/*响应状态的填充，这里返回可以不为bool类型*/
bool web_conn::succeessful_respond() //200
{
    bzero(web_conn::request_header_buf, sizeof(web_conn::request_header_buf));
    sprintf(web_conn::request_header_buf, "HTTP/1.1 200 ok\r\nConnection: close\r\ncontent-length:%d\r\n\r\n", file_size);
}
bool web_conn::bad_respond() //400
{
    char hg[100];
    strcpy(hg, "400.html");
    bzero(file_address, sizeof(file_address));
    sprintf(file_address, "%s/%s", dir, hg);
    struct stat my_file;
    if (stat(file_address, &my_file) < 0)
    {
        cout << "22222222222222222  " << endl;
        cout << "文件不存在\n";
    }
    file_size = my_file.st_size;
    bzero(web_conn::request_header_buf, sizeof(web_conn::request_header_buf));
    sprintf(web_conn::request_header_buf, "HTTP/1.1 400 BAD_REQUESTION\r\nConnection: close\r\ncontent-length:%d\r\n\r\n", file_size);
}
bool web_conn::forbiden_respond() //403
{
    char ug[100];
    strcpy(ug, "403.html");
    bzero(file_address, sizeof(file_address));
    sprintf(file_address, "%s/%s", dir, ug);
    struct stat my_file;
    if (stat(file_address, &my_file) < 0)
    {
        cout << "3333  " << endl;
        cout << "文件不存在" << endl;
    }
    file_size = my_file.st_size;
    bzero(web_conn::request_header_buf, sizeof(web_conn::request_header_buf));
    sprintf(web_conn::request_header_buf, "HTTP/1.1 403 FORBIDDEN\r\nConnection: close\r\ncontent-length:%d\r\n\r\n", file_size);
    int fd = open(file_address, O_RDONLY);
    assert(fd != -1);
    int ret;
    ret = write(sockfd, request_header_buf, strlen(request_header_buf));
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
bool web_conn::not_found_request() //404
{
    char gk[100];
    strcpy(gk, "404.html");
    bzero(file_address, sizeof(file_address));
    sprintf(file_address, "%s/%s", dir, gk);
    struct stat my_file;
    if (stat(file_address, &my_file) < 0)
    {
        cout << "不存在\n"
             << endl;
    }
    file_size = my_file.st_size;
    bzero(web_conn::request_header_buf, sizeof(web_conn::request_header_buf));
    sprintf(web_conn::request_header_buf, "HTTP/1.1 404 NOT_FOUND\r\nConnection: close\r\ncontent-length:%d\r\n\r\n", file_size);
}

bool web_conn::process_write(HTTP_CODE ret)
{
    switch (ret)
    {
    case web_conn::BAD_REQUEST: //400
    {
        web_conn::bad_respond();
        modfd(epollfd, sockfd, EPOLLOUT);
        break;
    }
    case web_conn::NO_REQUEST: //404
    {
        web_conn::not_found_request();
        modfd(epollfd, sockfd, EPOLLOUT);
        break;
    }
    case web_conn::FORBIDDEN_REQUEST:
    {
        web_conn::forbiden_respond();
        modfd(epollfd, sockfd, EPOLLOUT);
        break;
    }
    case web_conn::FILE_REQUEST:
    {
        web_conn::succeessful_respond();
        web_conn::write_file();
        break;
    }
    case web_conn::POST_REQUEST:
    {
        web_conn::succeessful_respond();
        web_conn::post_respond();
        break;
    }
    default:
    {
        web_conn::close_conn();
        break;
    }
    }
    modfd(epollfd, sockfd, EPOLLOUT);
}
void web_conn::doit()
{
    web_conn::HTTP_CODE read_ret = read_process();
    if (read_ret == NO_REQUEST)
    {
        modfd(epollfd, sockfd, EPOLLIN);
        return;
    }
    bool write_ret = process_write(read_ret);
    if (!write_ret)
    {
        close_conn();
    }
    modfd(epollfd, sockfd, EPOLLOUT);
}