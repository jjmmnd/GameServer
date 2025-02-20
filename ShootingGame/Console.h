
#ifndef __CONSOLE__
#define __CONSOLE__

#define dfSCREEN_WIDTH	81		// 콘솔 가로 80칸 + NULL
#define dfSCREEN_HEIGHT	24		// 콘솔 세로 25칸 + NULL

void cs_Initial(void);

void cs_MoveCursor(int iPosX, int iPosY);


void cs_ClearScreen(void);


#endif

#pragma once
