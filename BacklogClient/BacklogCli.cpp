
#pragma comment(lib,"ws2_32")
#include<WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;
	
	int cnt = 0;
	while (1)
	{
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			wprintf(L"socket err: %d", WSAGetLastError());
			wprintf(L"max socket: %d", cnt);
			return 1;
		}
		
		
		LINGER ling;
		ling.l_onoff = 1;
		ling.l_linger = 0;
		setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
		
		
		SOCKADDR_IN serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(9000);
		InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);
		int ret = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
		if (ret == SOCKET_ERROR)
		{
			wprintf(L"connect err: %d", WSAGetLastError());
			wprintf(L"max socket: %d", cnt);
			while (1);
		}

		closesocket(sock);
		cnt++;
	}
	WSACleanup();

	return 0;
}