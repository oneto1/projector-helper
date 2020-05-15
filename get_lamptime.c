#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include "prase.h"
#include "log.h"

//官方管理应用通信管理端口
#define PORT 3629

void handle(int);

/*
    根据 'ESCVP21 command guide for business projector'所说
    获取灯时的命令为"LAMP?"
    所以我们发送后接受回复就行
*/
void get_lamptime(struct ip_address *ip_p)
{

    //建立TCP连接
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        EXIT_ERROR("socket error ");

    //初始化
    struct sockaddr_in addr_in = {
        .sin_addr.s_addr = inet_addr(ip_p->ip_all),
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
    };

    if (connect(sockfd, (struct sockaddr *)&addr_in, sizeof(addr_in)) < 0)
        EXIT_ERROR("connect error ");

    char cmd[] = "LAMP?";
    if (send(sockfd, cmd, strlen(cmd), 0) < 0)
        EXIT_ERROR("send error ");

    //定时3秒
    signal(SIGALRM, handle);
    alarm(3);

    char buf[20] = {0};
    while (1)
    {

        if (recv(sockfd, buf, sizeof(buf), 0) < 0)
            EXIT_ERROR("recv error ");
        else
        {
            printf("get it!%s : %s", ip_p->ip_all, buf);
            break;
        }
    }

    close(sockfd);

    LOG("Send to %s OK",ip_p->ip_all);

    return ;
}

/*超时处理*/
void handle(int foo)
{
    EXIT_ERROR("Projector is busy or dead , please try again !");

}
