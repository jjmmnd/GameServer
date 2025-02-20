#pragma once

#include "Global_Struct.h"

#define ENEMY_COUNT 60
#define BULLET_MAX 50
#define dfSCREEN_WIDTH	81		// �ܼ� ���� 80ĭ + NULL
#define dfSCREEN_HEIGHT	24		// �ܼ� ���� 25ĭ + NULL


bool KeyProcess(void);					// Ű�����Է��� �޾Ƽ� �÷��̾ ó��
void CreateShot(bool, int, int, int);	// �Ѿ� ����
void Shooting();						// �Ѿ��� �̵�ó��
void CheckConflict();					// �Ѿ��� �浹ó��
void EnemyAction();						// ������ �̵� ó�� �� ���� 

extern st_PLAYER g_stPlayer;
extern st_Enemy g_stEnemy[ENEMY_COUNT];
extern st_Bullet g_stBullet[BULLET_MAX];