#pragma once

class CSceneGame :public CSceneBase
{
public:
	CSceneGame(int curStage);
	virtual ~CSceneGame();
	virtual bool Update();
	
private:
	int _curStage;
};

extern FileHeader g_stStageHeader;