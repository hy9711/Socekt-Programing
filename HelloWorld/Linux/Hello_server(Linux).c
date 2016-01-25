//전화 받는 소켓.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>   // 리눅스 라이브러리
#include<arpa.inet.h> //리눅스 라이브러리
#include<sys/socket.h> //소켓 관련 함수가 들어있는 라이브러리(리눅스).
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;,
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[] = "Hello World!";
	
	if (argc != 2)
	{
		printf("Usage : %s <port>\m", argv[0]);
		exit(1);
	}

	//socket, 소켓 생성 (성공하면 0을 반환하고 실패하면 -1 반환)
	serv_sock = socket(PF_INET, SOCK_STREAM, 0); 
	if (serv_sock == -1)
		error_handling("socket() error");
	//--------------------------------------------------

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//bind, ip 주소와 port 번호 할당 (성공하면 0을 반환하고 실패하면 -1 반환).
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) 
		error_handling("bind() errer");
	//-----------------------------------

	//listen, 소켓에 케이블을 연결 (성공하면 0을 반환하고 실패하면 -1 반환)->연결요청을 받을 수 있도록 함.
	if (listen(serv_scok, 5) == -1)
		error_handling("listen() error");
	//-------------------------------------

	//accept, 연결요청에 대한 응답을 할 준비 (성공하면 0을 반환하고 실패하면 -1 반환).
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept() error");
	//-------------------------------------

	write(clnt_sock, message, sizeof(message));//연결요청에 대한 응답이 긍정적일 때, 데이터를 전송.
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}