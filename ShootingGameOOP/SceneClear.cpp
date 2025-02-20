#include "SceneBase.h"
#include "SceneClear.h"
#include "ScreenBuffer.h"
#include <cstring>


CSceneClear::CSceneClear()
{
	_SceneType = CLEAR;
}
CSceneClear::~CSceneClear()
{

}
bool CSceneClear::Update()
{
	// ·£´õ
	CScreenBuffer::GetInstance()->Buffer_Clear();
	const char clearString[] = "Game Clear";
	int x = 35;
	int y = 15;
	for (int i = 0; i < strlen(clearString); i++)
	{
		CScreenBuffer::GetInstance()->Sprite_Draw(x, y, clearString[i]);
		x++;
	}
	CScreenBuffer::GetInstance()->Buffer_Flip();

	return false;
}