#include "SceneBase.h"
#include "CList.h"
#include "BaseObject.h"
#include "ObjectManager.h"


CSceneBase::CSceneBase()
{

}

CSceneBase::~CSceneBase()
{
	CObjectManager::GetInstance()->ClearList();
}

int CSceneBase::GetSceneType()
{
	return _SceneType;
}