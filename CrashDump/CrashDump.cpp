
#include <iostream>

#include "CrashDump.h"

int main()
{
	CCrashDump dump;

	int checkCnt = 0;
	while (1)
	{
		checkCnt++;
		printf("d");
		if (checkCnt >= 1000)
		{
			dump.Crash();

		}
	}
	return 0;
}
