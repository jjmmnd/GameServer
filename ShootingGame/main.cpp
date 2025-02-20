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
			// �������� ���� �� �ʿ��� �ʱ�ȭ �۾�
			// 1. ���� ��������(���� ��ġ) + g_stEnemy �ʱ�ȭ
			// 2. g_stBullet �ʱ�ȭ
			// 3. Player�� HP �ʱ�ȭ
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