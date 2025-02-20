#include <Windows.h>
#include "Profile.h"
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;
//---------------------------------
// 초기화 함수 정의 
//---------------------------------
LARGE_INTEGER Freq;
char* fileName;

void Initial()
{
	QueryPerformanceFrequency(&Freq);
}

const char* SetFileName()
{
	fileName = (char*)malloc(128);
	if (fileName == NULL)
	{
		return NULL;
	}
	struct tm newTime;
	__time64_t longTime;

	_time64(&longTime); // 현재 시간 64bit으로 시간 받아서
	errno_t err = _localtime64_s(&newTime, &longTime);
	if (err)
	{
		printf("Invalid argument to _localtime64_s.");
		return NULL;
	}

	sprintf_s(fileName, 128, "%d%d%d_%d_%d_%d.txt",
		newTime.tm_year + 1900, newTime.tm_mon + 1, newTime.tm_mday,
		newTime.tm_hour, newTime.tm_min, newTime.tm_sec);

	return fileName;
}

//---------------------------------
// 전역 배열 
//---------------------------------
stPROFILE g_profile[10];


//---------------------------------
// 멤버 함수 정의 
//---------------------------------
Profile::Profile(const char* tag)
{
	Begin(tag);
	_tag = tag;
}

Profile::~Profile()
{
	End(_tag);
}


// tag 단위로 이루어져야함 
void Begin(const char* tag)
{
	LARGE_INTEGER Start;
	QueryPerformanceCounter(&Start);


	// 두 번 이상 함수 호출인 경우
	for (int i = 0; i < 10; i++)
	{
		if (!strcmp(tag, g_profile[i].szName))	// 동일한 태그가 있다면
		{
			if (g_profile[i].lFlag == static_cast<int>(callType::END))		// End 호출이 진행되었다면
			{
				g_profile[i].lFlag = static_cast<int>(callType::START);
				g_profile[i].lStartTime = Start;
				++g_profile[i].iCall;
				return;
			}
			else // End 호출 없이 옴 
				__debugbreak();
		}
	}


	// 처음 함수 호출인 경우
	for (int i = 0; i < 10; i++)
	{
		if (g_profile[i].lFlag == 0)	// 배열에 남은 공간이 있으면
		{
			g_profile[i].lFlag = static_cast<int>(callType::START);
			strcpy_s(g_profile[i].szName, tag);
			g_profile[i].lStartTime = Start;
			++g_profile[i].iCall;
			g_profile[i].iMax[0] = 0;
			g_profile[i].iMax[1] = 0;
			__int64 longMax = 1;
			g_profile[i].iMin[0] = ~(longMax << 63); // 최대크기로 초기화
			g_profile[i].iMin[1] = ~(longMax << 63);
			return;
		}
	}
	// 전역 배열이 꽉 찬 경우
	cout << "g_profile is full\n";
	return;

}

void End(const char* tag)
{
	LARGE_INTEGER End;
	QueryPerformanceCounter(&End);
	__int64 curTime;


	// 먼저 태그가 전역 배열에 존재하는지 한번 돌고
	// 존재하지 않는다면 에러 뱉도록 
	for (int i = 0; i < 10; i++)
	{
		if (!strcmp(g_profile[i].szName, tag))	// 전역 배열에 존재한다면  확인 후 정보 갱신
		{
			if (g_profile[i].lFlag == static_cast<int>(callType::START))
			{
				g_profile[i].lFlag = static_cast<int>(callType::END);
				curTime = (End.QuadPart - g_profile[i].lStartTime.QuadPart);
				g_profile[i].iTotalTime += curTime;

				// min, max 갱신
				if (curTime < g_profile[i].iMin[1])
				{
					if (curTime < g_profile[i].iMin[0])
					{
						g_profile[i].iMin[1] = g_profile[i].iMin[0];
						g_profile[i].iMin[0] = curTime;
						return;
					}
					g_profile[i].iMin[1] = curTime;
					return;
				}
				else if (curTime > g_profile[i].iMax[1])
				{
					if (curTime > g_profile[i].iMax[0])
					{
						g_profile[i].iMax[1] = g_profile[i].iMax[0];
						g_profile[i].iMax[0] = curTime;
						return;
					}
					g_profile[i].iMax[1] = curTime;
					return;
				}
				// min, max는 변화 없을 경우 그냥 리턴
				return;
			}
			else // 전역 배열에 tag가 존재하지만 Begin 호출 없이 옴
				__debugbreak();

		}
	}
}


void DataOutText(const char* szFileName)
{
	FILE* fPtr;
	fopen_s(&fPtr, szFileName, "w");
	if (fPtr == NULL)
	{
		cout << "File Open fail\n";
		return;
	}

	fprintf(fPtr, "---------------------------------------------------------------------------------\n");
	fprintf(fPtr, "Name\t\t |\tAverage\t|\tMin\t|\tMax\t|\tCall\t|\n");
	fprintf(fPtr, "---------------------------------------------------------------------------------\n");

	for (int i = 0; i < 10; i++)
	{
		if (g_profile[i].lFlag != 0)
		{
			g_profile[i].iTotalTime -= g_profile[i].iMin[0];
			g_profile[i].iTotalTime -= g_profile[i].iMin[1];
			g_profile[i].iTotalTime -= g_profile[i].iMax[0];
			g_profile[i].iTotalTime -= g_profile[i].iMax[1];
			g_profile[i].iCall = max(g_profile[i].iCall - 4, 0);
			double average = (double)(g_profile[i].iTotalTime * 1000000 / Freq.QuadPart) / g_profile[i].iCall;
			double min = (double)(g_profile[i].iMin[0]) * 1000000 / Freq.QuadPart;
			double max = (double)(g_profile[i].iMax[0]) * 1000000 / Freq.QuadPart;
			fprintf(fPtr, "%s\t|\t%0.4lfμs |\t%0.4lfμs |\t%0.4lfμs |\t%lld \n", g_profile[i].szName, average, min, max, g_profile[i].iCall);
		}

	}
	fclose(fPtr);

	free(fileName);
}


void ProfileReset()
{
	for (int i = 0; i < 10; i++)
	{
		if (g_profile[i].lFlag != 0)
		{
			g_profile[i].iTotalTime ^= g_profile[i].iTotalTime;
			__int64 longMax = 1;
			g_profile[i].iMin[0] = ~(longMax << 63); // 최대크기로 초기화
			g_profile[i].iMin[1] = ~(longMax << 63);
			g_profile[i].iMax[0] ^= g_profile[i].iMax[0]; // 0으로 초기화 
			g_profile[i].iMax[1] ^= g_profile[i].iMax[1];
			g_profile[i].iCall ^= g_profile[i].iCall;
		}
	}
}