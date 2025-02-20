#include <windows.h>
#include <stdio.h>
#include "Console.h"

HANDLE  hConsole;

void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;			// 커서 크기.
											// 이상하게도 0 이면 나온다. 1로하면 안나온다.


	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}

void cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	SetConsoleCursorPosition(hConsole, stCoord);
}

void cs_ClearScreen(void)
{
	int iCountX, iCountY;

	for (iCountY = 0; iCountY < dfSCREEN_HEIGHT; iCountY++)
	{
		for (iCountX = 0; iCountX < dfSCREEN_WIDTH; iCountX++)
		{
			cs_MoveCursor(iCountX, iCountY);
			printf(" ");
		}
	}
}
