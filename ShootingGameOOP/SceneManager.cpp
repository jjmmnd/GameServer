#include "SceneBase.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneOver.h"
#include "SceneClear.h"

CSceneManager::CSceneManager()
{
	_pScene = new CSceneTitle;
	_bSceneFlag = false;
	_curSceneType = CSceneBase::SceneType::TITLE;

	_curStage = 1;
}

CSceneManager::~CSceneManager()
{
	delete _pScene;
}

bool CSceneManager::run()
{
	if (_bSceneFlag)
	{
		delete _pScene;

		if (_curStage > g_stStageHeader._cnt)
		{
			_curSceneType = CSceneBase::SceneType::CLEAR;
		}

		switch (_curSceneType)
		{
		case CSceneBase::SceneType::TITLE:
			_pScene = new CSceneTitle;
			break;

		case CSceneBase::SceneType::GAME:
			_pScene = new CSceneGame(_curStage);
			_bSceneFlag = false;
			break;

		case CSceneBase::SceneType::OVER:
			_pScene = new CSceneOver;
			break;

		case CSceneBase::SceneType::CLEAR:
			_pScene = new CSceneClear;
			break;
		}
	}

	return _pScene->Update();

}

void CSceneManager::LoadScene(int loadSceneType)
{
	_bSceneFlag = true;
	_curSceneType = loadSceneType;
}

void CSceneManager::UpdateStage()
{
	++_curStage;
}

CSceneManager* CSceneManager::GetInstance()
{
	static CSceneManager sceneManager;
	return &sceneManager;
}