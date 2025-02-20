#include <cstdlib>
#include "BaseObject.h"
#include "SceneBase.h"
#include "Enemy.h"
#include "ScreenBuffer.h"
#include "Bullet.h"
#include "CList.h"
#include "ObjectManager.h"


CEnemyObject::CEnemyObject(int ObjectType, int x, int y, char enemyType) : CBaseObject(ObjectType, x, y)
{
	_bVisible = true;
	_enemyType = enemyType;

	// 전역 헤더 참고해서 적군 속성 세팅 
	for (int i = 0; i < _EnemyHeader._cnt; i++)
	{
		if (g_stEnemyAttribute[i]._type == _enemyType)
		{
			_moveDirection_X = g_stEnemyAttribute[i]._moveDirectionX;
			_moveDirection_Y = g_stEnemyAttribute[i]._moveDirectionY;
		}
	}
}

CEnemyObject::~CEnemyObject()
{

}

bool CEnemyObject::Update()
{
	if (_iX < 0 || _iX >= dfSCREEN_WIDTH || _iY < 0 || _iY >= dfSCREEN_HEIGHT)
	{
		_bVisible = false;
		return false;
	}

	if (_iX == 10 || _iX == dfSCREEN_WIDTH / 2 || _iX == dfSCREEN_WIDTH - 10)
		_moveDirection_X = -_moveDirection_X;
	if (_iY == 1 || _iY == dfSCREEN_HEIGHT / 2)
		_moveDirection_Y = -_moveDirection_Y;

	_iX += _moveDirection_X;
	_iY += _moveDirection_Y;

	if (rand() % 100 < 4)
	{
		// 총알 생성
		CBaseObject* pObject = new CBulletObject(BULLET, _iX, _iY, ENEMY);
		// 매니저에게 리스트 추가 요청
		CObjectManager::GetInstance()->CreateObject(pObject);
	}
	
	return true;
}

void CEnemyObject::Render()
{
	if(_bVisible)
		CScreenBuffer::GetInstance()->Sprite_Draw(_iX, _iY, _enemyType);
}

void CEnemyObject::OnCollision(CBaseObject* collisionObject)
{
	// 내가 충돌됐다는 뜻이기 때문에 플래그를 남김 

	if (collisionObject->GetObjectType() == BULLET)
	{
		// bullet과 충돌했을 때 bullet의 생성 Type과 비교
		if (static_cast<CBulletObject*>(collisionObject)->GetCreateObjectType() == PLAYER)
		{
//			CObjectManager::GetInstance()->DestroyObject(this);
			_bCollision = true;
			_bVisible = false;
			CObjectManager::GetInstance()->ReduceEnemyCnt();

		}
	}
	return;
}
