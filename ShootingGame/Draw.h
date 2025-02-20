#pragma once

#include "Console.h"
#include "Global_Struct.h"

#define ENEMY_COUNT 60
#define BULLET_MAX 50

//--------------------------------------------------------------------
// 버퍼의 내용을 화면으로 찍어주는 함수.
//
// 적군,아군,총알 등을 szScreenBuffer 에 넣어주고, 
// 1 프레임이 끝나는 마지막에 본 함수를 호출하여 버퍼 -> 화면 으로 그린다.
//--------------------------------------------------------------------
void Buffer_Flip(void);

//--------------------------------------------------------------------
// 화면 버퍼를 지워주는 함수
//
// 매 프레임 그림을 그리기 직전에 버퍼를 지워 준다. 
// 안그러면 이전 프레임의 잔상이 남으니까
//--------------------------------------------------------------------
void Buffer_Clear(void);

//--------------------------------------------------------------------
// 버퍼의 특정 위치에 원하는 문자를 출력.
//
// 입력 받은 X,Y 좌표에 아스키코드 하나를 출력한다. (버퍼에 그림)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, char chSprite);

void Draw();
void Draw_Player();
void DrawEnemy();
void DrawBullet();
void DrawHP();		// 간단한 HP UI, 우측 상단에 표기
void DrawTitle();	// 타이틀 화면 그리기
void DrawClear();	// 클리어 화면 그리기


// 타이틀 밑 클리어에서 사용할 문구
const char startString[14] = "1. Start Game";
const char exitString[13] = "2. Exit Game";
const char clearString[17] = "CLEAR ALL STAGE";

extern st_PLAYER g_stPlayer;
extern st_Enemy g_stEnemy[ENEMY_COUNT];
extern st_Bullet g_stBullet[BULLET_MAX];
