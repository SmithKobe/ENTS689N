/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include "conio.h"
#define    BUFLEN        20000                  // ��������С
#define WSVERS        MAKEWORD(2, 0)        // ָ���汾2.0 
#pragma comment(lib,"ws2_32.lib")         // ʹ��winsock 2.0 Llibrary

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
	WSAStartup(WSVERS, &wsadata);                          //����winsock library��WSVERSΪ����İ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);      //�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��

	//���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));                          // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;                              // ��������ַ��
	sin.sin_addr.s_addr = inet_addr(host);                // ������IP��ַ(32λ)
	sin.sin_port = htons((u_short)atoi(service));         // �������˿ں�  
	connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // ���ӵ�������
	//    alen = sizeof(struct sockaddr); 
	//    ssock = accept(sock, (struct sockaddr *)&fsin, &alen);    
	printf("             TCP(Client) Echo��ǿ���� \n\n");
flag1:   cc = recv(sock, buf1, BUFLEN + 1, 0);                // ccΪ���յ����ַ��ĸ���(>0)��Է��ѹر�(=0)�����ӳ���(<0)
	if (cc == SOCKET_ERROR || cc == 0)
		printf("Error: %d.\n", GetLastError());     //����������ر��׽���sock��
	else if (cc > 0) {
		buf1[cc] = '\0';    // ensure null-termination
		//         if(buf[0] == '*')
		//         {
		//             printf("\n�����յ�������Ϊ�� * , �봫������ \n");
		//             goto flag2;
		//         }
		(void)time(&now);                                      // ȡ��ϵͳʱ��
		pts = ctime(&now);
		printf("�����յ�������Ϊ��\n");
		printf(" ʱ�䣺  %s\n", pts);
		printf(" %s\n", buf1);                         // ��ʾ�����յ��ַ���
	}
	goto flag2;

flag2:



	printf("\n���봫��ʲô��\n");
	printf("    �����ر����ӣ������� @ \n");
	//    printf("\n �������ʱ��Ϊ���գ������ # \n");
	scanf("%s", pts);
	if (pts[0] == '@')
		goto end;
	//    if(pts[0] == '#')
	//    {
	//        printf("\n��ȴ���������\n");
	//        sprintf(buf1,"%s",pts);
	//     (void) send(sock, buf1, strlen(pts), 0);
	//        goto flag1;
	//    }
	sprintf(buf1, "%s", pts);
	(void)send(sock, buf1, strlen(pts), 0);
	goto flag1;
end:    closesocket(sock);                             // �رռ����׽���

	WSACleanup();                                  // ж��winsock library

	printf("\n���س�������...");
	getchar();                                        // �ȴ����ⰴ��
}