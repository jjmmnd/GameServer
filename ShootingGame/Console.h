
#ifndef __CONSOLE__
#define __CONSOLE__

#define dfSCREEN_WIDTH	81		// �ܼ� ���� 80ĭ + NULL
#define dfSCREEN_HEIGHT	24		// �ܼ� ���� 25ĭ + NULL

void cs_Initial(void);

void cs_MoveCursor(int iPosX, int iPosY);


void cs_ClearScreen(void);


#endif

#pragma once
