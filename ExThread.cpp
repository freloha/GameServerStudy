#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

unsigned int __stdcall ThreadFunc(void* _pArgs) 
{
	int recvByte;
	char recvData[255];
	int recvCount = 0;
	SOCKET socket = *(SOCKET*)_pArgs;

	while ((recvByte = recv(socket, recvData, sizeof(recvData), 0)) > 0)
	{
		//받은 걸 그대로 보낸다.
		recvData[recvByte] = '\0';
		printf("recv 채팅 : %s \n", recvData);
	}

	printf("close thread \n");
	return 0;
}

int main(void)
{
	int i = 0;
	unsigned int threadID;

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	//1. 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup() \n");
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

	//3. 서버 주소를 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
	servAddr.sin_port = htons(30002);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("listen Error\n");
		closesocket(hSocket);
		WSACleanup();
		return 1;
	}

	printf("Connect....\n");

	HANDLE hThread;

	hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadFunc, (void*)&hSocket, 0, &threadID);
	printf("start recv thread \n");

	char chat[255];
	int chatSize;
	while (1) {
		scanf_s("%s", chat, (unsigned)sizeof(chat));
		if (strcmp(chat, "exit") == 0)
			break;
		chatSize = strnlen_s(chat, sizeof(chat));
		if (chatSize > 0)
			send(hSocket, chat, chatSize, 0);
	}

	//5. 소켓 종료 -> 윈속 종료
	CloseHandle(hThread);
	WSACleanup();

	return 0;
}
