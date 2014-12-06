/* TCPServer.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include "conio.h"

#define QLEN       5
#define    WSVERS    MAKEWORD(2, 0)
#define    BUFLEN        20000  
#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library
/*------------------------------------------------------------------------
* main - Iterative TCP server for TIME service
*------------------------------------------------------------------------
*/
char buf[20000];
char buf1[20000];
char buf2[20000];
void main(int argc, char *argv[])
/* argc: 命令行参数个数， 例如：C:\> TCPServer 8080
argc=2 argv[0]="TCPServer",argv[1]="8080" */
{
	struct    sockaddr_in fsin;        /* the from address of a client      */
	SOCKET    msock, ssock;            /* master & slave sockets          */
	WSADATA wsadata;
	char    *service = "5050";
	struct  sockaddr_in sin;        /* an Internet endpoint address        */
	int        alen;                    /* from-address length                */
	char    *pts;                    /* pointer to time string            */
	//    char pts[2000];
	time_t    now;                    /* current time                        */
	int cc;

	WSAStartup(WSVERS, &wsadata);                        // 加载winsock library。WSVERS指明请求使用的版本。wsadata返回系统实际支持的最高版本
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);    // 创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
	// 返回：要监听套接字的描述符或INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));                        // 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;                            // 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;                    // 监听所有(接口的)IP地址。
	sin.sin_port = htons((u_short)atoi(service));        // 监听的端口号。atoi--把ascii转化为int，htons--主机序到网络序(16位)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // 绑定监听的IP地址和端口号

	listen(msock, 5);                                   // 等待建立连接的队列长度为5

	while (!_kbhit()){                                      // 检测是否有按键
		alen = sizeof(struct sockaddr);                   // 取到地址结构的长度
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // 如果有新的连接请求，返回连接套接字，否则，被阻塞。fsin包含客户端IP地址和端口号

		(void)time(&now);                                      // 取得系统时间
		pts = ctime(&now);                                      // 把时间转换为字符串
		sprintf(buf, "IP :%s 端口号: %d\n", inet_ntoa(fsin.sin_addr), fsin.sin_port);
		// sprintf(buf,"%d:%d Hello my friends ! %s",inet_ntoa(sin.sin_addr),fsin.sin_port,pts);
		(void)send(ssock, buf, strlen(pts), 0);          // 把缓冲区(指针,长度)的数据发送出去
		printf("             TCP(Server) Echo增强程序 \n\n");
		printf(" 时间：  %s\n", pts);
		printf(" %s\n", buf);                                      // 显示发送字符串
		printf("请等待接收数据 ：\n");

		flag1: 
		       printf("\n您还要传送什么？\n");
		       printf(" 如果想临时改为接收，请键入 * \n");
		    scanf("%s",pts);

		    if(pts[0] == '*')
		    {
		        printf("\n请等待接收数据\n");
		        sprintf(buf1,"%s",pts);
		    (void) send(ssock, buf1, strlen(pts), 0); 
		        goto flag2;
		    }
		    sprintf(buf1,"%s",pts);
		    (void) send(ssock, buf1, strlen(pts), 0);  
		    goto flag1;
		     ssock = accept(msock, (struct sockaddr *)&fsin, &alen);

	flag2:         cc = recv(ssock, buf1, BUFLEN, 0);
		if (cc == SOCKET_ERROR || cc == 0)
			printf("Error: %d.\n", GetLastError());     //出错。其后必须关闭套接字sock。
		else if (cc > 0) {
			buf1[cc] = '\0';    // ensure null-termination
			buf2[cc] = '\0';
			//         if(buf1[0] == '#')
			//         {
			//             printf("\n您接收到的数据为： # , 请传输数据 \n");
			//             goto flag2;
			//         }
			(void)time(&now);
			pts = ctime(&now);
			sprintf(buf2, "%s   \n  %s", pts, buf1);
			//    strcat(buf2,buf1);
			printf("\n您接收到的数据为：");
			//     printf(" 时间：  %s\n", pts);
			printf("%s\n", buf2);
			printf("\n         将自动把此数据返回给客户！\n");
			(void)send(ssock, buf1, strlen(pts), 0);
			goto flag2;
		}
		(void)closesocket(ssock);                              // 关闭连接套接字
	}
	(void)closesocket(msock);                                 // 关闭监听套接字
	WSACleanup();                                             // 卸载winsock library

}