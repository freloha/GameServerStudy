// 모든 코드는 https://cafe.naver.com/gamepromaster/67에 있습니다.

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(void) {
	WSADATA wsaData; // 윈속 구조체 생성
	SOCKET hSocket; // 소켓 구조체 생성
	SOCKADDR_IN servAddr; // 서버에 대한 주소 구조체 생성

	//1. 윈속 초기화
	if (WSAStartup(0x0202, &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return 1;
	}

	//2. socket 생성
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		WSACleanup();
		printf("Failed socket() \n");
		return 1;
	}

	//3. 서버 자신의 주소를 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	// htons() : host to network short, htonl() : host to network long 네트워크 바이트로 변환
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr); // 
	servAddr.sin_port = htons(30002);

	// 서버에 연결 시도
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("Failed Connect \n");
		closesocket(hSocket);
		WSACleanup();
		return 1;
	}

	//4. 서버로부터 통신 대기
	int recvSize;
	char recvData[255];
	char sendData[255];
	recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
	if(recvSize == SOCKET_ERROR)
	{
		printf("recv() Error \n");
	}
	printf("recv out while %s \n", recvData);

	//5. 연결된 서버와 통신
	int count = 10;
	while (count--) // count가 0이 될때까지
	{
		sendData[0] = count;
		send(hSocket, sendData, 1, 0);
		printf("send Data : %d \n", sendData[0]);
		recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
		if (recvSize == SOCKET_ERROR)
		{
			printf("recv() Error \n");
			break;
		}
		printf("recv Data : %d \n", recvData[0]);
	}

	closesocket(hSocket);
	WSACleanup();

	system("pause");

	return 0;
}
