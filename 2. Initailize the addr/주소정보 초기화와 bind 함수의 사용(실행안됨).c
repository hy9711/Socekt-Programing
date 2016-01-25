#include<stdio.h>
#include<string.h>
#include<WinSock2.h>

void a(char *message)
{
	printf("%s", message);
}

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServ_Sock;
	SOCKADDR_IN serv_addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //winsock 라이브러리 초기화
		a("WSAStartup() error!");

	hServ_Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //소켓 생성.
	if (hServ_Sock == INVALID_SOCKET)
		a("socket() error");

	//주소 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));//sockaddr_in 구조체의 크기와 sockaddr의 구조체 크기를 맞추기 위하여 memset함수 사용.
	serv_addr.sin_family = AF_INET;//INETv4 주소체계 사용.
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //=>serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(atoi(argv[2]));
	//INADDR_ANY 는 자신의 호스트 IP주소를 자동으로 초기화 해주는 키워드이다.
	/*
	보통 ip주소나 port번호는 char 형으로 선언되어지는데 이를 정수형으로 바꿔주기 위해 ip주소는 inet_addr 함수를 사용하고
	port 번호는 atoi함수를 사용한다. 두 함수 모두 인수는 char *형으로 선언된 우리가 알아볼 수 있는 주소이다.
	*/

	/*
	bind 함수의 설명 :
	int bind(SOCKET s, struct SOCKADDR *addr, int addrlen);
	인수를 소켓핸들, SOCKADDR 구조체의 주소(즉, 여기에는 주소체계, PORT번호, IP주소가 모두 SOCKADDR_IN으로 저장되어서 보내짐->반드시 (SOCKADDR *)로 형변환), SOCKADDR_IN으로 선언된 변수의 길이
	가 들어간다.
	*/
	if (bind(hServ_Sock, (SOCKADDR *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		a("bind() error!");

	WSACleanup();
	closesocket(hServ_Sock);
}