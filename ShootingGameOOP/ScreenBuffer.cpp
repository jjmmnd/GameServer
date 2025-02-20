
#include "ScreenBuffer.h"
#include <iostream>
#include <Windows.h>

HANDLE  hConsole;

CScreenBuffer::CScreenBuffer()
{
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memset(_screenBuffer[iCnt], ' ', dfSCREEN_WIDTH);
		_screenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}

	CONSOLE_CURSOR_INFO stConsoleCursor;

	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;			
		
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}

CScreenBuffer::~CScreenBuffer() { }

CScreenBuffer* CScreenBuffer::GetInstance()
{
	static CScreenBuffer screenBuffer;
	return &screenBuffer;
}

void CScreenBuffer::cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	SetConsoleCursorPosition(hConsole, stCoord);
}

void CScreenBuffer::cs_ClearScreen(void)
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
void CScreenBuffer::Buffer_Clear(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memset(_screenBuffer[iCnt], ' ', dfSCREEN_WIDTH);
		_screenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}
void CScreenBuffer::Buffer_Flip()
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		cs_MoveCursor(0, iCnt);
		printf(_screenBuffer[iCnt]);
	}
}
void CScreenBuffer::Sprite_Draw(int refX, int refY, char chSprite)
{
	if (refX < 0 || refY < 0 || refX >= dfSCREEN_WIDTH || refY >= dfSCREEN_HEIGHT)
		return;

	_screenBuffer[refY][refX] = chSprite;
}