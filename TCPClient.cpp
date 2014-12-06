/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include "conio.h"
#define    BUFLEN        20000                  // 缓冲区大小
#define WSVERS        MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary

/*------------------------------------------------------------------------
* main - TCP client for TIME service
*------------------------------------------------------------------------
*/
char    buf1[BUFLEN + 1];
char    buf[BUFLEN + 1];                   /* buffer for one line of text    */
void main(int argc, char *argv[])
{
	char *host = "127.0.0.1";
	//    char    *host = "10.4.4.146";        /* server IP to connect         */

	char    *service = "5050";          /* server port to connect       */
	struct sockaddr_in sin, fsin;                /* an Internet endpoint address    */


	SOCKET    sock, ssock;                          /* socket descriptor            */
	int    cc, alen;   /* recv character count            */
	char    *pts;
	time_t    now;
	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);                          //加载winsock library。WSVERS为请求的版本，wsadata返回系统实际支持的最高版本

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);      //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议

	//返回：要监听套接字的描述符或INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));                          // 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;                              // 因特网地址簇
	sin.sin_addr.s_addr = inet_addr(host);                // 服务器IP地址(32位)
	sin.sin_port = htons((u_short)atoi(service));         // 服务器端口号  
	connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // 连接到服务器
	//    alen = sizeof(struct sockaddr); 
	//    ssock = accept(sock, (struct sockaddr *)&fsin, &alen);    
	printf("             TCP(Client) Echo增强程序 \n\n");
flag1:   cc = recv(sock, buf1, BUFLEN + 1, 0);                // cc为接收到的字符的个数(>0)或对方已关闭(=0)或连接出错(<0)
	if (cc == SOCKET_ERROR || cc == 0)
		printf("Error: %d.\n", GetLastError());     //出错。其后必须关闭套接字sock。
	else if (cc > 0) {
		buf1[cc] = '\0';    // ensure null-termination
		//         if(buf[0] == '*')
		//         {
		//             printf("\n您接收到的数据为： * , 请传输数据 \n");
		//             goto flag2;
		//         }
		(void)time(&now);                                      // 取得系统时间
		pts = ctime(&now);
		printf("您接收到的数据为：\n");
		printf(" 时间：  %s\n", pts);
		printf(" %s\n", buf1);                         // 显示所接收的字符串
	}
	goto flag2;

flag2:



	printf("\n您想传送什么？\n");
	printf("    如果想关闭连接，请输入 @ \n");
	//    printf("\n 如果想临时改为接收，请键入 # \n");
	scanf("%s", pts);
	if (pts[0] == '@')
		goto end;
	//    if(pts[0] == '#')
	//    {
	//        printf("\n请等待接收数据\n");
	//        sprintf(buf1,"%s",pts);
	//     (void) send(sock, buf1, strlen(pts), 0);
	//        goto flag1;
	//    }
	sprintf(buf1, "%s", pts);
	(void)send(sock, buf1, strlen(pts), 0);
	goto flag1;
end:    closesocket(sock);                             // 关闭监听套接字

	WSACleanup();                                  // 卸载winsock library

	printf("\n按回车键继续...");
	getchar();                                        // 等待任意按键
}