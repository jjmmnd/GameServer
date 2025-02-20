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
	// 1. ����Ʈ ��ȸ�ϸ鼭 �����ʿ��� ��� ����
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
			// ��ü �޸𸮵� ���� 
			delete* iter;
			iter =_ObjectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//--------------------------------------------------
	// 2 - 1. ����Ʈ ��ȸ�ϸ鼭 ������Ʈ���� Update ȣ��
	// 2 - 2. ������Ʈ �� �浹�������� ������Ʈ���� ����
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
	// 3 - 1. enemy ��ü�� �� ���� ���ٸ� stage ����
	// 3 - 2. stage ���� �� SceneGame �ٽ� �ε�
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
		pObject->Render(); // �÷��� ��� nullptr�� �����ϸ� ���⼭ ���� 
	}
}

void CObjectManager::CreateObject(CBaseObject* pNewObject)
{
	// ���ڷ� �Ѿ�� ���� ������ ��ü�� �����͸� _ObjectList�� push
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
