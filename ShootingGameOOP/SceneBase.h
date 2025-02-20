#pragma once

// �������� ��� ����ü
struct FileHeader
{
	int _cnt;
	char _fileName[10][256];
};

struct EnemyAttribute
{
	char _type;
	int _moveDirectionX;
	int _moveDirectionY;
};

class CSceneBase
{
public:
	CSceneBase();
	virtual ~CSceneBase();

	virtual bool Update() = 0;

	int GetSceneType();

	enum SceneType
	{
		TITLE = 0, GAME, OVER, CLEAR
	};

protected:
	int _SceneType;
};