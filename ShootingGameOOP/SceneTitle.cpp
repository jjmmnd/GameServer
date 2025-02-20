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
	// 1. �������� ��� ���� ���� �ؽ�Ʈ ���� �о ���ۿ� ����
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
	// 2. ���ۿ� ����� ���� �������� ����� �ֱ�
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
	// 3. �� ��� ���� ���� �ؽ�Ʈ ���� �о ���ۿ� ����
	//--------------------------------------------------
	fopen_s(&fPtr, "enemy.txt", "r");
	fseek(fPtr, 0, SEEK_END);
	fileSize = ftell(fPtr);
	fseek(fPtr, 0, SEEK_SET);
	_TEMPBuffer = (char*)malloc(fileSize);
	fread(_TEMPBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	//--------------------------------------------------
	// 4. ���ۿ� ����� ���� �� ����� �ֱ�
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
	// 5. _EnemyHeader ������� enemy ���ϵ� ���� g_stEnemyAttribute�� ���� ���� 
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
	// Ű���� ó��
	// ���� 1 ������ ���� ������ 
	if (GetAsyncKeyState(VK_NUMPAD1) || GetAsyncKeyState(0x31))
	{
		CSceneManager::GetInstance()->LoadScene(CSceneBase::SceneType::GAME);

	}
	// ���� 2 ������ ���� Exit
	if (GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState(0x32))
	{
		CSceneManager::GetInstance()->LoadScene(CSceneBase::SceneType::OVER);
	}
	//===============================================================
	// ����
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