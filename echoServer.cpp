// 모든 코드는 https://cafe.naver.com/gamepromaster/67에 있습니다.

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(void) {
	WSADATA wsaData; // 윈속 구조체 생성
	SOCKET listenSocket; // 소켓 구조체 생성
	SOCKADDR_IN servAddr; // 서버에 대한 주소 구조체 생성

	//1. 윈속 초기화
	if (WSAStartup(0x0202, &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return 1;
	}

	//2. socket 생성
	listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		printf("Failed socket() \n");
		return 1;
	}

	//3. 서버 자신의 주소를 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	// htons() : host to network short, htonl() : host to network long 네트워크 바이트로 변환
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(30002);

	if (bind(listenSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("Binding Error \n");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		printf("listen Error \n");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("Server Start \n");

	SOCKADDR_IN clientAddr;
	SOCKET clientSocket;
	int sizeClientAddr = sizeof(clientAddr);
	char sendData[255] = "Hi";
	char recvByte;
	char recvData[255];
	int recvCount = 0;

	//4. 접속이 시도된 클라이언트 연결과 통신 처리
	while (1) 
	{
		clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &sizeClientAddr);
		if (clientSocket == INVALID_SOCKET)
			printf("Failed accept()");

		printf("accept client socket : %d \n", clientSocket);

		// 연결된 클라이언트에게 send
		send(clientSocket, sendData, strlen(sendData) + 1, 0); // 마지막 바이트에 + 1해줘야 원본 전달 가능

		// 접속한 client 가 접속을 끊을때까지 recv 대기를 반복한다.
		//printf("recv count : ");
		while ((recvByte = recv(clientSocket, recvData, sizeof(recvData), 0)) > 0) // 0이면 recv한 것이 없는 거고, 양수이면 어떠한 데이터가 recv
		{
			// 받은 메시지를 그대로 전달
			printf("recv count : %d ,", ++recvCount);
			printf("recv Data : %d \n", recvData[0]);
			send(clientSocket, recvData, 1, 0);
		}

		// 접속이 끊긴 처리
		printf("\n");
		closesocket(clientSocket);
		printf("close socket : %d \n", clientSocket);
	}

	//5. 소켓 종료 -> 윈속 종료
	closesocket(listenSocket);
	WSACleanup();

	return 0;
}
