#include <Windows.h>
#include <WinBase.h>

#include "common.h"
#include <process.h>    /* _beginthread, _endthread */  

//#include <string.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <stddef.h>  
//#include <stdlib.h>  
//#include <conio.h>  


#define PORT_NUM      5
#define MAX_MSG_LEN   1024
#define SERVER_IP     "13.125.9.76"							// ���� IP �ּ�

void RecvThreadPoint(void *param);
int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);						// WinSock �ʱ�ȭ	

	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		// ���� ����
	if (sock == -1)
	{
		return -1;
	}

	SOCKADDR_IN addr_server = { 0 };							// ���� �ּ�
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = inet_addr(SERVER_IP);
	addr_server.sin_port = htons(PORT_NUM);

	if (connect(sock, (struct sockaddr *)&addr_server, sizeof(addr_server)) == -1)
	{
		return -1;
	}

	_beginthread(RecvThreadPoint, 0, (void *)sock);

	char msg[MAX_MSG_LEN] = "";

	while (true)
	{
		gets_s(msg, MAX_MSG_LEN);
		send(sock, msg, sizeof(msg), 0);					// �۽�
		if (strcmp(msg, "exit") == 0)
		{
			break;
		}
	}

	closesocket(sock);										// ���� �ݱ�    

	WSACleanup();											// WinSock ����
	return 0;
}

void RecvThreadPoint(void *param)
{
	SOCKET sock = (SOCKET)param;
	char msg[MAX_MSG_LEN];

	SOCKADDR_IN addr_client = { 0 };
	int len = sizeof(addr_client);

	while (recv(sock, msg, MAX_MSG_LEN, 0) > 0)
	{
		printf("%s\n", msg);
	}
	closesocket(sock);
}
