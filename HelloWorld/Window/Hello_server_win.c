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

	//WSAStartup, 윈속 라이브러리 초기화 (성공하면 0을 반환, 실패하면 에러코드 반환). 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	//--------------------------------------------

	//socket, 소켓 생성 (성공하면 소켓핸들을 반환, 실패 시 INVALID_SOCKET반환).
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");
	//------------------------------------

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	//bind, ip주소와 port번호 할당 (성공 시 0반환, 실패 시 SOCKET_ERROR 반환).
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");
	//-------------------------------------------

	//listen, 소켓에 케이블을 끼움-> 연결요청을 받을 수 있음 (성공 시 0반환, 실패 시 SOCKET_ERROR 반환).
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");
	//------------------------------------------

	//accept, 연결요청에 대한 응답을 할 수 있게함 (성공 시 소켓 핸들 반환, 실패 시 INVALD_SOCKET 반환).
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error!");
	//----------------------------------------------

	send(hClntSock, message, sizeof(message), 0); // 연결된 클라이언트(소켓)에 데이터를 전송.
	closesocket(hClntSock); //소켓 해제
	closesocket(hServSock); //소켓 해제
	WSACleanup(); // 윈속 라이브러리 해제.
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}