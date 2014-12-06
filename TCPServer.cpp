/* TCPServer.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include "conio.h"

#define QLEN       5
#define    WSVERS    MAKEWORD(2, 0)
#define    BUFLEN        20000  
#pragma comment(lib,"ws2_32.lib")  //ʹ��winsock 2.2 library
/*------------------------------------------------------------------------
* main - Iterative TCP server for TIME service
*------------------------------------------------------------------------
*/
char buf[20000];
char buf1[20000];
char buf2[20000];
void main(int argc, char *argv[])
/* argc: �����в��������� ���磺C:\> TCPServer 8080
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

	WSAStartup(WSVERS, &wsadata);                        // ����winsock library��WSVERSָ������ʹ�õİ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);    // �����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
	// ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));                        // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;                            // ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;                    // ��������(�ӿڵ�)IP��ַ��
	sin.sin_port = htons((u_short)atoi(service));        // �����Ķ˿ںš�atoi--��asciiת��Ϊint��htons--������������(16λ)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // �󶨼�����IP��ַ�Ͷ˿ں�

	listen(msock, 5);                                   // �ȴ��������ӵĶ��г���Ϊ5

	while (!_kbhit()){                                      // ����Ƿ��а���
		alen = sizeof(struct sockaddr);                   // ȡ����ַ�ṹ�ĳ���
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // ������µ��������󣬷��������׽��֣����򣬱�������fsin�����ͻ���IP��ַ�Ͷ˿ں�

		(void)time(&now);                                      // ȡ��ϵͳʱ��
		pts = ctime(&now);                                      // ��ʱ��ת��Ϊ�ַ���
		sprintf(buf, "IP :%s �˿ں�: %d\n", inet_ntoa(fsin.sin_addr), fsin.sin_port);
		// sprintf(buf,"%d:%d Hello my friends ! %s",inet_ntoa(sin.sin_addr),fsin.sin_port,pts);
		(void)send(ssock, buf, strlen(pts), 0);          // �ѻ�����(ָ��,����)�����ݷ��ͳ�ȥ
		printf("             TCP(Server) Echo��ǿ���� \n\n");
		printf(" ʱ�䣺  %s\n", pts);
		printf(" %s\n", buf);                                      // ��ʾ�����ַ���
		printf("��ȴ��������� ��\n");

		flag1: 
		       printf("\n����Ҫ����ʲô��\n");
		       printf(" �������ʱ��Ϊ���գ������ * \n");
		    scanf("%s",pts);

		    if(pts[0] == '*')
		    {
		        printf("\n��ȴ���������\n");
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
			printf("Error: %d.\n", GetLastError());     //����������ر��׽���sock��
		else if (cc > 0) {
			buf1[cc] = '\0';    // ensure null-termination
			buf2[cc] = '\0';
			//         if(buf1[0] == '#')
			//         {
			//             printf("\n�����յ�������Ϊ�� # , �봫������ \n");
			//             goto flag2;
			//         }
			(void)time(&now);
			pts = ctime(&now);
			sprintf(buf2, "%s   \n  %s", pts, buf1);
			//    strcat(buf2,buf1);
			printf("\n�����յ�������Ϊ��");
			//     printf(" ʱ�䣺  %s\n", pts);
			printf("%s\n", buf2);
			printf("\n         ���Զ��Ѵ����ݷ��ظ��ͻ���\n");
			(void)send(ssock, buf1, strlen(pts), 0);
			goto flag2;
		}
		(void)closesocket(ssock);                              // �ر������׽���
	}
	(void)closesocket(msock);                                 // �رռ����׽���
	WSACleanup();                                             // ж��winsock library

}