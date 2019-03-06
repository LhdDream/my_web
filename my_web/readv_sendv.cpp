//Web服务器上的读和写
//解析完一个httpo请求后应该保存一个状态行，多个头部文字，1个空行，文档中的内容
//http应答的发送，直接将目标文件作为第三个参数传递给服务器程序，客户telent到服务器上就可以获得该文件
#include<sys/socket.h>
#include<netinet/in.h>
#include<assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <memory.h>
#define BUFFER_SIZE 1024

//定义两种 Http 状态码和状态信息
static const char * status_line[2]={"200 OK","500 Internal server error"};

int main(int argc,char** agrv)
{
    if(argc <= 3)
    {
        printf("error argc\n");
        return 1;
    }
    const char * ip = argv[1];
    int port = atoi(argv[2]);
    //目标文件作为程序的第三个参数传入
    const char *file_name = argv[3];

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock >= 0);

    int ret = bind(sock,(struct sockaddr *)&address,sizeof(address));
    assert(ret != -1);

    ret = listen(sock,5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_length = sizeof(client);
    int connfd = accept(sock,(struct sockaddr *)&client,&client_length);
    if(connfd < 0)
    {
        printf("errno \n");
        return 1;
    }
    else
    {
        //用于保存HTTP应答的状态行,头部字段，和一个空行的缓存区
        char header_buf[BUFFER_SIZE];
        memset(header_buf,'\0',BUFFER_SIZE);
        //用于存放目标文件内容的应用程序缓存
        char* file_buf;
        //获取目标文件的属性，比如是不是为目录，文件的大小等等
        struct stat file_stat;
        //记录目标文件是不是有效文件
        bool valid = true;
        //缓冲区head_buf已经用过多少个字节
        int len = 0 ;
        if(stat(file_name,&file_stat) < 0)//目标文件并不存在
        {
            valid = false;
        }
        else
        {
            if(S_ISDIR(file_stat.st_mode)) //目标文件是一个目录的话
            {
                valid = false;
            }
            else if(file_stat.st_mode & S_IROTH) //当前用户有读取目标文件的权限
            {
                //动态分配缓存区file_buf大小为目标文件+1,然后将目标文件读入缓存区file_buf中
                int fd = open(file_name,O_RDONLY);//文件打开的描述符号
                file_buf = new char [file_stat.st_size+1];
                memset(file_buf,'\0',file_stat.st_size + 1);
                if(read(fd,file_buf,file_stat.st_size ) < 0)
                {
                    valid = false;
                }
            }
            else
            {
                valid = false;
            }
        }
        //如果目标文件有效的话，发送正常的http应答
        if(valid)
        {
            //将http应答的状态行，"content-length"头部字段和一个空行依次加入到header_buf中
            ret = snprintf(header_buf,BUFFER_SIZE-1,"%s %s\r\n","HTTP/1.1",status_line[0]);

            len += ret;
            ret = snprintf(header_buf + len,BUFFER_SIZE-1-len,"Content-Length: %d\r\n",file_stat.st_size);    
            
            len += ret;
            ret = snprintf(header_buf + len,BUFFER_SIZE-1-len,"%s","\r\n");
            //利用writev将header_buf和file_buf的内容一并写入
            struct iovec iv[2];
            // /* Structure for scatter/gather I/O.  */
            // struct iovec
            // {
            //     void *iov_base; /* Pointer to data.  */
            //     size_t iov_len; /* Length of data.  */
            // };
            iv[0].iov_base = header_buf;
            iv[0].iov_len = strlen(header_buf);
            iv[1].iov_base = file_buf;
            iv[1].iov_len = file_stat.st_size;
            ret = writev(connfd,iv,2); //多个分散的内存数据同时写入到一个文件描述符中   
        }
        else // 目标文件如果无效。则通知客户端服务器发生了“内部错误"
        {
            ret = snprintf(header_buf,BUFFER_SIZE-1,"%s %s\r\n","HTTP/1.1",status_line[1]);

            len += ret;
            ret = snprintf(header_buf+len,BUFFER_SIZE-len-1,"%s","\r\n");
            send(connfd,header_buf,strlen(header_buf),0);   
        }
        close(connfd);
        delete [] file_buf;
    }
    close(sock);
    return 0;
}