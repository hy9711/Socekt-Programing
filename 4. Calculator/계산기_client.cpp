#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#pragma warning (disable : 4996)
#define INT 4
void error(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argv, char *argc[])
{
	WSADATA wsaData;
	SOCKET hclnt;
	SOCKADDR_IN serv_addr;

	if (argv != 3)
	{
		printf("<Usage> %s\n", argc[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error("초기화 error");

	hclnt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hclnt == INVALID_SOCKET)
		error("socket error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argc[1]);
	serv_addr.sin_port = htons(atoi(argc[2]));

	if (connect(hclnt, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		error("conect error");
	else
		puts("Connected...............");

	int count;
	char *opmsg;
	fputs("Operand count : ", stdout);
	scanf("%d", &count);
	opmsg = new char[count * INT + 3];
	opmsg[0] = (char)count;

	for (int i = 0; i < count; i++)
	{
		printf("Number %d : ", i + 1);
		scanf("%d", (int *)&opmsg[INT*i + 1]);
	}
	char result[4];
	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf("%c", &opmsg[INT*count + 1]);
	send(hclnt, opmsg, INT*count + 2, 0);

	int recvlen = 0;
	while (recvlen < 4)
	{
		static int z = 0;
		if ((z = (recv(hclnt, &result[recvlen], INT, 0))) == -1)
			error("recv error");
		else
			recvlen += z;
	}

	int *resulted = (int *)result;

	printf("Operation result : %d\n", *resulted);
	closesocket(hclnt);
	WSACleanup();
	delete[]opmsg;
	return 0;
}