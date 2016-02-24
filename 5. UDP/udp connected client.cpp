#include<iostream>
#include<stdio.h>
#include<WinSock2.h>
using namespace std;
#define BUF_SIZE 30
#pragma warning(disable : 4996)
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN toAdr;

	if (argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <PORT>" << endl;
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("startup() Error!");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("sock() Error!");

	memset(&toAdr, 0, sizeof(toAdr));
	toAdr.sin_family = AF_INET;
	toAdr.sin_addr.s_addr = inet_addr(argv[1]);
	toAdr.sin_port = htons(atoi(argv[2]));

	connect(sock, (SOCKADDR *)&toAdr, sizeof(toAdr));

	char message[BUF_SIZE];
	while (1)
	{
		fputs("Insert message(q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		send(sock, message, strlen(message), 0);
		int strLen = recv(sock, message, sizeof(message) - 1, 0);
		message[strLen] = '\0';
		cout << "Message From Server : " << message;
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}