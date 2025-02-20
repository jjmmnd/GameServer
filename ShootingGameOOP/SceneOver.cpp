#include "SceneBase.h"
#include "SceneOver.h"
#include "ScreenBuffer.h"
#include "CList.h"
#include "BaseObject.h"
#include "ObjectManager.h"
#include <cstring>

CSceneOver::CSceneOver()
{
	_SceneType = OVER;
}
CSceneOver::~CSceneOver()
{

}
bool CSceneOver::Update()
{
	// ·£´õ
	CScreenBuffer::GetInstance()->Buffer_Clear();
	const char overString[] = "Game Over";
	int x = 35;
	int y = 15;
	for (int i = 0; i < strlen(overString); i++)
	{
		CScreenBuffer::GetInstance()->Sprite_Draw(x, y, overString[i]);
		x++;
	}
	CScreenBuffer::GetInstance()->Buffer_Flip();

	return false;
}