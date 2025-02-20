#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "Draw.h"


char szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];


void Buffer_Flip(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		cs_MoveCursor(0, iCnt);
		printf(szScreenBuffer[iCnt]);
	}
}

void Buffer_Clear(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memset(szScreenBuffer[iCnt], ' ', dfSCREEN_WIDTH);
		szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

void Sprite_Draw(int iX, int iY, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	szScreenBuffer[iY][iX] = chSprite;
}


void Draw()
{
	Buffer_Clear();
	Draw_Player();
	DrawEnemy();
	DrawBullet();
	DrawHP();
	Buffer_Flip();
}


// 버퍼에 플레이어 그리기
void Draw_Player(void)
{
	if (g_stPlayer.iVisible)
		Sprite_Draw(g_stPlayer.iX, g_stPlayer.iY, '#');

}


// 버퍼에 적군 그리기
void DrawEnemy()
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		// 적군이 살아있다면 랜더링 
		if (g_stEnemy[i].visible)
			Sprite_Draw(g_stEnemy[i].enemyX, g_stEnemy[i].enemyY, g_stEnemy[i].type);
	}
}


// 발사된 총알을 버퍼에 그리기
void DrawBullet()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_stBullet[i].visible)
		{
			int bX = g_stBullet[i].bulletX;
			int bY = g_stBullet[i].bulletY;
			if ((bX >= 0 && bX < dfSCREEN_WIDTH - 1) && (bY > 0 && bY < dfSCREEN_HEIGHT))
				if (g_stBullet[i].player)
				{
					Sprite_Draw(g_stBullet[i].bulletX, g_stBullet[i].bulletY, '|');
				}
				else if (!g_stBullet[i].player)
				{
					Sprite_Draw(g_stBullet[i].bulletX, g_stBullet[i].bulletY, '*');
				}
		}
	}
}

// 간단한 HP UI
// 우측 상단에 표기된다
void DrawHP()
{
	int x = dfSCREEN_WIDTH - 4;
	int y = 1;

	for (int i = 1; i <= g_stPlayer.iHP; i++)
	{
		Sprite_Draw(x, y, '0');
		x--;
	}
}


void DrawTitle()
{
	Buffer_Clear();

	int x = 35;
	int y = 15;
	for (int i = 0; i < strlen(startString); i++)
	{
		Sprite_Draw(x, y, startString[i]);
		x++;
	}

	x = 35;
	y = 16;
	for (int i = 0; i < strlen(exitString); i++)
	{
		Sprite_Draw(x, y, exitString[i]);
		x++;
	}

	Buffer_Flip();
}

void DrawClear()
{
	Buffer_Clear();

	int x = 40;
	int y = 11;
	for (int i = 0; i < strlen(clearString); i++)
	{
		Sprite_Draw(x, y, clearString[i]);
		x++;
	}

	Buffer_Flip();
}