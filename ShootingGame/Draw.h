#pragma once

#include "Console.h"
#include "Global_Struct.h"

#define ENEMY_COUNT 60
#define BULLET_MAX 50

//--------------------------------------------------------------------
// ������ ������ ȭ������ ����ִ� �Լ�.
//
// ����,�Ʊ�,�Ѿ� ���� szScreenBuffer �� �־��ְ�, 
// 1 �������� ������ �������� �� �Լ��� ȣ���Ͽ� ���� -> ȭ�� ���� �׸���.
//--------------------------------------------------------------------
void Buffer_Flip(void);

//--------------------------------------------------------------------
// ȭ�� ���۸� �����ִ� �Լ�
//
// �� ������ �׸��� �׸��� ������ ���۸� ���� �ش�. 
// �ȱ׷��� ���� �������� �ܻ��� �����ϱ�
//--------------------------------------------------------------------
void Buffer_Clear(void);

//--------------------------------------------------------------------
// ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���.
//
// �Է� ���� X,Y ��ǥ�� �ƽ�Ű�ڵ� �ϳ��� ����Ѵ�. (���ۿ� �׸�)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, char chSprite);

void Draw();
void Draw_Player();
void DrawEnemy();
void DrawBullet();
void DrawHP();		// ������ HP UI, ���� ��ܿ� ǥ��
void DrawTitle();	// Ÿ��Ʋ ȭ�� �׸���
void DrawClear();	// Ŭ���� ȭ�� �׸���


// Ÿ��Ʋ �� Ŭ����� ����� ����
const char startString[14] = "1. Start Game";
const char exitString[13] = "2. Exit Game";
const char clearString[17] = "CLEAR ALL STAGE";

extern st_PLAYER g_stPlayer;
extern st_Enemy g_stEnemy[ENEMY_COUNT];
extern st_Bullet g_stBullet[BULLET_MAX];
