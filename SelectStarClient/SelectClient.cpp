#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <list>
#include <locale.h>
#include <WS2tcpip.h>
using namespace std;

#define dfWIDTH 82 // 80 + NULL
#define dfHEIGHT 24 // 23
char g_map[dfHEIGHT][dfWIDTH];

struct stPlayerInfo
{
	int _id;
	int _iX;
	int _iY;
};

SOCKET g_clientSock;
list<stPlayerInfo*> g_PlayerList;
stPlayerInfo g_myPlayer;
HANDLE hConsole;
bool isMove;

void cs_Initial()
{
	CONSOLE_CURSOR_INFO stConsoleCursor;
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;			// 커서 크기
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}


void KeyProcess()
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		g_myPlayer._iX--;
		isMove = true;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		g_myPlayer._iX++;
		isMove = true;
	}
	if (GetAsyncKeyState(VK_UP))
	{
		g_myPlayer._iY--;
		isMove = true;
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		g_myPlayer._iY++;
		isMove = true;
	}

	g_myPlayer._iX = max(g_myPlayer._iX, 0);
	g_myPlayer._iX = min(g_myPlayer._iX, 80);
	g_myPlayer._iY = max(g_myPlayer._iY, 0);
	g_myPlayer._iY = min(g_myPlayer._iY, 23);
}

void Network()
{
	char buf[1600];

	fd_set ReadSet; // 읽기셋만, 보내고 싶을 떄 보내면 되니까 쓰기셋은 X
	FD_ZERO(&ReadSet);
	FD_SET(g_clientSock, &ReadSet);
	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;
	int retval = select(0, &ReadSet, NULL, NULL, &t);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("Network() select error [%d]", WSAGetLastError());
			closesocket(g_clientSock);
			WSACleanup();
			return;
		}
	}

	if (FD_ISSET(g_clientSock, &ReadSet))
	{
		// recv
		retval = recv(g_clientSock, buf, 1600, 0);
		if (retval == SOCKET_ERROR)
		{
			printf("Network() recv error [%d]", WSAGetLastError());
			closesocket(g_clientSock);
			WSACleanup();
			return;
		}

		int ret = 0;
		while (retval >= 16)
		{
			int type = (int)*(buf + ret);
			int newID = (int)*(buf + ret + 4);
			int newX = (int)*(buf + ret + 8);
			int newY = (int)*(buf + ret + 12);

			ret += 16;
			retval -= 16;

			switch (type)
			{
			case 0:
			{
				// ID 할당 (한번만 옴)
				g_myPlayer._id = newID;

				list<stPlayerInfo*>::iterator iter;
				for (iter = g_PlayerList.begin(); iter != g_PlayerList.end(); ++iter)
				{
					if ((*iter)->_id == g_myPlayer._id)
					{
						g_myPlayer._iX = (*iter)->_iX;
						g_myPlayer._iY = (*iter)->_iY;
						g_PlayerList.erase(iter);
						break;
					}
				}
				break;
			}
			case 1:
			{
				if (newID == g_myPlayer._id)
				{
					g_myPlayer._iX = newX;
					g_myPlayer._iY = newY;
					break;
				}

				// 별 생성 (클라 들어올 때마다 옴)
				stPlayerInfo* newPlayer = new stPlayerInfo;
				newPlayer->_id = newID;
				newPlayer->_iX = newX;
				newPlayer->_iY = newY;
				g_PlayerList.push_back(newPlayer);
				break;
			}
			case 2:
			{
				// 별 삭제
				list<stPlayerInfo*>::iterator iter = g_PlayerList.begin();
				for (; iter != g_PlayerList.end(); ++iter)
				{
					if ((*iter)->_id == newID)
					{
						delete* iter;
						g_PlayerList.erase(iter);
						break;
					}
				}
				break;
			}
			case 3:
			{
				// 별 이동
				list<stPlayerInfo*>::iterator iter = g_PlayerList.begin();
				for (; iter != g_PlayerList.end(); ++iter)
				{
					if ((*iter)->_id == newID)
					{
						(*iter)->_iX = newX;
						(*iter)->_iY = newY;
						break;
					}
				}
				break;
			}
			}
		}
	}

	if (isMove)
	{
		int sendPacket[4];
		sendPacket[0] = 3;
		sendPacket[1] = g_myPlayer._id;
		sendPacket[2] = g_myPlayer._iX;
		sendPacket[3] = g_myPlayer._iY;
		int retval = send(g_clientSock, (char*)sendPacket, sizeof(sendPacket), 0);
		if (retval == SOCKET_ERROR)
		{
			printf("send error [%d]\n", WSAGetLastError());
			closesocket(g_clientSock);
			return;
		}
		isMove = false;
	}
}

void cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	SetConsoleCursorPosition(hConsole, stCoord);
}

void Buffer_Flip(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfHEIGHT; iCnt++)
	{
		cs_MoveCursor(0, iCnt);
		printf(g_map[iCnt]);
	}
}

void Buffer_Clear(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfHEIGHT; iCnt++)
	{
		memset(g_map[iCnt], ' ', dfWIDTH);
		g_map[iCnt][dfWIDTH - 1] = (char)NULL;
	}
}

void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfWIDTH - 1 || iY >= dfHEIGHT)
		return;

	g_map[iY][iX] = chSprite;
}

void Render()
{
	Buffer_Clear();
	Sprite_Draw(g_myPlayer._iX, g_myPlayer._iY, '*');
	list<stPlayerInfo*>::iterator iter = g_PlayerList.begin();
	for (; iter != g_PlayerList.end(); ++iter)
	{
		Sprite_Draw((*iter)->_iX, (*iter)->_iY, '*');
	}
	Buffer_Flip();

	cs_MoveCursor(0, 0);
	printf("Client Connect: %d", g_PlayerList.size() + 1);
}

int main()
{
	// 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	g_clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_clientSock == INVALID_SOCKET)
	{
		printf("socket error [%d]\n", WSAGetLastError());
		return 1;
	}

	// connect
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(3000);
	int retval = connect(g_clientSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		// 연결 안 됐으면 에러 찍고 종료 
		printf("connect error [%d]\n", WSAGetLastError());
		return 1;
	}

	// 논블로킹 전환
	u_long on = 1;
	retval = ioctlsocket(g_clientSock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
	{
		printf("ioctlsocket error [%d]\n", WSAGetLastError());
		closesocket(g_clientSock);
		return 1;
	}


	cs_Initial();

	// 루프
	while (1)
	{
		// 키보드 입력
		KeyProcess();
		// 네트워크
		Network();

		// 랜더
		Render();

		Sleep(15);
	}
}