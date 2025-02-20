#pragma once


// ΩÃ±€≈Ê 
class CSceneManager
{
private:
	CSceneManager();
	~CSceneManager();

public:
	bool run();
	void LoadScene(int loadSceneType);

	void UpdateStage();

	static CSceneManager* GetInstance();

private:
	CSceneBase* _pScene;

	bool _bSceneFlag;
	int _curSceneType;
	int _curStage;
};

extern FileHeader g_stStageHeader;
