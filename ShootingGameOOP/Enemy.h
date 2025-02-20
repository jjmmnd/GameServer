#pragma once

class CEnemyObject : public CBaseObject
{
public:
	CEnemyObject(int ObjectType, int x, int y, char enemyType);
	virtual ~CEnemyObject();

	virtual bool Update();
	virtual void Render();

	virtual void OnCollision(CBaseObject* collisionObject);

protected:
	bool _bVisible;

	char _enemyType;

	int _moveDirection_X;
	int _moveDirection_Y;
};


// SceneBase.h에 정의 있음 
extern FileHeader _EnemyHeader;
extern EnemyAttribute g_stEnemyAttribute[10];