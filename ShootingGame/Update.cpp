#include <stdio.h>
#include <Windows.h>

#include "Update.h"


// Ÿ��Ʋ �Լ�
void UpdateTitle()
{
	g_stage = 0;

	while (g_stage == 0)
	{
		// Ű���� �Էº�
		// ���� 1 ������ ���� Start
		if (GetAsyncKeyState(VK_NUMPAD1) || GetAsyncKeyState(0x31))
		{
			g_stage++;
			g_scene = LOAD;
			return;

		}
		// ���� 2 ������ ���� Exit
		if (GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState(0x32))
		{
			g_scene = OVER;
			return;
		}


		// ������
		DrawTitle();
	}
}


// ���� �Լ�
void UpdateGame()
{
	// Ű���� �Է�
	// esc�� ������ OVER�� �̵�
	if (!KeyProcess())
	{
		g_scene = OVER;
		return;
	}

	// ������
	EnemyAction();
	Shooting();
	CheckConflict();

	// �������� Ŭ���� üũ
	if (CheckClearGame())
	{
		g_stage++;
		
		// ���� ���������� ��ü ���������� �������� Ŀ���� ���� Ŭ����
		if (g_stage == g_stageCnt + 1)
		{
			g_scene = CLEAR;
			return;
		}

		g_scene = LOAD;
		return;
	}

	// ���� ���� üũ
	if (CheckGameOver())
	{
		g_scene = OVER;
		return;
	}

	// ������
	Draw();

	// ������ �ӵ� ����
	Sleep(100);
}


void UpdateClear(bool* flag)
{
	// �����Ҵ� ���� ���� ����ü�� free
	if (st_stageInfo != NULL)
		free(st_stageInfo);
	if (st_enemyInfo != NULL)
		free(st_enemyInfo);
	if (st_enemyType != NULL)
		free(st_enemyType);
	if (g_stageBuffer != NULL)
		free(g_stageBuffer);

	*flag = false;

	// ���� Ŭ���� ���� �� ����
	DrawClear();

	return;
}

void UpdateOver(bool* flag)
{
	// �����Ҵ� ���� ���� ����ü�� free
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

// ���������� Ŭ�����ϸ� true
bool CheckClearGame()
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (g_stEnemy[i].visible)
		{
			// ����ִ� ���� �ִٸ�  ����
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