#pragma once

#include "Global_Struct.h"
#include "Draw.h"
#include "Action.h"

void UpdateTitle();
void UpdateGame();
void UpdateClear(bool* flag);
void UpdateOver(bool* flag);
bool CheckClearGame();		// 스테이지 클리어 체크
bool CheckGameOver();		// 게임오버 체크 


extern Enemy_Info* st_enemyInfo;
extern Enemy_Type* st_enemyType;
extern Stage_Info* st_stageInfo;
extern char* g_stageBuffer;

extern int g_stage;
extern int g_scene;
extern int g_stageCnt;


enum SceneType { TITLE = 1, LOAD, GAME, CLEAR, OVER };