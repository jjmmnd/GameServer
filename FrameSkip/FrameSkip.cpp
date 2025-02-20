#include <iostream>
#include <Windows.h>

#pragma comment(lib,"winmm.lib")

int fps = 0;
int rander = 0;

void Logic()
{
	fps++;
}


void Rander()
{
	rander++;
	int delay = 0;
	if (GetAsyncKeyState(VK_SPACE))
	{
		delay += 100;
	}
	Sleep(delay);
}

DWORD beforeTick;

bool Check()
{
	DWORD curTick = timeGetTime();

	if (curTick - beforeTick <= 20)
	{
		Sleep(20 - curTick + beforeTick);
		beforeTick += 20;
		return true;
	}
	else if (curTick - beforeTick > 20 && curTick - beforeTick < 40)
	{
		beforeTick += 20;
		return true;
	}
	else if (curTick - beforeTick >= 40)
	{
		beforeTick += 20;
		return false;
	}
}


int main()
{
	timeBeginPeriod(1);
	beforeTick = timeGetTime();
	DWORD frameTick = beforeTick;
	
	// 1초에 50프레임으로 
	while (1)
	{
		Logic();

		if (Check()) // 랜더를 스킵할지 말지 체크 
		{
			Rander();
		}

		if (timeGetTime() - frameTick >= 1000)
		{
			printf("FPS:%d Rander:%d\n", fps, rander);
			fps = 0;
			rander = 0;
			frameTick += 1000;
		}
	}
}