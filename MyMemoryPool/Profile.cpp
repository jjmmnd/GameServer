#include <Windows.h>
#include "Profile.h"
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;
//---------------------------------
// �ʱ�ȭ �Լ� ���� 
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

	_time64(&longTime); // ���� �ð� 64bit���� �ð� �޾Ƽ�
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
// ���� �迭 
//---------------------------------
stPROFILE g_profile[10];


//---------------------------------
// ��� �Լ� ���� 
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


// tag ������ �̷�������� 
void Begin(const char* tag)
{
	LARGE_INTEGER Start;
	QueryPerformanceCounter(&Start);


	// �� �� �̻� �Լ� ȣ���� ���
	for (int i = 0; i < 10; i++)
	{
		if (!strcmp(tag, g_profile[i].szName))	// ������ �±װ� �ִٸ�
		{
			if (g_profile[i].lFlag == static_cast<int>(callType::END))		// End ȣ���� ����Ǿ��ٸ�
			{
				g_profile[i].lFlag = static_cast<int>(callType::START);
				g_profile[i].lStartTime = Start;
				++g_profile[i].iCall;
				return;
			}
			else // End ȣ�� ���� �� 
				__debugbreak();
		}
	}


	// ó�� �Լ� ȣ���� ���
	for (int i = 0; i < 10; i++)
	{
		if (g_profile[i].lFlag == 0)	// �迭�� ���� ������ ������
		{
			g_profile[i].lFlag = static_cast<int>(callType::START);
			strcpy_s(g_profile[i].szName, tag);
			g_profile[i].lStartTime = Start;
			++g_profile[i].iCall;
			g_profile[i].iMax[0] = 0;
			g_profile[i].iMax[1] = 0;
			__int64 longMax = 1;
			g_profile[i].iMin[0] = ~(longMax << 63); // �ִ�ũ��� �ʱ�ȭ
			g_profile[i].iMin[1] = ~(longMax << 63);
			return;
		}
	}
	// ���� �迭�� �� �� ���
	cout << "g_profile is full\n";
	return;

}

void End(const char* tag)
{
	LARGE_INTEGER End;
	QueryPerformanceCounter(&End);
	__int64 curTime;


	// ���� �±װ� ���� �迭�� �����ϴ��� �ѹ� ����
	// �������� �ʴ´ٸ� ���� �񵵷� 
	for (int i = 0; i < 10; i++)
	{
		if (!strcmp(g_profile[i].szName, tag))	// ���� �迭�� �����Ѵٸ�  Ȯ�� �� ���� ����
		{
			if (g_profile[i].lFlag == static_cast<int>(callType::START))
			{
				g_profile[i].lFlag = static_cast<int>(callType::END);
				curTime = (End.QuadPart - g_profile[i].lStartTime.QuadPart);
				g_profile[i].iTotalTime += curTime;

				// min, max ����
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
				// min, max�� ��ȭ ���� ��� �׳� ����
				return;
			}
			else // ���� �迭�� tag�� ���������� Begin ȣ�� ���� ��
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
			fprintf(fPtr, "%s\t|\t%0.4lf��s |\t%0.4lf��s |\t%0.4lf��s |\t%lld \n", g_profile[i].szName, average, min, max, g_profile[i].iCall);
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
			g_profile[i].iMin[0] = ~(longMax << 63); // �ִ�ũ��� �ʱ�ȭ
			g_profile[i].iMin[1] = ~(longMax << 63);
			g_profile[i].iMax[0] ^= g_profile[i].iMax[0]; // 0���� �ʱ�ȭ 
			g_profile[i].iMax[1] ^= g_profile[i].iMax[1];
			g_profile[i].iCall ^= g_profile[i].iCall;
		}
	}
}