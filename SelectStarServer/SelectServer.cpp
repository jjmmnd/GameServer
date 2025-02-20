#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <list>
using namespace std;

#define dfWIDTH 82
#define dfHEIGHT 24
char g_map[dfHEIGHT][dfWIDTH];

struct stPlayerInfo
{
	bool bRemove;
	SOCKET sock;
	int id;
	int iX;
	int iY;
};

list<stPlayerInfo*> g_PlayerList; // 최대 63명
SOCKET g_listenSock;
HANDLE hConsole;
int g_id = 1;

void SendBroadCast(stPlayerInfo* pPlayer, char* buf);

void Disconnect()
{
	int retval;
	int sendPacket[4];
	list<stPlayerInfo*>::iterator iter;
	for (iter = g_PlayerList.begin(); iter != g_PlayerList.end(); ++iter)
	{
		if ((*iter)->bRemove)
		{
			closesocket((*iter)->sock);
			sendPacket[0] = 2;
			sendPacket[1] = (*iter)->id;
			SendBroadCast(*iter, (char*)sendPacket);
			delete* iter;
			g_PlayerList.erase(iter);
		}
	}
}

void SendUnicast(stPlayerInfo* pPlayer, char* buf)
{
	int Error;
	int retval;
	retval = send(pPlayer->sock, buf, 16, 0);
	if (retval == SOCKET_ERROR)
	{
		Error = WSAGetLastError();
		if (Error != WSAEWOULDBLOCK)
		{
			if (Error == 10054)
			{
				pPlayer->bRemove = true;
			}
			printf("SendUniCast send error [%d]", WSAGetLastError());
		}
	}
}

void SendBroadCast(stPlayerInfo* pPlayer, char* buf)
{
	int Error;
	int retval;
	list<stPlayerInfo*>::iterator iter;
	for (iter = g_PlayerList.begin(); iter != g_PlayerList.end();++iter)
	{
		if ((*iter) == pPlayer)
			continue;
		if ((*iter)->bRemove)
			continue;
		retval = send((*iter)->sock, buf, 16, 0);
		if (retval == SOCKET_ERROR)
		{
			Error = WSAGetLastError();
			if (Error != WSAEWOULDBLOCK)
			{
				if (Error == 10054)
				{
					(*iter)->bRemove = true;
					continue;
				}
				printf("SendBroadCast send error [%d]", Error);
			}
		}
	}
}

bool AcceptNewPlayer()
{
	if (g_PlayerList.size() > 63)
	{
		printf("maximum client socket count is 63");
		return false;
	}

	SOCKADDR_IN clientAddr;
	int addrlen = sizeof(clientAddr);
	SOCKET newSock = accept(g_listenSock, (SOCKADDR*)&clientAddr, &addrlen);
	if (newSock == INVALID_SOCKET)
	{
		printf("accept err [%d]", WSAGetLastError());
		return false;
	}
	stPlayerInfo* newPlayer = new stPlayerInfo;
	newPlayer->bRemove = false;
	newPlayer->sock = newSock;
	newPlayer->id = g_id++;
	newPlayer->iX = 40;
	newPlayer->iY = 12;

	// id send
	int sendPacket[4];
	sendPacket[0] = 0;
	sendPacket[1] = newPlayer->id;
	SendUnicast(newPlayer, (char*)sendPacket);

	// 신규별에게 기존별 생성메시지 send
	list<stPlayerInfo*>::iterator iter;
	for (iter = g_PlayerList.begin(); iter != g_PlayerList.end(); ++iter)
	{
		sendPacket[0] = 1;
		sendPacket[1] = (*iter)->id;
		sendPacket[2] = (*iter)->iX;
		sendPacket[3] = (*iter)->iY;
		SendUnicast(newPlayer, (char*)sendPacket);
	}

	// 리스트에 신규별 추가
	g_PlayerList.push_back(newPlayer);
	
	// 모두에게 신규별 생성메시지 send
	sendPacket[0] = 1;
	sendPacket[1] = newPlayer->id;
	sendPacket[2] = newPlayer->iX;
	sendPacket[3] = newPlayer->iY;
	SendBroadCast(NULL, (char*)sendPacket);

	return true;
}


