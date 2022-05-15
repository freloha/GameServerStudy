#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    /*
     typedef struct WSADATA{
    WORD wVersion; //윈속 버전 (예 2.1)
    WORD wHighVersion;   //최상위의 윈속 버전 (예 2.2)
    char szDescription[WSADESCRIPTION_LEN + 1];   //회사 정보
    char szSystemStatus[WSASYS_STATUS_LEN + 1];   //구성 정보
    unsigned short iMaxSockets;   //한 프로세스가 열 수 있는 소켓 수
    unsigned short iMaxUdpDg;    //UDP 패킷 크기
    char* lpVendorInfo;   //회사별 데이터 구조의 포인터
    }WSADATA;
    */
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
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(30002);

    //servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 이 함수가 아래의 양식으로 바뀌었음
    //inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);

    if (bind(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        printf("Binding Error \n");
    if (listen(hSocket, 5) == SOCKET_ERROR)
        printf("listen Error\n");

    // 클라이언트 연결 기다리기
    SOCKADDR_IN clientAddr;
    SOCKET clientSocket;
    int sizeClientAddr = sizeof(clientAddr);
    clientSocket = accept(hSocket, (SOCKADDR*)&clientAddr, &sizeClientAddr);
    if (clientSocket == SOCKET_ERROR)
        printf("Failed accept() \n");

    // 4. 클라이언트에게 send
    char sendData[255] = "hello";
    send(clientSocket, sendData, strlen(sendData) + 1, 0);

    // 5. 소켓 종료 -> 윈속 종료
    closesocket(clientSocket);
    closesocket(hSocket);
    WSACleanup();

    return 0;
}
