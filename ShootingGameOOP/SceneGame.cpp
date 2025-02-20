#include "SceneBase.h"
#include "SceneGame.h"
#include <Windows.h>
#include <cstdio>
#include "BaseObject.h"
#include "Player.h"
#include "ScreenBuffer.h"
#include "Enemy.h"
#include "CList.h"
#include "ObjectManager.h"

CSceneGame::CSceneGame(int curStage)
{
	_SceneType = GAME;
	_curStage = curStage;
	//--------------------------------------------------
	// 1. 플레이어 객체 생성
	//--------------------------------------------------
	CPlayerObject* pPlayer = new CPlayerObject(CBaseObject::ObjectEnum::PLAYER, 44, 18);
	CObjectManager::GetInstance()->CreateObject(pPlayer);

	//--------------------------------------------------
	// 2 - 1. _stageBuffer에 _curStage의 배치 불러옴
	// 2 - 2. _stageBuffer 기준으로 적군 좌표로 Enemy객체 생성 
	//--------------------------------------------------
	FILE* fPtr;
	fopen_s(&fPtr, g_stStageHeader._fileName[curStage - 1], "r");
	fseek(fPtr, 0, SEEK_END);
	long fileSize = ftell(fPtr);
	fseek(fPtr, 0, SEEK_SET);

	char *_stageBuffer = (char*)malloc(fileSize);
	fread(_stageBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	//_stageBuffer[fileSize] = '\n'; // 끝을 나타내기 위해 널문자 저장
	//--------------------------------------------------
	char* chpBuffer = _stageBuffer;

	int posX = 0;
	int posY = 0;
	for (int i = 0; i < dfSCREEN_HEIGHT - 1; i++)
	{
		while (*chpBuffer != '\n')
		{
			if (*chpBuffer != ' ' && *chpBuffer != '\\')
			{
				CEnemyObject* pEnemy = new CEnemyObject(CBaseObject::ObjectEnum::ENEMY, posX, posY, *chpBuffer);
				CObjectManager::GetInstance()->CreateObject(pEnemy);
				CObjectManager::GetInstance()->IncreaseEnemyCnt();
			}
			++posX;
			++chpBuffer;
		}
		posX = 0;
		++posY;
		++chpBuffer;
	}

	free(_stageBuffer);
}

CSceneGame::~CSceneGame()
{

}

bool CSceneGame::Update()
{
	// 키보드 처리
	// ESC 키. (종료)
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		return false;
	}

	// 로직 
	CObjectManager::GetInstance()->Update();

	// 랜더
	CScreenBuffer::GetInstance()->Buffer_Clear();
	CObjectManager::GetInstance()->Render();
	CScreenBuffer::GetInstance()->Buffer_Flip();

	return true;
}