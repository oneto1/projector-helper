#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/ip.h>
#include<netinet/in.h>
#include<netinet/ip_icmp.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>

/*可单独编译为一个小程序,为了root权限影响最小化
	使用方法 	 printf("Usage: %s [ip address]",argv[0]); 
*/


//#define SELECT 0
//#define NORMAL 0
/*1.如果只定义了SELECT,超时处理为selcet版实现
	2.如果只定义了NORMAL则为堵塞式实现(若ping的主机不回复,
	将一直堵塞,无超时处理)
	3.两者都没定义,超时处理为信号实现
*/

#ifdef SELECT
#include<sys/select.h>
#endif

#ifndef  NORMAL
#include<sys/signal.h>
#endif


#define ICMPDATALEN 56
#define MAXIPLEN 60
#define MAXICMPLEN 76
#define BUFLEN MAXICMPLEN+MAXIPLEN+ICMPDATALEN


int check_sum(unsigned short *,int );
void send_pak(char *);
void unpack_pak(void );
void  ping_stat(int  );
void exit_error(char *);

int pid;								
int sockfd;					
char buf[BUFLEN];
char *iplist;

int recv_num = 0;
int send_num = 0;

int main(int argc,char **argv )
{
    if(argc != 2){
        printf("Usage: %s [ip address]",argv[0]);
        exit(1);
    }

    iplist = argv[1];

    send_pak(iplist);
    unpack_pak();

    ping_stat(0);

}

/*校验和算法*/
int check_sum(unsigned short *buf, int len)
{
	int nleft = len;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;

	//每次相加以两字节为单位
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;

	return (ans);
}


/*发送icmp包*/
void send_pak(char *iplist)
{
    struct protoent * p_prote;
    p_prote= getprotobyname("ICMP");

	//使用当前进程号作为唯一标识号
    pid = getpid();

	sockfd = socket(AF_INET,SOCK_RAW,p_prote->p_proto);

    if(sockfd < 0)
    	exit_error("socket error ");

    // 初始化icmp数据结构
    struct icmp s_icmp;

    s_icmp.icmp_type = ICMP_ECHO;					//报头类型
    s_icmp.icmp_code = 0;										 //编码
    s_icmp.icmp_cksum = 0;									  //校验和
   	s_icmp.icmp_id = pid;										  //标识
 	s_icmp.icmp_seq = 0;					//顺序号
	
	/*	
	若需要计算往返时间
    struct timeval *tv;
    tv = (struct timeval *)(&s_icmp+8);//icmp报文字节长为8字节
    gettimeofday(tv,0);
	*/

    s_icmp.icmp_cksum = check_sum((unsigned short *)&s_icmp,64);

	//初始化
    struct sockaddr_in s_addr_in;
    memset(&s_addr_in,0,sizeof(s_addr_in));
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_addr.s_addr = inet_addr(iplist);

    int ret = sendto(sockfd,&s_icmp,ICMPDATALEN+8,0,(struct sockaddr *)&s_addr_in,sizeof(s_addr_in));

    if(ret < 0){
        exit_error("sendto error ");
    }else{
			send_num++;
        }
		
	//收回root权限
	setuid(getuid());

	return ; 

}

void unpack_pak(void )
{

#ifdef SELECT
/* 超时实现由select() 实现*/

	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(sockfd,&rset);

	//设置超时时间为5s
	struct timeval time = {5,0};

	int ret = select(sockfds[i]+1,&rset,NULL,NULL,&time);

	if(ret < 0)
		exit_error("select error ");
	else	if(ret != 0 )
					if(FD_ISSET(sockfds[i--],&rset))
					{
						recv_num++;
					}
}
#endif

#ifdef NORMAL
/* 若recvfrom()一直收不到回复,将一直堵塞,无超时处理*/

		struct sockaddr_in recv_in;
		socklen_t len = sizeof(recv_in);
		int recvlen ;
		if((recvlen =  recvfrom(sockfd,buf,BUFLEN,0,(struct sockaddr *)&recv_in,&len)) <0 )
			exit_error("recv error ");

		//若收到的报文长度小于最小ip报文长度,则受到的不是所需数据
		if(recvlen > MAXIPLEN){
				struct ip *p_ip = (struct ip *)buf;
				int iplen =p_ip->ip_hl << 2;				//首部长度以4字节为单位

				struct icmp * p = (struct icmp *)(buf+iplen);		//跳过ip首部

				//若收到的报文标识号不是发送程序的pid或报文类型不是 reply则不是想要的报文
				if(p->icmp_id == pid  && p->icmp_type == ICMP_ECHOREPLY){
					recv_num++;
				}
			}
		else
			printf("bad packet\n");


#else
/*信号*/

	//超过3秒后即退出
	signal(SIGALRM, ping_stat);
	alarm(3);															//默认不处理超时信号则退出整个程序

	while(1 ){
		struct sockaddr_in recv_in;
		socklen_t len = sizeof(recv_in);
		int recvlen ;
		if((recvlen =  recvfrom(sockfd,buf,BUFLEN,0,(struct sockaddr *)&recv_in,&len)) <0 )
			exit_error("recv error ");

		if(recvlen > MAXIPLEN){
			struct ip *p_ip = (struct ip *)buf;
			int iplen =p_ip->ip_hl << 2;

			struct icmp * p = (struct icmp *)(buf+iplen);

			if(p->icmp_id == pid  && p->icmp_type == ICMP_ECHOREPLY){
				recv_num++;
				break;
			}
		}
	}

	return ;


#endif
}


/*	该函数功能有两个,一为程序状态显示,二为信号处理*/
void ping_stat(int foo)
{
    if(recv_num > 0){
		printf("%s is live\n",iplist);
		exit(0);
	}else{
		printf("%s is dead\n",iplist);
		exit(1);
	}
        
}

/*虽然主函数已有定义, 但为了能单独作为一个独立的小程序,故保留*/
void exit_error(char *p){
    perror(p);
    exit(1);
}
