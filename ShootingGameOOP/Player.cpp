#include "BaseObject.h"
#include "Player.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "ScreenBuffer.h"
#include "Bullet.h"
#include <Windows.h>
#include "CList.h"
#include "ObjectManager.h"


CPlayerObject::CPlayerObject(int ObjectType, int x, int y) : CBaseObject(ObjectType,x,y)
{
	_bVisible = true;
	_HP = 5;
}

CPlayerObject::~CPlayerObject()
{
	_bVisible = false;
}

bool CPlayerObject::Update()
{
	if (!KeyProcess())
	{
		CSceneManager::GetInstance()->LoadScene(CSceneBase::SceneType::OVER);
		return false;
	}

	return true;
}

void CPlayerObject::Render()
{
	CScreenBuffer::GetInstance()->Sprite_Draw(_iX, _iY, '#');
}

void CPlayerObject::OnCollision(CBaseObject* collisionObject)
{
	if (collisionObject->GetObjectType() == BULLET)
	{
		if (static_cast<CBulletObject*>(collisionObject)->GetCreateObjectType() == ENEMY)
		{
			--_HP;
			if (_HP <= 0)
			{
				_bVisible = false;
				// 게임 오버씬으로 교체 요청
				CSceneManager::GetInstance()->LoadScene(CSceneBase::SceneType::OVER);
			}
		}
	}

}

bool CPlayerObject::KeyProcess()
{
	// 왼쪽 방향키.
	if (GetAsyncKeyState(VK_LEFT))
	{
		// g_stPlayer.iX -= 1;
		_iX--;
	}
	// 오른쪽 방향키.
	if (GetAsyncKeyState(VK_RIGHT))
	{
		//g_stPlayer.iX += 1;
		++_iX;
	}
	// 위쪽 방향키.
	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		//g_stPlayer.iY--;
		--_iY;
	}
	// 아래쪽 방향키.
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		//g_stPlayer.iY++;
		++_iY;
	}

	_iX = max(_iX, 0);
	_iX = min(_iX, 79);
	_iY = max(_iY, 0);
	_iY = min(_iY, 23);


	// 콘트롤 키. (미사일 키)
	if (GetAsyncKeyState(VK_CONTROL))
	{
		// 플레이어의 위치에서 총알 발사
		CBaseObject* pObject = new CBulletObject(BULLET, _iX, _iY, PLAYER);
		// 매니저에게 리스트추가 요청
		CObjectManager::GetInstance()->CreateObject(pObject);
	}

	return true;
}
