#include "BaseObject.h"
#include "Bullet.h"
#include "ScreenBuffer.h"


CBulletObject::CBulletObject(int ObjectType, int x, int y, int createObjectType) : CBaseObject(ObjectType, x, y)
{
	_bVisible = true;
	_createObjectType = createObjectType;


	// 만약 호출한 대상이 Player라면
	if (_createObjectType == PLAYER)
		 _moveDirection = -1;
	// 적군 총알이라면
	else if(_createObjectType == ENEMY)
		_moveDirection = 1;
}

CBulletObject::~CBulletObject()
{

}

bool CBulletObject::Update()
{
	if (_iY < 0 || _iY >dfSCREEN_HEIGHT)
	{
		_bVisible = false;
		return false;
	}
	_iY += _moveDirection;

	return true;
}

void CBulletObject::Render()
{
	if (_bVisible)
	{
		if (_createObjectType == PLAYER)
			CScreenBuffer::GetInstance()->Sprite_Draw(_iX, _iY, '*');
		else if (_createObjectType == ENEMY)
			CScreenBuffer::GetInstance()->Sprite_Draw(_iX, _iY, 'v');
	}
}

void CBulletObject::OnCollision(CBaseObject* collisionObject)
{
	// 충돌 이후의 로직은 알아서 
	if (collisionObject->GetObjectType() == PLAYER)
	{
		if (_createObjectType == ENEMY)
		{
			_bVisible = false;
			_bCollision = true;
//			CObjectManager::GetInstance()->DestroyObject(this);
		}
		else
		{
			return;
		}
	}
	else if (collisionObject->GetObjectType() == ENEMY)
	{
		if (_createObjectType == PLAYER)
		{
			_bVisible = false;
			_bCollision = true;
	//		CObjectManager::GetInstance()->DestroyObject(this);
		}
		else
		{
			return;
		}
	}
}


int CBulletObject::GetCreateObjectType()
{
	return _createObjectType;
}