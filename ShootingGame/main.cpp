#include "Initial.h"
#include "Update.h"

int main(void)
{
	cs_Initial();

	Initial();

	g_scene = TITLE;

	bool flag = true;

	while (1)
	{
		if (!flag)
			break;

		switch (g_scene)
		{
		case TITLE:
			UpdateTitle();
			break;

		case LOAD:
			//-------------------------------------------------
			// 스테이지 시작 전 필요한 초기화 작업
			// 1. 게임 스테이지(적군 배치) + g_stEnemy 초기화
			// 2. g_stBullet 초기화
			// 3. Player의 HP 초기화
			//-------------------------------------------------
			ReadStage();
			InitBullet();
			InitHP();
			g_scene = GAME;
			break;

		case GAME:
			UpdateGame();
			break;

		case CLEAR:
			UpdateClear(&flag);
			break;

		case OVER:
			UpdateOver(&flag);
			break;
		}
	}
	return 0;
}