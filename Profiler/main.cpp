#include <Windows.h>
#include "Profile.h"
#include "sample.h"
#include <iostream>
#include <conio.h>


int main()
{

	Initial();


	while (1)
	{
		/*
		// 입력부
		if (_kbhit())	// 키 입력이 있다면 1을 반환하고 없다면 0을 반환 
		{
			// 콘솔키를 빼기 위해 사용 
			// 어떤 키가 눌리기 전까지는 무한 대기 상태 
			// 키보드 입력받고 출력은 해주지 않는 함수 
			int iChar = _getch();

			if (iChar == 'p' || iChar == 'P')
				ProfileDataOutText(SetFileName());

			if (iChar == 'i' || iChar == 'I')
				ProfileReset();

		}*/
		if (GetAsyncKeyState(VK_SHIFT))
			DataOutText(SetFileName());

		// 로직
		MainLogic();

		// 랜더
		MainRander();
	}
}