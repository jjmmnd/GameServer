// DomainToIP.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <stdlib.h>
#include <WS2tcpip.h>

BOOL DomainToIP(const WCHAR* szDomain, IN_ADDR* pAddr)
{
    ADDRINFOW* pAddrInfo; // 링크드 리스트 구조 
    SOCKADDR_IN* pSockAddr;
    if (GetAddrInfo(szDomain, L"0", NULL, &pAddrInfo) != 0)
    {
        std::cout << WSAGetLastError();
        return FALSE;
    }
    int i = 0;
    while (pAddrInfo->ai_next != NULL)
    {
        pSockAddr = (SOCKADDR_IN*)pAddrInfo->ai_addr;
        *(pAddr + i) = pSockAddr->sin_addr;
        pAddrInfo = pAddrInfo->ai_next;
        i++;

        FreeAddrInfo(pAddrInfo);
        return TRUE;
    }
}

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "FALSE";
        return 0;
    }


    SOCKADDR_IN SockAddr;
    IN_ADDR* Addr = NULL;
    memset(&SockAddr, 0, sizeof(SockAddr));
    
    DomainToIP(L"naver.com", Addr);

    for (int i = 0; i < 10; i++)
    {
        std::cout << Addr[i].S_un.S_addr << std::endl;
    }

    WSACleanup();

    return 0;
}
