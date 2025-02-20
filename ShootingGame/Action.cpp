#include <Windows.h>
#include "Action.h"

bool KeyProcess(void)
{
	// 왼쪽 방향키.
	if (GetAsyncKeyState(VK_LEFT))
	{
		g_stPlayer.iX -= 1;
	}
	// 오른쪽 방향키.
	if (GetAsyncKeyState(VK_RIGHT))
	{
		g_stPlayer.iX += 1;
	}
	// 위쪽 방향키.
	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		g_stPlayer.iY--;
	}
	// 아래쪽 방향키.
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
		// 플레이어의 위치에서 총알 발사
		CreateShot(true, g_stPlayer.iX, g_stPlayer.iY, 0);
	}


	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		return false;
	}

	return true;
}

/*
* 총알 생성
* 1. 총알 배열에서 사용되지 않았던 총알을 visible
* 2. 인자로 받은 좌표로 총알 좌표를 세팅
* 3. 첫 번째 인자로 true가 들어오면 player의 총알로 생성
* 4. 마지막 인자의 damage에 맞게 설정
*/
void CreateShot(bool player, int posX, int posY, int damage)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (!g_stBullet[i].visible)
		{
			if (player)
			{
				// 플레이어 총알로 세팅
				g_stBullet[i].player = true;
				g_stBullet[i].damage = 0;
			}
			else if (!player)
			{
				// 적군 총알로 세팅
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


// 총알의 이동 처리
// 플레이어의 총알(|)이면 위로, 적군의 총알(*)이면 아래로 이동한다
void Shooting()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_stBullet[i].visible)
		{
			if (g_stBullet[i].player)
			{
				g_stBullet[i].bulletY--;
				if (g_stBullet[i].bulletY < 0)	// 총알이 화면 밖으로 이탈할 시 삭제
				{
					g_stBullet[i].visible = false;
				}
			}
			else if (!g_stBullet[i].player)
			{
				g_stBullet[i].bulletY++;
				if (g_stBullet[i].bulletY >= dfSCREEN_HEIGHT)	// 총알이 화면 밖으로 이탈할 시 삭제
				{
					g_stBullet[i].visible = false;
				}
			}

		}
	}
}

// 총알의 충돌 처리 
// 플레이어의 총알이면 적군의 모든 배열과 비교
// 적군의 총알이면 플레이어의 좌표랑만 비교 
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


// 적군의 이동처리 + 공격
// 적군의 특정 범위 내에서만 움직이도록 제한 
// 랜덤하게 총알 발사 => 프레임 단위로 수정 필요 
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
