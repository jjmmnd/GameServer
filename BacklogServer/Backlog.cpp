// 서버 코드

#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>

int main()
{
	// 성공하면 0반환
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
	{
		wprintf(L"socket err: %d", WSAGetLastError());
		return 1;
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	serverAddr.sin_port = htons(9000);

	int ret = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (ret == SOCKET_ERROR)
	{
		wprintf(L"bind err: %d", WSAGetLastError());
		return 1;
	}

	ret = listen(listenSock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		wprintf(L"listen err: %d", WSAGetLastError());
		return 1;
	}

	while (1)
	{
		// 계속 ...
		// accept를 실행하지 않음으로써 백로그 큐를 비우지 않음 
	}

	closesocket(listenSock);
	WSACleanup();
	return 0;
}

