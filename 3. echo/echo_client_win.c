#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#define buf 1024
#pragma warning(disable : 4996)
void error(char *message);
int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN hServ_addr;
	char message[buf];
	int strLen;

	if (argc != 3)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		error("socket() error");

	memset(&hServ_addr, 0, sizeof(hServ_addr));
	hServ_addr.sin_family = AF_INET;
	hServ_addr.sin_addr.s_addr = inet_addr(argv[1]);
	hServ_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (SOCKADDR *)&hServ_addr, sizeof(hServ_addr)) == SOCKET_ERROR)
		error("connect() error");
	else
		puts("Connected.......");

	/*
	echo 클라이언트는 서버로 data를 전송하고 전송한 data를 서버로 부터 다시 재전송 받는다.
	*/
	while (1)
	{
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, buf, stdin); // message 입력.
		
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		send(sock, message, strlen(message), 0); //message를 서버로 발송.
		strLen = recv(sock, message, buf, 0); //보낸 message를 서버로 부터 다시 받음.
		message[strLen] = 0;
		printf("Message from server : %s\n", message);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

void error(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}