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
				// ���� ���������� ��ü ��û
				CSceneManager::GetInstance()->LoadScene(CSceneBase::SceneType::OVER);
			}
		}
	}

}

bool CPlayerObject::KeyProcess()
{
	// ���� ����Ű.
	if (GetAsyncKeyState(VK_LEFT))
	{
		// g_stPlayer.iX -= 1;
		_iX--;
	}
	// ������ ����Ű.
	if (GetAsyncKeyState(VK_RIGHT))
	{
		//g_stPlayer.iX += 1;
		++_iX;
	}
	// ���� ����Ű.
	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		//g_stPlayer.iY--;
		--_iY;
	}
	// �Ʒ��� ����Ű.
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		//g_stPlayer.iY++;
		++_iY;
	}

	_iX = max(_iX, 0);
	_iX = min(_iX, 79);
	_iY = max(_iY, 0);
	_iY = min(_iY, 23);


	// ��Ʈ�� Ű. (�̻��� Ű)
	if (GetAsyncKeyState(VK_CONTROL))
	{
		// �÷��̾��� ��ġ���� �Ѿ� �߻�
		CBaseObject* pObject = new CBulletObject(BULLET, _iX, _iY, PLAYER);
		// �Ŵ������� ����Ʈ�߰� ��û
		CObjectManager::GetInstance()->CreateObject(pObject);
	}

	return true;
}
