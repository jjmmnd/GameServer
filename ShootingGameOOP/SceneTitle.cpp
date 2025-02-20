#include "SceneBase.h"
#include "SceneTitle.h"
#include <Windows.h>
#include <cstdio>
#include "SceneManager.h"
#include "ScreenBuffer.h"

FileHeader g_stStageHeader;
FileHeader _EnemyHeader;
EnemyAttribute g_stEnemyAttribute[10];

CSceneTitle::CSceneTitle()
{
	_SceneType = TITLE;

	//--------------------------------------------------
	// 1. 스테이지 헤더 파일 내용 텍스트 모드로 읽어서 버퍼에 저장
	//--------------------------------------------------
	char* _TEMPBuffer;

	FILE* fPtr;
	fopen_s(&fPtr, "stage.txt", "r");
	fseek(fPtr, 0, SEEK_END);
	long fileSize = ftell(fPtr);
	fseek(fPtr, 0, SEEK_SET);
	_TEMPBuffer = (char*)malloc(fileSize);
	fread(_TEMPBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	//--------------------------------------------------
	// 2. 버퍼에 저장된 내용 스테이지 헤더에 넣기
	//--------------------------------------------------
	char* context = NULL;
	char* token = strtok_s(_TEMPBuffer, "\n", &context);
	g_stStageHeader._cnt = atoi(token);

	for (int i = 0; i < g_stStageHeader._cnt; i++)
	{
		token = strtok_s(NULL, "\n", &context);
		strcpy_s(g_stStageHeader._fileName[i], 256, token);
	}
	free(_TEMPBuffer);
	//--------------------------------------------------
	// 3. 적 헤더 파일 내용 텍스트 모드로 읽어서 버퍼에 저장
	//--------------------------------------------------
	fopen_s(&fPtr, "enemy.txt", "r");
	fseek(fPtr, 0, SEEK_END);
	fileSize = ftell(fPtr);
	fseek(fPtr, 0, SEEK_SET);
	_TEMPBuffer = (char*)malloc(fileSize);
	fread(_TEMPBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	//--------------------------------------------------
	// 4. 버퍼에 저장된 내용 적 헤더에 넣기
	//--------------------------------------------------
	context = NULL;
	token = strtok_s(_TEMPBuffer, "\n", &context);
	_EnemyHeader._cnt = atoi(token);

	for (int i = 0; i < _EnemyHeader._cnt; i++)
	{
		token = strtok_s(NULL, "\n", &context);
		strcpy_s(_EnemyHeader._fileName[i], 256, token);
	}
	free(_TEMPBuffer);
	//--------------------------------------------------
	//--------------------------------------------------
	// 5. _EnemyHeader 기반으로 enemy 파일들 열고 g_stEnemyAttribute에 내용 저장 
	//--------------------------------------------------
	//--------------------------------------------------
	for (int i = 0; i < _EnemyHeader._cnt; i++)
	{
		fopen_s(&fPtr, _EnemyHeader._fileName[i], "r");
		fseek(fPtr, 0, SEEK_END);
		fileSize = ftell(fPtr);
		fseek(fPtr, 0, SEEK_SET);

		_TEMPBuffer = (char*)malloc(fileSize);
		fread(_TEMPBuffer, 1, fileSize, fPtr);
		fclose(fPtr);

		context = NULL;
		token = strtok_s(_TEMPBuffer, "\n", &context);
		g_stEnemyAttribute[i]._type = *token;
		token = strtok_s(NULL, "\n", &context);
		g_stEnemyAttribute[i]._moveDirectionX = atoi(token);
		g_stEnemyAttribute[i]._moveDirectionY = atoi(token);

		free(_TEMPBuffer);
	}

}

CSceneTitle::~CSceneTitle() { }

bool CSceneTitle::Update()
{
	//===============================================================
	// 키보드 처리
	// 숫자 1 누르면 게임 씬으로 
	if (GetAsyncKeyState(VK_NUMPAD1) || GetAsyncKeyState(0x31))
	{
		CSceneManager::GetInstance()->LoadScene(CSceneBase::SceneType::GAME);

	}
	// 숫자 2 누르면 게임 Exit
	if (GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState(0x32))
	{
		CSceneManager::GetInstance()->LoadScene(CSceneBase::SceneType::OVER);
	}
	//===============================================================
	// 랜더
	CScreenBuffer::GetInstance()->Buffer_Clear();
	const char startString[] = "1. Start Game";
	const char exitString[] = "2. Exit Game";
	int x = 35;
	int y = 15;
	for (int i = 0; i < strlen(startString); i++)
	{
		CScreenBuffer::GetInstance()->Sprite_Draw(x, y, startString[i]);
		x++;
	}
	x = 35;
	y = 16;
	for (int i = 0; i < strlen(exitString); i++)
	{
		CScreenBuffer::GetInstance()->Sprite_Draw(x, y, exitString[i]);
		x++;
	}
	CScreenBuffer::GetInstance()->Buffer_Flip();
	
	return true;
}