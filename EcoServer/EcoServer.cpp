#pragma comment (lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <locale.h>

int main()
{
	_wsetlocale(LC_ALL, L"KOREAN");
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
	{
		wprintf(L"listen error [%d]", WSAGetLastError());
		return 1;
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(9000);
	retval = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		wprintf(L"bind error [%d]", WSAGetLastError());
		return 1;
	}

	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		wprintf(L"listen error [%d]", WSAGetLastError());
		return 1;
	}

	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrlen = sizeof(clientAddr);
	WCHAR wbuf[513];

	while (1)
	{
		// accept 1
		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrlen);
		if (clientSock == INVALID_SOCKET)
		{
			wprintf(L"accept error [%d]", WSAGetLastError());
			break;
		}
		WCHAR clientIP[16] = { 0, };
		InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
		wprintf(L"\n[TCP 서버] 클라 접속 : ip 주소 = %s 포트번호 = %d\n", clientIP, ntohs(clientAddr.sin_port));

		// 클라와 통신
		while (1)
		{
			memset(wbuf, 0, sizeof(wbuf));

			// 데이터 받기
			retval = recv(clientSock, (char*)wbuf, 512*2, 0);
			if (retval == SOCKET_ERROR)
			{
				wprintf(L"recv error [%d]", WSAGetLastError());
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			// 받은 데이터 출력
			wbuf[512] = L'\0';
			InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
			wprintf(L"[TCP/%s:%d] %s", clientIP, ntohs(clientAddr.sin_port), wbuf);

			// 데이터 보내기
			retval = send(clientSock, (char*)wbuf, retval, 0);
			if (retval == SOCKET_ERROR)
			{
				wprintf(L"send error [%d]", WSAGetLastError());
				break;
			}
			wprintf(L"[TCP 서버] %d 바이트를 보냈습니다\n\n", retval);
		}

		closesocket(clientSock);
		InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
		wprintf(L"[TCP] 클라 종료: ip주소 = %s 포트번호 = %d\n", clientIP, ntohs(clientAddr.sin_port));
	}

	closesocket(listenSock);
	WSACleanup();
	return 0;
}