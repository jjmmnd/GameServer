#include <stdio.h>
#include <Windows.h>

#include "Update.h"


// 타이틀 함수
void UpdateTitle()
{
	g_stage = 0;

	while (g_stage == 0)
	{
		// 키보드 입력부
		// 숫자 1 누르면 게임 Start
		if (GetAsyncKeyState(VK_NUMPAD1) || GetAsyncKeyState(0x31))
		{
			g_stage++;
			g_scene = LOAD;
			return;

		}
		// 숫자 2 누르면 게임 Exit
		if (GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState(0x32))
		{
			g_scene = OVER;
			return;
		}


		// 랜더링
		DrawTitle();
	}
}


// 게임 함수
void UpdateGame()
{
	// 키보드 입력
	// esc를 누르면 OVER로 이동
	if (!KeyProcess())
	{
		g_scene = OVER;
		return;
	}

	// 로직부
	EnemyAction();
	Shooting();
	CheckConflict();

	// 스테이지 클리어 체크
	if (CheckClearGame())
	{
		g_stage++;
		
		// 현재 스테이지가 전체 스테이지의 개수보다 커지면 최종 클리어
		if (g_stage == g_stageCnt + 1)
		{
			g_scene = CLEAR;
			return;
		}

		g_scene = LOAD;
		return;
	}

	// 게임 오버 체크
	if (CheckGameOver())
	{
		g_scene = OVER;
		return;
	}

	// 랜더링
	Draw();

	// 프레임 속도 조절
	Sleep(100);
}


void UpdateClear(bool* flag)
{
	// 동적할당 받은 전역 구조체들 free
	if (st_stageInfo != NULL)
		free(st_stageInfo);
	if (st_enemyInfo != NULL)
		free(st_enemyInfo);
	if (st_enemyType != NULL)
		free(st_enemyType);
	if (g_stageBuffer != NULL)
		free(g_stageBuffer);

	*flag = false;

	// 게임 클리어 랜더 후 종료
	DrawClear();

	return;
}

void UpdateOver(bool* flag)
{
	// 동적할당 받은 전역 구조체들 free
	if (st_stageInfo != NULL)
		free(st_stageInfo);
	if (st_enemyInfo != NULL)
		free(st_enemyInfo);
	if (st_enemyType != NULL)
		free(st_enemyType);
	if (g_stageBuffer != NULL)
		free(g_stageBuffer);

	*flag = false;

	return;
}

// 스테이지를 클리어하면 true
bool CheckClearGame()
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (g_stEnemy[i].visible)
		{
			// 살아있는 적이 있다면  리턴
			return false;
		}
	}

	return true;
}

bool CheckGameOver()
{
	if (!g_stPlayer.iVisible)
	{
		return true;
	}
	return false;
}