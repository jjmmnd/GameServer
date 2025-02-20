#pragma once

// ΩÃ±€≈Ê 
class CObjectManager
{
private:
	CObjectManager();
	~CObjectManager();

public:
	void Update();
	void Render();

	static CObjectManager* GetInstance();

	void CreateObject(CBaseObject* pNewObject);

	void ClearList();

	void IncreaseEnemyCnt();
	void ReduceEnemyCnt();

	bool CheckPos(const CBaseObject& pObject, const CBaseObject& pTargetObject);

private:
	CList<CBaseObject*> _ObjectList;
	int _enemyObjectCnt;
	
};