#pragma once

#include <new.h>
#include <stdlib.h>

template <class T>
class CMemoryPool
{
	struct stNode
	{
		T data;
		stNode* next;
	};

public:
	CMemoryPool(int iBlockNum, bool bPlacementNew = false);
	virtual	~CMemoryPool();

	T* Alloc(void);
	bool Free(T* pData);

	int	GetCapacityCount(void);
	int	GetUseCount(void);

private:
	stNode* m_pFreeNode; // top
	int m_iCapacity;
	int m_iUseCount;
	bool m_bPlacementNew;
};

template <class T>
CMemoryPool<T>::CMemoryPool(int iBlockNum, bool bPlacementNew) : m_iCapacity(iBlockNum), m_bPlacementNew(bPlacementNew)
{
	m_iUseCount = 0;
	m_pFreeNode = nullptr;

	// �ʱ� Ȯ������ �ʿ���� ��� -> FreeList
	if (m_iCapacity == 0)
		return;

	// �ʱ� Ȯ������ �ʿ��� ��� -> MemoryPool
	for (int i = 0; i < m_iCapacity; i++)
	{
		if (m_bPlacementNew == false)
		{
			// ���� �����ڴ� ȣ��
			stNode* newNode = new stNode;
			newNode->next = m_pFreeNode;
			m_pFreeNode = newNode;
		}
		else
		{
			// bPlacementNew�� true���
			// Alloc ���� replacementNew�� ȣ���� �ǹ��� �ֱ� ������
			// �����ڿ��� malloc���� �޸� �Ҵ縸 ����
			stNode* newNode = (stNode*)malloc(sizeof(stNode));
			if (newNode == NULL)
			{
				int* p = nullptr;
				*p = 0;
			}
			newNode->next = m_pFreeNode;
			m_pFreeNode = newNode;
		}
	}
}

template <class T>
CMemoryPool<T>::~CMemoryPool()
{
	stNode* deleteNode = m_pFreeNode;
	stNode* nextNode;
	while (deleteNode != nullptr)
	{
		nextNode = deleteNode->next;
		// ���� �Ҹ��ڴ� �� ȣ��Ǿ�� ��
		if (m_bPlacementNew)
			free(deleteNode);
		else
			delete deleteNode;
		deleteNode = nextNode;
	}
}


template <class T>
T* CMemoryPool<T>::Alloc()
{
	// ����Ʈ�� ��Ȱ���� ��ü�� ���� ���
	// ���� �����Ҵ��ؼ� ��ȯ����
	if (m_iCapacity <= 0)
	{
		stNode* retNode;
		if (m_bPlacementNew)
		{
			retNode = (stNode*)malloc(sizeof(stNode));
			// new�� Ÿ�� ���̿� ����� �޸𸮸� ����ؼ� ������ ȣ��
			// retNode�� T�� �����ڸ� ȣ���ϸ鼭 �ν��Ͻ� ���� �� �ʱ�ȭ
			// �̷��� �����ڸ� ȣ���ߴٸ� ������ �Ҹ��ڸ� ȣ���� �� delete�� �ƴ� ���� ����ؾ���
			new (retNode) T;
		}
		else
		{
			retNode = new stNode;
		}

		m_iUseCount++;
		retNode->next = (stNode*)this; // ������ġ ����
		return (T*)retNode;
	}

	// ����Ʈ�� ��Ȱ���� ��ü�� �ִ� ���
	// �ִ� �ָ� �׳� �ְų� ������ ȣ���ؼ� ���� 
	stNode* retNode = m_pFreeNode;
	m_pFreeNode = m_pFreeNode->next;
	if (m_bPlacementNew)
	{
		new (retNode) T;
	}
	m_iUseCount++;
	m_iCapacity--;
	retNode->next = (stNode*)this;
	return (T*)retNode;
}

template <class T>
bool CMemoryPool<T>::Free(T* pData)
{
	// next �� Ȯ�� �� ��ȯ �ʿ� 
	if (pData == nullptr || ((stNode*)pData)->next != (stNode*)this)
		return false;

	if (m_bPlacementNew)
		pData->~T();

	m_iUseCount--;
	m_iCapacity++;
	

	((stNode*)pData)->next = m_pFreeNode;
	m_pFreeNode = (stNode*)pData;

	return true;
}

template <class T>
int CMemoryPool<T>::GetCapacityCount()
{
	return m_iCapacity;
}

template <class T>
int CMemoryPool<T>::GetUseCount()
{
	return m_iUseCount;
}