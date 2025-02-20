#pragma once

class CBulletObject : public CBaseObject
{
public:
	CBulletObject(int ObjectType, int x, int y, int createObjectType);
	virtual ~CBulletObject();

	virtual bool Update();
	virtual void Render();

	virtual void OnCollision(CBaseObject* collisionObject);

	int GetCreateObjectType();

protected:
	bool _bVisible;

	int _moveDirection;

	int _createObjectType;

};