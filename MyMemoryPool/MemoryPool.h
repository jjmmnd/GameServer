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

	// 초기 확보량이 필요없는 경우 -> FreeList
	if (m_iCapacity == 0)
		return;

	// 초기 확보량이 필요한 경우 -> MemoryPool
	for (int i = 0; i < m_iCapacity; i++)
	{
		if (m_bPlacementNew == false)
		{
			// 최초 생성자는 호출
			stNode* newNode = new stNode;
			newNode->next = m_pFreeNode;
			m_pFreeNode = newNode;
		}
		else
		{
			// bPlacementNew가 true라면
			// Alloc 전에 replacementNew를 호출할 의무가 있기 때문에
			// 생성자에선 malloc으로 메모리 할당만 진행
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
		// 최총 소멸자는 꼭 호출되어야 함
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
	// 리스트에 재활용할 객체가 없는 경우
	// 실제 동적할당해서 반환해줌
	if (m_iCapacity <= 0)
	{
		stNode* retNode;
		if (m_bPlacementNew)
		{
			retNode = (stNode*)malloc(sizeof(stNode));
			// new와 타입 사이에 사용할 메모리를 명시해서 생성자 호출
			// retNode가 T의 생성자를 호출하면서 인스턴스 생성 및 초기화
			// 이렇게 생성자를 호출했다면 무조건 소멸자를 호출할 때 delete가 아닌 직접 명시해야함
			new (retNode) T;
		}
		else
		{
			retNode = new stNode;
		}

		m_iUseCount++;
		retNode->next = (stNode*)this; // 안전장치 역할
		return (T*)retNode;
	}

	// 리스트에 재활용할 객체가 있는 경우
	// 있는 애를 그냥 주거나 생성자 호출해서 제공 
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
	// next 값 확인 후 반환 필요 
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