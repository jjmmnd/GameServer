// 24.07.08

#include <iostream>
#include <Windows.h>
#include <process.h>
#include "SpinLock.h"

volatile unsigned long long g_Flag = 0;
int g_Resource = 0;

unsigned int _stdcall ThreadFunc(void* pParam)
{
	SpinLock spinLock;
	printf("ID: %d #START\n", GetCurrentThreadId());
	spinLock.Lock(&g_Flag);
	++g_Resource;
	printf("%d\n", g_Resource);
	spinLock.UnLock(&g_Flag);
	printf("ID: %d #END\n", GetCurrentThreadId());
	return 0;
}

int main()
{
	int param = 0;
	HANDLE hThread[10];
	for (int i = 0; i < 10; ++i)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)param, CREATE_SUSPENDED, NULL);
	}

	for (int i = 0; i < 10; ++i)
	{
		ResumeThread(hThread[i]);
	}

	WaitForMultipleObjects(10, hThread, true, INFINITE);

	printf("g_Resource: %d\n", g_Resource);
	return 0;
}
