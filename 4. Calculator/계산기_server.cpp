#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#include<iostream>
#pragma warning (disable : 4996)
#define INT 4
void error(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int cal(int count, int* number, char op);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET serv, clnt;
	SOCKADDR_IN serv_addr, clnt_addr;

	if (argc != 2)
	{
		printf("<Usage> %s\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error("초기화 error");

	serv = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv == INVALID_SOCKET)
		error("socket error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv, (SOCKADDR *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		error("bind() error");

	if (listen(serv, 5) == SOCKET_ERROR)
		error("listen() error");

	int szClnt = sizeof(clnt_addr);
	char count;

	for (int i = 0; i < 5; i++)
	{
		clnt = accept(serv, (SOCKADDR *)&clnt_addr, &szClnt);
		if (clnt == INVALID_SOCKET)
			error("accept error");
		else
			printf("connected %d\n", i + 1);

		recv(clnt, &count, 1, 0);
		int int_count = (int)count;
		int recv_len = 0;
		char *number = new char[int_count*INT + 3];

		while ((INT*int_count + 1) > recv_len)
		{
			static int z = 0;
			if ((z = (recv(clnt, &number[recv_len], INT, 0))) == -1)
				error("recv() error");
			else
				recv_len += z;
		}
		int result;
		result = cal(int_count, (int *)number, number[recv_len - 1]);

		send(clnt, (char *)&result, INT, 0);
		delete[]number;
		closesocket(clnt);
	}
	closesocket(serv);
	WSACleanup();
	return 0;
}

int cal(int count, int* number, char op)
{
	int result = number[0];

	switch (op)
	{
	case '+':
	{
		for (int i = 1; i < count; i++)
			result += number[i];
		break;
	}
	case '-':
	{
		for (int i = 1; i < count; i++)
			result -= number[i];
		break;
	}
	case '*':
	{
		for (int i = 1; i < count; i++)
			result *= number[i];
		break;
	}
	}

	return result;
}