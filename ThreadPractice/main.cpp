// ThreadPractice.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>
#include <process.h>
#pragma comment(lib,"Winmm.lib")

constexpr int Thread_NUM = 5;

//CRITICAL_SECTION g_DataCS;
SRWLOCK g_srwLock;
long g_Data = 0;
volatile long g_Connect = 0;
bool g_Shutdown = false;

unsigned WINAPI AcceptThread(void* pParam)
{
//	srand(GetCurrentThreadId());
	int seed;
	srand((int)&seed);
	while (!g_Shutdown)
	{
		InterlockedAdd(&g_Connect, 1);
		Sleep(rand() % 901 + 100);
	}
	return 0;
}

unsigned WINAPI DisconnectThread(void* pParam)
{
//	srand(GetCurrentThreadId());
	int seed;
	srand((int)&seed);
	while (!g_Shutdown)
	{
		
		InterlockedAdd(&g_Connect, -1);
		Sleep(rand() % 901 + 100);
	}
	return 0;
}

unsigned WINAPI UpdateThread(void* pParam)
{
	while (!g_Shutdown)
	{
		/*AcquireSRWLockExclusive(&g_srwLock);
		++g_Data;
		if (g_Data % 1000 == 0)
			printf("g_Data: %ld\n", g_Data);
		ReleaseSRWLockExclusive(&g_srwLock);*/

		// 증가한 결과값을 리턴함
		if (InterlockedIncrement(&g_Data) % 1000 == 0)
			printf("g_Data: %ld\n", g_Data);

		Sleep(10);
	}
	return 0;
}


int main()
{
	timeBeginPeriod(1);
	InitializeSRWLock(&g_srwLock);
	
	HANDLE hThread[Thread_NUM];
	for (int i = 0; i < 3; i++)
	{
		hThread[i] = (HANDLE)_beginthreadex(nullptr, 0, UpdateThread, nullptr, CREATE_SUSPENDED, nullptr);
	}
	hThread[Thread_NUM-2] = (HANDLE)_beginthreadex(nullptr, 0, AcceptThread, nullptr, CREATE_SUSPENDED, nullptr);
	hThread[Thread_NUM - 1] = (HANDLE)_beginthreadex(nullptr, 0, DisconnectThread, nullptr, CREATE_SUSPENDED, nullptr);
	for (int i = 0; i < Thread_NUM; i++)
	{
		ResumeThread(hThread[i]);
	}

	DWORD oldTime = timeGetTime();
	while (timeGetTime() - oldTime < 20000)
	{
		Sleep(1000);
		printf("g_Connect: %ld\n", g_Connect);
	}
	g_Shutdown = true;
	
	WaitForMultipleObjects(Thread_NUM, hThread, true, INFINITE);
	
	for (int i = 0; i < Thread_NUM; i++)
	{
		CloseHandle(hThread[i]);
	}
	
	timeEndPeriod(1);
	return 0;
}

/*
unsigned WINAPI UpdateThread(void* pParam)
{
	while (!g_Shutdown)
	{
		EnterCriticalSection(&g_DataCS);
		++g_Data;
		if (g_Data % 1000 == 0)
			printf("g_Data: %ld\n", g_Data);
		LeaveCriticalSection(&g_DataCS);
		Sleep(10);
	}
	return 0;
}*/