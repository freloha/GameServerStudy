#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(void) {
	WSADATA wsaData;
	SOCKET sockServer;

	//1. 윈속 사용 초기;화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup() \n");
		return 1;
	}

	//2. server socket 생성
	sockServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockServer == INVALID_SOCKET)
	{
		WSACleanup();
		printf("Failed Server Socket() \n");
		return 1;
	}

	SOCKADDR_IN addrServer;

	//3. server 주소 binding 및 listen socket
	memset(&addrServer, 0, sizeof(addrServer));
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
	addrServer.sin_port = htons(30002);

	if (bind(sockServer, (SOCKADDR*)&addrServer, sizeof(addrServer)))
	{
		printf("binding Error\n");
		closesocket(sockServer);
		WSACleanup();
		return 1;
	}

	if (listen(sockServer, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen Error\n");
		closesocket(sockServer);
		WSACleanup();
		return 1;
	}

	printf("Start Server \n");

	// 4. select 함수 세팅
	fd_set fdSocketInfors; // socket 정보를 저장하고 있는 구조체
	fd_set fdReads; // 변화를 읽어올 구조체
	int iRet;
	unsigned int i;
	SOCKET sockClient, sockTemp;
	SOCKADDR_IN addrClient;
	int addrSize;
	char recvBuffer[255];
	int recvSize;

	FD_ZERO(&fdSocketInfors); // fd_set 구조체의 fd_count를 0으로 만들어주는 매크로 함수
	FD_SET(sockServer, &fdSocketInfors); // fd_set 구조체에 감시 대상을 넣어줌

	while (1)
	{
		fdReads = fdSocketInfors;
		
		//blocking - 감시할 소켓을 넣고 대기
		iRet = select(0, &fdReads, 0, 0, 0);

		if (iRet == SOCKET_ERROR)
			break;

		for (i = 0; i < fdSocketInfors.fd_count; i++) {
			sockTemp = fdSocketInfors.fd_array[i];
			//보유한 소켓 중에 변화가 일어난 소켓이 있는지
			if (FD_ISSET(sockTemp, &fdReads)) // fd_isset은 fd_set구조체에 해당 감시 대상이 있는지 판별
			{
				//Reads 가 일어난 소켓이 서버인 경우 accept
				if (sockTemp == sockServer)
				{
					addrSize = sizeof(addrClient);
					sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &addrSize);
					FD_SET(sockClient, &fdSocketInfors);
					printf("%d connected %d %d,%d,%d,%d : %d \n", i, sockClient,
						addrClient.sin_addr.S_un.S_un_b.s_b1,
						addrClient.sin_addr.S_un.S_un_b.s_b2,
						addrClient.sin_addr.S_un.S_un_b.s_b3,
						addrClient.sin_addr.S_un.S_un_b.s_b4,
						addrClient.sin_port);
				}
				else
				{
					recvSize = recv(sockTemp, recvBuffer, sizeof(recvBuffer), 0);
					printf("%d : socket = %d recv \n", i, sockTemp);
					if (recvSize > 0)
						send(sockTemp, recvBuffer, recvSize, 0);
					else
					{
						FD_CLR(sockTemp, &fdSocketInfors); // fd_set구조체에 감시 대상 제거
						closesocket(sockTemp);
						printf("close socket : %d \n", sockTemp);
					}
				}
			}
		}
	}

	closesocket(sockServer);
	WSACleanup();

	return 0;
}
