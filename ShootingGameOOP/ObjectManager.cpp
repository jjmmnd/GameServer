#include "CList.h"
#include "BaseObject.h"
#include "ObjectManager.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "SceneBase.h"


CObjectManager::CObjectManager()
{
	srand((unsigned int)time_t(NULL));

	_enemyObjectCnt = 0;
}

CObjectManager::~CObjectManager()
{
	// ClearList();
}

CObjectManager* CObjectManager::GetInstance()
{
	static CObjectManager objectManager;
	return &objectManager;
}


void CObjectManager::Update()
{
	CList<CBaseObject*>::iterator iter;

	//--------------------------------------------------
	// 1. 리스트 순회하면서 삭제필요한 노드 삭제
	//--------------------------------------------------
	for (iter = _ObjectList.begin(); iter != _ObjectList.end();)
	{
		/*
		if (*iter == nullptr)
		{
			iter = _ObjectList.erase(iter);
		}
		else
		{
			++iter;
		}
		*/
		
		CBaseObject* pObject = *iter;
		if (pObject->GetCollision())
		{
			// 객체 메모리도 해제 
			delete* iter;
			iter =_ObjectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//--------------------------------------------------
	// 2 - 1. 리스트 순회하면서 오브젝트들의 Update 호출
	// 2 - 2. 오브젝트 간 충돌판정여부 오브젝트에게 전송
	//--------------------------------------------------
	for (iter = _ObjectList.begin(); iter != _ObjectList.end(); ++iter)
	{
		CBaseObject* pObject = *iter;
		pObject->Update();

		CList<CBaseObject*>::iterator target_iter = iter;
		for (++target_iter; target_iter != _ObjectList.end(); ++target_iter)
		{
			CBaseObject* pTargetObject = *target_iter;
			if (CheckPos(*pObject, *pTargetObject))
			{
				pObject->OnCollision(pTargetObject);
				pTargetObject->OnCollision(pObject);
			}
			
		}

	}

	//--------------------------------------------------
	// 3 - 1. enemy 객체가 한 개도 없다면 stage 갱신
	// 3 - 2. stage 갱신 후 SceneGame 다시 로드
	//--------------------------------------------------
	if (!_enemyObjectCnt)
	{
		CSceneManager::GetInstance()->UpdateStage();
		CSceneManager::GetInstance()->LoadScene(CSceneBase::SceneType::GAME);
	}
}

bool CObjectManager::CheckPos(const CBaseObject& pObject, const CBaseObject& pTargetObject)
{
	int pObjectX;
	int pObjectY;
	int targetX;
	int targetY;

	pObject.GetPos(pObjectX, pObjectY);
	pTargetObject.GetPos(targetX, targetY);

	if (pObjectX == targetX && pObjectY == targetY)
		return true;
	else
		return false;
}

void CObjectManager::Render()
{
	CList<CBaseObject*>::iterator iter;

	for (iter = _ObjectList.begin(); iter != _ObjectList.end(); ++iter)
	{
		CBaseObject* pObject = *iter;
		pObject->Render(); // 플래그 대신 nullptr로 설정하면 여기서 문제 
	}
}

void CObjectManager::CreateObject(CBaseObject* pNewObject)
{
	// 인자로 넘어온 새로 생성된 객체의 포인터를 _ObjectList에 push
	_ObjectList.push_back(pNewObject);
}

void CObjectManager::ClearList()
{
	CList<CBaseObject*>::iterator iter;
	for (iter = _ObjectList.begin(); iter != _ObjectList.end(); ++iter)
	{
		delete *iter;
	}

	_ObjectList.clear();
}


void CObjectManager::IncreaseEnemyCnt()
{
	++_enemyObjectCnt;
}

void CObjectManager::ReduceEnemyCnt()
{
	--_enemyObjectCnt;
}
