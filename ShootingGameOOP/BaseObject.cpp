#include "BaseObject.h"

CBaseObject::CBaseObject()
{

}

CBaseObject::CBaseObject(int ObjectType, int x, int y)
{
	_iX = x;
	_iY = y;
	_ObjectType = ObjectType;

	_bCollision = false;
}

CBaseObject::~CBaseObject()
{

}

int CBaseObject::GetObjectType()
{
	return _ObjectType;
}


void CBaseObject::GetPos(int& x, int& y) const
{
	x = _iX;
	y = _iY;
}

bool CBaseObject::GetCollision()
{
	return _bCollision;
}