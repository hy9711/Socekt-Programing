#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#include<iostream>
using namespace std;
#define BUF_SIZE 30
#pragma warning(disable : 4996)
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	char message[BUF_SIZE];
	int strLen;
	int adrSz;

	SOCKADDR_IN servAdr, fromAdr;

	if (argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <Port>" << endl;
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);

	if (sock == INVALID_SOCKET)
		ErrorHandling("sock() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));
	
	while (1)
	{
		fputs("Insert message(q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		sendto(sock, message, strlen(message), 0,(SOCKADDR *)&servAdr, sizeof(servAdr));
		adrSz = sizeof(fromAdr);
		strLen = recvfrom(sock, message, BUF_SIZE, 0, (SOCKADDR *)&fromAdr, &adrSz);
		message[strLen] = 0;
		cout << "Message from server : " << message << endl;
	}
	closesocket(sock);
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}