#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#define buf 1024
void error(char *message);
int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServ, hClnt;
	SOCKADDR_IN hserv_addr, hClnt_addr;
	char message[buf];
	int szClnt, i, strLen;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error("WSAStartup() error");

	hServ = socket(PF_INET, SOCK_STREAM, 0);
	if (hServ == INVALID_SOCKET)
		error("socket() error");

	memset(&hserv_addr, 0, sizeof(hserv_addr));
	hserv_addr.sin_family = AF_INET;
	hserv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	hserv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(hServ, (SOCKADDR *)&hserv_addr, sizeof(hserv_addr)) == SOCKET_ERROR)
		error("bind() error");

	if (listen(hServ, 5) == SOCKET_ERROR)
		error("listen() error");

	/*
	echo 서버란 ? 클라이언트가 보낸 data를 수신하고 수신한 data를 다시 클라이언트에게 재 전송하는 서버를 말한다.
	*/
	szClnt = sizeof(hClnt_addr);
	for (i = 0; i < 5; i++) //대기 큐 목록. 
	{
		// 연결 승인.
		hClnt = accept(hServ, (SOCKADDR *)&hClnt_addr, &szClnt);
		if (hClnt == INVALID_SOCKET)
			error("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		while ((strLen = recv(hClnt, message, buf, 0)) != 0) // 클라이언트로 부터 data를 수신받고
			send(hClnt, message, strLen, 0); // 수신 받은 data를 다시 클라이언트에게 전송한다.

		closesocket(hClnt);
	}
	closesocket(hServ);
	WSACleanup();
	return 0;
}

void error(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}