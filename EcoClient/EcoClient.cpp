#pragma comment(lib,"ws2_32")
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

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR)
	{
		wprintf(L"socekt error [%d]", WSAGetLastError());
		return 1;
	}

	// connect
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(9000);
	retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		wprintf(L"connect error [%d]", WSAGetLastError());
	}

	WCHAR wbuf[513];
	char cbuf[513];
	int len;

	while (1)
	{
		wprintf(L"\n[보낼데이터] ");
		if (fgetws(wbuf, 513, stdin) == NULL)
			break;

		len = wcslen(wbuf);
		if (wbuf[len + 1] == L'\n')
			wbuf[len + 1] = L'\0';
		if (wcslen(wbuf) == 0)
			break;

		// 데이터 보내기
		retval = send(sock, (char*)wbuf, 2*wcslen(wbuf), 0);
		if (retval == SOCKET_ERROR)
		{
			wprintf(L"send error [%d]", WSAGetLastError());
			break;
		}
		wprintf(L"[TCP 클라이언트] %d 바이트를 보냈습니다\n", retval);

		// 데이터 받기
		retval = recv(sock, (char*)wbuf, retval, MSG_WAITALL);
		if (retval == SOCKET_ERROR)
		{
			wprintf(L"recv error [%d]", WSAGetLastError());
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		wbuf[retval] = L'\0';
		wprintf(L"[TCP 클라이언트] %d 바이트를 받았습니다\n", retval);
		wprintf(L"[받은데이터] %s\n", wbuf);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}