void RecvProcessor(stPlayerInfo* pPlayer)
{
	int retval;
	int Error;
	char buf[1600];
	retval = recv(pPlayer->sock, buf, sizeof(buf), 0);
	if (retval == 0)
		pPlayer->bRemove = true;
	if (retval == SOCKET_ERROR)
	{
		Error = WSAGetLastError();
		// 에러 더 추가하기 
		if(Error != WSAEWOULDBLOCK)
		{
			if (Error == 10054) // 클라가 x누를 때마다 여기로 분기
			{
				pPlayer->bRemove = true;
			}
			printf("what error : %d", Error);
		}
		return;
	}
	int ret = 0;
	while (retval >= 16)
	{
		int type = (int)*(buf + ret);
		int newID = (int)*(buf + ret + 4);
		int newX = (int)*(buf + ret + 8);
		int newY = (int)*(buf + ret + 12);
		int sendPacket[4];
		sendPacket[0] = type;
		sendPacket[1] = newID;
		sendPacket[2] = newX;
		sendPacket[3] = newY;

		ret += 16;
		retval -= 16;

		switch (type)
		{
			case 3:
			{
				pPlayer->iX = newX;
				pPlayer->iY = newY;
				SendBroadCast(pPlayer, (char*)sendPacket);
				break;
			}
			default:
			{
				__debugbreak;
			}
		}
	}

}

void Network()
{
	int retval;
	fd_set ReadSet;
	FD_ZERO(&ReadSet);
	FD_SET(g_listenSock, &ReadSet);
	list<stPlayerInfo*>::iterator iter;
	for (iter = g_PlayerList.begin(); iter != g_PlayerList.end(); ++iter)
		FD_SET((*iter)->sock, &ReadSet);
	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;
	retval = select(0, &ReadSet, NULL, NULL, NULL); // 타임아웃 설정 
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("Network() select error [%d]", WSAGetLastError());
			WSACleanup();
			return;
		}
	}
	if (retval > 0)
	{
		if (FD_ISSET(g_listenSock, &ReadSet))
		{
			if(!AcceptNewPlayer())
				printf("AcceptNewPlayer() Error");
		}
		list<stPlayerInfo*>::iterator iter;
		for (iter = g_PlayerList.begin(); iter != g_PlayerList.end(); ++iter)
		{
			if(FD_ISSET((*iter)->sock,&ReadSet))
				RecvProcessor(*iter);
		}
	}

	Disconnect();
}

void cs_Initial()
{
	CONSOLE_CURSOR_INFO stConsoleCursor;
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;			// 커서 크기
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
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
		g_map[iCnt][dfWIDTH - 1] = '\0';
	}
}

void Render()
{
	Buffer_Clear();
	list<stPlayerInfo*>::iterator iter = g_PlayerList.begin();
	for (; iter != g_PlayerList.end(); ++iter)
	{
		g_map[(*iter)->iY][(*iter)->iX] = '*';
	}
	Buffer_Flip();
	cs_MoveCursor(0, 0);
	printf("Server / Connect Client: %d", g_PlayerList.size());
}

int main()
{
	int retval;

	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket
	g_listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_listenSock == INVALID_SOCKET)
	{
		printf("socket err [%d]", WSAGetLastError());
		return 1;
	}
	// 링거 0
	LINGER ling;
	ling.l_onoff = 1;
	ling.l_linger = 0;
	setsockopt(g_listenSock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
	// bind
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(3000);
	retval = bind(g_listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		printf("bind err [%d]", WSAGetLastError());
		return 1;
	}
	// listen
	retval = listen(g_listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		printf("listen err [%d]", WSAGetLastError());
		return 1;
	}
	// ioctlsocket
	u_long on = 1;
	retval = ioctlsocket(g_listenSock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
	{
		printf("ioctlsocket err [%d]", WSAGetLastError());
		return 1;
	}

	cs_Initial();

	while (1)
	{
		Network();
		Render();
	}

	closesocket(g_listenSock);
	WSACleanup();
	return 0;
}