#pragma once

#include "Global_Struct.h"

#define ENEMY_COUNT 60
#define BULLET_MAX 50
#define dfSCREEN_WIDTH	81		// 콘솔 가로 80칸 + NULL
#define dfSCREEN_HEIGHT	24		// 콘솔 세로 25칸 + NULL


//--------------------------------------------------------------------
// 게임 시작시 전체 데이터 초기화
//--------------------------------------------------------------------
void Initial(void);
void InitPlayer();
void InitHP();
void InitBullet();
void ReadEnemyFileInfo();	// 적군 타입 헤더 읽기	=> 전역 구조체에 저장
void ReadEnemy();			// 타입별 적군의 속성 읽기 => 전역 구조체에 저장 
void ReadStageFileInfo();	// 스테이지 헤더 읽기		=> 전역 구조체에 저장

//--------------------------------------------------------------------
// 게임 로딩시 스테이지 및 적군 초기화
//--------------------------------------------------------------------
void ReadStage();
void InitEnemy(int);
void SettingAttribute(int index, char type);
