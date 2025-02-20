#include <Windows.h>
#include "Action.h"

bool KeyProcess(void)
{
	// ���� ����Ű.
	if (GetAsyncKeyState(VK_LEFT))
	{
		g_stPlayer.iX -= 1;
	}
	// ������ ����Ű.
	if (GetAsyncKeyState(VK_RIGHT))
	{
		g_stPlayer.iX += 1;
	}
	// ���� ����Ű.
	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		g_stPlayer.iY--;
	}
	// �Ʒ��� ����Ű.
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		g_stPlayer.iY++;
	}


	g_stPlayer.iX = max(g_stPlayer.iX, 0);
	g_stPlayer.iX = min(g_stPlayer.iX, 79);
	g_stPlayer.iY = max(g_stPlayer.iY, 0);
	g_stPlayer.iY = min(g_stPlayer.iY, 23);



	if (GetAsyncKeyState(VK_CONTROL))
	{
		// �÷��̾��� ��ġ���� �Ѿ� �߻�
		CreateShot(true, g_stPlayer.iX, g_stPlayer.iY, 0);
	}


	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		return false;
	}

	return true;
}

/*
* �Ѿ� ����
* 1. �Ѿ� �迭���� ������ �ʾҴ� �Ѿ��� visible
* 2. ���ڷ� ���� ��ǥ�� �Ѿ� ��ǥ�� ����
* 3. ù ��° ���ڷ� true�� ������ player�� �Ѿ˷� ����
* 4. ������ ������ damage�� �°� ����
*/
void CreateShot(bool player, int posX, int posY, int damage)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!g_stBullet[i].visible)
		{
			if (player)
			{
				// �÷��̾� �Ѿ˷� ����
				g_stBullet[i].player = true;
				g_stBullet[i].damage = 0;
			}
			else if (!player)
			{
				// ���� �Ѿ˷� ����
				g_stBullet[i].player = false;
				g_stBullet[i].damage = damage;
			}
			g_stBullet[i].visible = true;
			g_stBullet[i].bulletX = posX;
			g_stBullet[i].bulletY = posY;
			return;
		}
	}
}


// �Ѿ��� �̵� ó��
// �÷��̾��� �Ѿ�(|)�̸� ����, ������ �Ѿ�(*)�̸� �Ʒ��� �̵��Ѵ�
void Shooting()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_stBullet[i].visible)
		{
			if (g_stBullet[i].player)
			{
				g_stBullet[i].bulletY--;
				if (g_stBullet[i].bulletY < 0)	// �Ѿ��� ȭ�� ������ ��Ż�� �� ����
				{
					g_stBullet[i].visible = false;
				}
			}
			else if (!g_stBullet[i].player)
			{
				g_stBullet[i].bulletY++;
				if (g_stBullet[i].bulletY >= dfSCREEN_HEIGHT)	// �Ѿ��� ȭ�� ������ ��Ż�� �� ����
				{
					g_stBullet[i].visible = false;
				}
			}

		}
	}
}

// �Ѿ��� �浹 ó�� 
// �÷��̾��� �Ѿ��̸� ������ ��� �迭�� ��
// ������ �Ѿ��̸� �÷��̾��� ��ǥ���� �� 
void CheckConflict()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_stBullet[i].visible)
		{
			if (g_stBullet[i].player)
			{
				for (int j = 0; j < ENEMY_COUNT; j++)
				{
					if (g_stEnemy[j].visible)
					{
						if ((g_stEnemy[j].enemyX == g_stBullet[i].bulletX) && (g_stEnemy[j].enemyY == g_stBullet[i].bulletY))
						{

							g_stBullet[i].visible = false;
							g_stEnemy[j].visible = false;
							break;
						}
					}
				}
			}
			else
			{
				if (g_stPlayer.iVisible)
				{
					if ((g_stBullet[i].bulletX == g_stPlayer.iX) && g_stBullet[i].bulletY == g_stPlayer.iY)
					{
						g_stPlayer.iHP -= g_stBullet[i].damage;
						g_stBullet[i].visible = false;
						if (g_stPlayer.iHP <= 0)
						{
							g_stPlayer.iVisible = false;
							g_stBullet[i].visible = false;
							break;
						}
					}
				}
			}
		}
	}
}


// ������ �̵�ó�� + ����
// ������ Ư�� ���� �������� �����̵��� ���� 
// �����ϰ� �Ѿ� �߻� => ������ ������ ���� �ʿ� 
void EnemyAction()
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (g_stEnemy[i].visible)
		{
			if (g_stEnemy[i].enemyX == 10 || g_stEnemy[i].enemyX == dfSCREEN_WIDTH / 2 || g_stEnemy[i].enemyX == dfSCREEN_WIDTH - 10)
				g_stEnemy[i].dirX = -g_stEnemy[i].dirX;
			if (g_stEnemy[i].enemyY == 1 || g_stEnemy[i].enemyY == dfSCREEN_HEIGHT / 2)
				g_stEnemy[i].dirY = -g_stEnemy[i].dirY;

			g_stEnemy[i].enemyX += g_stEnemy[i].dirX;
			g_stEnemy[i].enemyY += g_stEnemy[i].dirY;

			if (rand() % 100 < 5)
				CreateShot(false, g_stEnemy[i].enemyX, g_stEnemy[i].enemyY, g_stEnemy[i].damage);
		}
	}

}
