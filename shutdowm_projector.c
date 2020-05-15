#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"
#include "prase.h"

#define PORT 80


/*
	其投影机自身提供一个web管理方式
	此函数等同于为模拟点击关机按钮发送开机请求
	具体参数系通过抓包获得的
*/
void shutdowm_projector(struct ip_address * ip_p)
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

	//发送http报文
	char buf[200] = {0};
	sprintf(buf, "GET /cgi-bin/sender.exe?KEY=3A HTTP/1.1 \r\n");
	send(sockfd, buf, strlen(buf), 0);
	sprintf(buf, "Host: %s\r\n", ip_p->ip_all);
	send(sockfd, buf, strlen(buf), 0);
	sprintf(buf, "Upgrade-Insecure-Requests: 1");
	send(sockfd, buf, strlen(buf), 0);
	sprintf(buf, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36\r\n");
	send(sockfd, buf, strlen(buf), 0);
	sprintf(buf, "Referer: http://%s/cgi-bin/webconf.exe?page=13\r\n", ip_p->ip_all);
	send(sockfd, buf, strlen(buf), 0);
	sprintf(buf, "Accept-Encoding: gzip, deflate\r\n");
	send(sockfd, buf, strlen(buf), 0);
	sprintf(buf, "Connection: close\r\n");
	send(sockfd, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(sockfd, buf, strlen(buf), 0);

	close(sockfd);

	LOG("Send to %s OK",ip_p->ip_all);

	return ;


}
