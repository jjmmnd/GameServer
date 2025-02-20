
#pragma comment(lib,"Winmm.lib")
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include"RingBuffer.h"

#define _MULTITHREAD_

constexpr const char* patternText = 
			"1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 123451234567890 abcdefghijklmnopqrstuvw 3nv1";
CRingBuffer g_RingBuffer;
char g_dequeueBuffer[2000] = { 0, };
char g_peekBuffer[2000] = { 0, };
bool g_bShutdown = false;

unsigned WINAPI EnqueueThread(LPVOID pParam);
unsigned WINAPI DequeueThread(LPVOID pParam);

// Todo
// Peek 이중 체크 해보기 


int main()
{
	// Initialize
	srand((unsigned)time(NULL));
	timeBeginPeriod(1);
	
#ifdef _MULTITHREAD_
	// 인큐와 디큐는 스레드 동기화 문제 x
	// Multi Thread Test START ---------------------------------------------
	HANDLE hThread[2];
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, EnqueueThread, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, DequeueThread, NULL, 0, NULL);

	while (1)
	{
		if (_kbhit())
		{
			char ch = _getch();
			if (ch == 'q' || ch == 'Q')
			{
				g_bShutdown = true;
				break;
			}
		}
	}

	int ret = WaitForMultipleObjects(2, hThread, true, INFINITE);
	if (ret == WAIT_FAILED)
		DebugBreak();
#endif


#ifndef _MULTITHREAD_
	char* ptr = (char*)patternText;
	CRingBuffer *tempBuffer = new CRingBuffer();
	char dequeueBuffer[2000] = { 0, };
	char peekBuffer[2000] = { 0, };
	char totalBuffer[121] = { 0, };
	int ret = 0;
	int fpsCnt = 0;
	bool flag = false;
	int aa;
	while (1)
	{
		
		int random = rand() % 120 + 1;
		int size = tempBuffer->Enqueue(ptr + ret, min((120 - ret), random));
		

		int vqvq = rand() % 1000 + 120;
		memset(peekBuffer, 0, vqvq);
		memset(dequeueBuffer, 0, vqvq);
		tempBuffer->Peek(peekBuffer, vqvq);
		int deqRet = tempBuffer->Dequeue(dequeueBuffer, vqvq);
		if (size != deqRet)
		{
			
			int* p = nullptr;
			*p = 0;
		}
		if (memcmp(dequeueBuffer, peekBuffer, deqRet) != 0)
		{
			int* p = nullptr;
			*p = 0;
		}
		printf("%s", dequeueBuffer);
		if (memcmp(dequeueBuffer, ptr + ret, deqRet) != 0)
		{
			int* p = nullptr;
			*p = 0;
		}
		
		if (strlen(dequeueBuffer) != deqRet)
		{
			aa = tempBuffer->GetUseSize();
			if (dequeueBuffer[size - 1] != '\0')
			{
				int* p = nullptr;
				*p = 0;
			}
		}
		ret += size;
		if (ret >= 120)
		{
			ret = 0;
			fpsCnt++;
			if ((fpsCnt % 5000) == 0)
				flag = true;
		}
		//Sleep(10);
	}
#endif



	return 0;
}


/*
unsigned int WINAPI EnqueueThread(LPVOID lpParam)
{
	char content[1000] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 123451234567890 abcdefghijklmnopqrstuvw 3nv1";
	while (1)
	{
		if (g_RingBuffer.GetFreeSize() >= strlen(content))
			g_RingBuffer.Enqueue(content, strlen(content));
		//Sleep(100);
	}
	return 0;
}


unsigned int WINAPI DequeueThread(LPVOID lpParam)
{
	int randomVal;
	int k = 0;
	char string[202];
	int i;
	int ret;
	int peek;
	while (1)
	{
		randomVal = rand() % 200;
		if (g_RingBuffer.GetUseSize() > 0)
		{

			ret = g_RingBuffer.Dequeue(string, randomVal);
			if (ret == -1)
				continue;
			string[ret] = 0;
			printf("%s", string);
		}
		//Sleep(100);
	}
	return 0;
}
*/

unsigned WINAPI EnqueueThread(LPVOID pParam)
{
	srand(GetCurrentThreadId());
	int random;
	int retval;

	const char* ptr = patternText;
	int idx = 0;
	while (!g_bShutdown)
	{
		random = rand() % 120 + 1;
		if (g_RingBuffer.GetFreeSize() >= random)
		{
			retval = g_RingBuffer.Enqueue((char*)(ptr + idx), min((120 - idx), random));
			idx += retval;
		}
		if (idx >= 120)
		{
			idx = 0;
		}
	}

	printf("\nEnqueue Thread is over.");
	return 0;
}

unsigned WINAPI DequeueThread(LPVOID pParam)
{
	srand(GetCurrentThreadId());
	int random;
	int retval;

	char checkBuffer[120];
	
	while (!g_bShutdown)
	{
		random = rand() % 1000;
		memset(g_dequeueBuffer, 0, sizeof(g_dequeueBuffer));
		memset(g_peekBuffer, 0, sizeof(g_peekBuffer));
		if (g_RingBuffer.GetUseSize() > 0)
		{
			retval = g_RingBuffer.Dequeue(g_dequeueBuffer, random);
			g_dequeueBuffer[retval] = 0;
			printf("%s", g_dequeueBuffer);
		}

	}

	printf("\nDequeue Thread is over.");
	return 0;
}
