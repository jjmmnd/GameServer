#pragma once

#include "Global_Struct.h"

#define ENEMY_COUNT 60
#define BULLET_MAX 50
#define dfSCREEN_WIDTH	81		// 콘솔 가로 80칸 + NULL
#define dfSCREEN_HEIGHT	24		// 콘솔 세로 25칸 + NULL


bool KeyProcess(void);					// 키보드입력을 받아서 플레이어를 처리
void CreateShot(bool, int, int, int);	// 총알 생성
void Shooting();						// 총알의 이동처리
void CheckConflict();					// 총알의 충돌처리
void EnemyAction();						// 적군의 이동 처리 및 공격 

extern st_PLAYER g_stPlayer;
extern st_Enemy g_stEnemy[ENEMY_COUNT];
extern st_Bullet g_stBullet[BULLET_MAX];