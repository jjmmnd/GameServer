#pragma once


class CPlayerObject : public CBaseObject
{
public:
	CPlayerObject(int ObjectType, int x, int y);
	virtual ~CPlayerObject();

	virtual bool Update();
	virtual void Render();

	virtual void OnCollision(CBaseObject* collisionObject);

	bool KeyProcess(void);

protected:
	bool _bVisible;

	int _HP;
};