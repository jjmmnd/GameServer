#pragma once

class CBaseObject
{
public:
	CBaseObject();
	CBaseObject(int ObjectType, int x, int y);
	virtual ~CBaseObject();

	virtual bool Update() = 0;
	virtual void Render() = 0;

	virtual void OnCollision(CBaseObject* collisionObject) = 0;

	int GetObjectType();
	void GetPos(int& x, int& y) const;

	bool GetCollision();

	enum ObjectEnum
	{
		PLAYER = 0, ENEMY, BULLET
	};

protected:
	int _iX;
	int _iY;
	int _ObjectType;

	bool _bCollision;
};