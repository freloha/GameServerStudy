#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData; // 윈속에 대한
    SOCKET hSocket;
    SOCKADDR_IN servAddr;

    // 1. 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        printf("Failed WSAStartup() \n");

    // 2. socket 생성
    hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hSocket == INVALID_SOCKET)
        printf("Failed socket()\n");

    // 3. 서버 연결
    // 3-1. 연결할 서버 주소 셋팅
    memset(&servAddr, 0, sizeof(servAddr));

    //AF_INET : 인터넷 프로토콜 IPv4를 의미
    servAddr.sin_family = AF_INET; 

    //servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 이 함수가 아래의 양식으로 바뀌었음
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);

    servAddr.sin_port = htons(30002);

    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        printf("Failed connect() \n");

    int recvSize;
    char recvData[255];
    recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
    if (recvSize == -1)
        printf("recv() Error\n");

    printf("recv %d message : %s \n", recvSize, recvData);

    closesocket(hSocket);
    WSACleanup();

    system("pause");
    return 0;
}
