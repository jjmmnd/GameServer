

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Synchronization.lib")
#include <iostream>
#include <Windows.h>
#include <process.h>

int g_Data = 0;

long g_Lock = 0; // 0이면 미사용 1이면 사용 


void Lock(volatile unsigned long long* pTarget, void* pCompare)
{
	while (InterlockedExchange(pTarget, 1) == 1)
	{
		WaitOnAddress(pTarget, pCompare, 4, INFINITE); // 1이면 블락 
	}
}
void Unlock(volatile unsigned long long* pTarget)
{
	InterlockedExchange(pTarget, 0);
	WakeByAddressSingle((void*)pTarget);
}

unsigned int _stdcall ThreadFunc(void* pParam)
{
	int compare = 1;

	for(int i=0;i<10;i++)
	{
		while (InterlockedExchange(&g_Lock, 1) == 1)
		{
			WaitOnAddress(&g_Lock, &compare, 8, INFINITE); // 1이면 블락 
		}

		g_Data++;
		
		InterlockedExchange(&g_Lock, 0);
		WakeByAddressSingle(&g_Lock);
	}

	return 0;
}


int main()
{
	timeBeginPeriod(1);
	HANDLE hThread[5];
	for (int i = 0; i < 5; ++i)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, NULL, CREATE_SUSPENDED, NULL);
	}
	for (int i = 0; i < 5; ++i)
	{
		ResumeThread(hThread[i]);
	}

	WaitForMultipleObjects(5, hThread, true, INFINITE);

	printf("%d\n", g_Data);
	timeEndPeriod(1);
	return 0;
}

