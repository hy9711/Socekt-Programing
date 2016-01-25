#include<stdio.h>
#include<iostream>
#include<string.h>
#include<winsock2.h>
#pragma warning (disable:4996)
using namespace std;

void Error_handling(char *message);
int main(int argc, char* argv[])
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
		Error_handling("WSA() error!");

	//inet_addr() 사용하기.
	// inet_addr 함수의 정의 : in_addr_t inet_addr(const char *string) => 반환하는 것은 sin_addr.s_addr형을 반환(즉 unsigned long)하고 문자형으로 정의된 주소(우리가 보는주소)를 인수로 받음.
	{
		char *serv = "188.223.11.20"; // inet_addr의 인수때문에 문자형으로 주소 초기화.
		unsigned long conv_addr = inet_addr(serv); //inet_addr이 반환하는 형은 sin_addr.s_addr or unsigned long 형이다.

		if (conv_addr == INADDR_NONE)
			Error_handling("inet_addr() error!");
		else
			printf("컴퓨터식으로 변환된 주소 : %#lx\n\n", conv_addr);
	}

	//inet_ntoa() 사용하기.
	//inet_ntoa 함수의 정의 : char *inet_ntoa(in_addr adr) => 반환하는 것은 문자형(즉 우리가 보는 주소)이고, sin_addr or 컴퓨터 주소를 인수로 받는다.
	{
		SOCKADDR_IN addr1, addr2; //inet_ntoa의 인수는 sin_addr 임으로 반드시 sockaddr_in 구조체 형인 변수를 선언 해야한다.
		char *serv1 = "127.220.120.201";
		char *str;//inet_ntoa의 반환형은 char *.
		char *memo1, *memo2;
		
		addr1.sin_addr.s_addr = htonl(0x2030405);
		addr2.sin_addr.s_addr = inet_addr(serv1); //inet_ntoa의 인수는 sin_addr임으로 반드시 컴퓨터형 주소로 변환해야한다.

		str = inet_ntoa(addr1.sin_addr);
		memo1 = new char[strlen(str)+1];
		strcpy(memo1, str);

		str = inet_ntoa(addr2.sin_addr);
		memo2 = new char[strlen(str)+1];
		strcpy(memo2, str);

		printf("우리식으로 알 수 있는 주소 addr1 : %s", memo1);
		cout << endl;
		printf("우리식으로 알 수 있는 주소 addr2 : %s", memo2);
		cout << endl;

		delete[]memo1;
		delete[]memo2;
	}
}
void Error_handling(char *message)
{
	cout << message << endl;
}