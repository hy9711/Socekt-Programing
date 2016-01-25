#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>
#pragma warning (disable : 4996)
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello World!";
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	//WSAStartup, ���� ���̺귯�� �ʱ�ȭ (�����ϸ� 0�� ��ȯ, �����ϸ� �����ڵ� ��ȯ). 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	//--------------------------------------------

	//socket, ���� ���� (�����ϸ� �����ڵ��� ��ȯ, ���� �� INVALID_SOCKET��ȯ).
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");
	//------------------------------------

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	//bind, ip�ּҿ� port��ȣ �Ҵ� (���� �� 0��ȯ, ���� �� SOCKET_ERROR ��ȯ).
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");
	//-------------------------------------------

	//listen, ���Ͽ� ���̺��� ����-> �����û�� ���� �� ���� (���� �� 0��ȯ, ���� �� SOCKET_ERROR ��ȯ).
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");
	//------------------------------------------

	//accept, �����û�� ���� ������ �� �� �ְ��� (���� �� ���� �ڵ� ��ȯ, ���� �� INVALD_SOCKET ��ȯ).
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error!");
	//----------------------------------------------

	send(hClntSock, message, sizeof(message), 0); // ����� Ŭ���̾�Ʈ(����)�� �����͸� ����.
	closesocket(hClntSock); //���� ����
	closesocket(hServSock); //���� ����
	WSACleanup(); // ���� ���̺귯�� ����.
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}