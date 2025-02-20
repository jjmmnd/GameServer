#pragma once

///template<class T>
class CRedBlackTree
{
private:
	enum NODE_COLOR
	{
		BLACK = 0,
		RED
	};

	struct stNODE
	{
		stNODE* pParent;
		stNODE* pLeft;
		stNODE* pRight;

		NODE_COLOR Color;

		int iKey;   // Key , Value
		int iValue;
		int value;
	};

	stNODE _Nil;	// 끝 리프노드. 무조건 블랙 / 데이터 무 / NULL 의 같은 용도.
	stNODE* _head;	// root 노드를 가리킴 
public:
	
	// 생성자 소멸자
	CRedBlackTree();
	~CRedBlackTree();

	void Insert(int key,int value);	//T value
	void Remove(int key);
	void PrintTree();
	bool Search(int key, int* outValue);

private:
	void Clear(stNODE* pNext);

	void InorderPrint(stNODE* pNext);


	void InsertBalance(stNODE *pNode);
	void LeftRemoveBalance(stNODE *pNode);
	void RightRemoveBalance(stNODE *pNode);

	void InsertCase1(stNODE* pNode, stNODE* uncleNode);
	void InsertCase2(stNODE* pNode);
	void InsertCase3(stNODE* pNode);

	stNODE* LeftRotation(stNODE* pNode);
	stNODE* RightRotation(stNODE* pNode);
};


CRedBlackTree::CRedBlackTree()
{
	// Nil 노드 초기화 
	_Nil.Color = BLACK;
	_Nil.pParent = nullptr;
	_Nil.pLeft = nullptr;
	_Nil.pRight = nullptr;

	_head = nullptr;
}

CRedBlackTree::~CRedBlackTree()
{
	Clear(_head);
	_head = nullptr;
}

void CRedBlackTree::Clear(stNODE* pNext)
{
	if (pNext != &_Nil)
	{
		Clear(pNext->pLeft);
		Clear(pNext->pRight);
		delete pNext;
	}
}

void CRedBlackTree::PrintTree()
{
	
	InorderPrint(_head);
}

void CRedBlackTree::InorderPrint(stNODE* pNext)
{
	if (pNext != &_Nil)
	{
		InorderPrint(pNext->pLeft);
		std::cout << pNext->iKey << '('<<pNext->Color<<')'<<' ';
		InorderPrint(pNext->pRight);
	}
}


bool CRedBlackTree::Search(int key, int* outValue)
{
	if (_head->iKey == key)
	{
		*outValue = _head->iValue;
		return true;
	}

	stNODE* nextNode = _head;
	while (nextNode->iKey != key)
	{
		if (nextNode->iKey > key)
		{
			nextNode = nextNode->pLeft;
		}
		else if (nextNode->iKey < key)
		{
			nextNode = nextNode->pRight;
		}
		else
			// 못찾음
			return false;
	}
	*outValue = nextNode->iValue;
	return true;
}

CRedBlackTree::stNODE* CRedBlackTree::LeftRotation(stNODE* pNode)
{
	stNODE* newNode = pNode->pRight; // 기준이 될 노드 
	pNode->pRight = newNode->pLeft;
	newNode->pParent = pNode->pParent;

	if (newNode->pLeft != &_Nil)
		newNode->pLeft->pParent = pNode;
	// 1. pNode가 root일 때
	if (pNode == _head)
		_head = newNode;
	// 2. pNode가 왼쪽 자식일 때
	else if (pNode->pParent->pLeft == pNode)
		pNode->pParent->pLeft = newNode;
	// 3. pNode가 오른쪽 자식일 때
	else
		pNode->pParent->pRight = newNode;

	newNode->pLeft = pNode;
	pNode->pParent = newNode;

	// 기준이 된 노드를 반환
	return newNode;
}

CRedBlackTree::stNODE* CRedBlackTree::RightRotation(stNODE* pNode)
{
	stNODE* newNode = pNode->pLeft;
	pNode->pLeft = newNode->pRight;
	newNode->pParent = pNode->pParent;
	
	if (newNode->pRight != &_Nil)
		newNode->pRight->pParent = pNode;
	if (pNode == _head)
		_head = newNode;
	else if (pNode->pParent->pLeft == pNode)
		pNode->pParent->pLeft = newNode;
	else
		pNode->pParent->pRight = newNode;

	newNode->pRight = pNode;
	pNode->pParent = newNode;

	return newNode;
}

void CRedBlackTree::Insert(int key, int value)
{

	stNODE* newNode = new stNODE;
	newNode->Color = RED;
	newNode->iKey = key;
	newNode->iValue = value;
	newNode->pParent = nullptr;
	newNode->pLeft = &_Nil;
	newNode->pRight = &_Nil;

	if (_head == nullptr)
	{
		// 첫 삽입
		_head = newNode;
		_head->Color = BLACK;
		return;
	}
	
	stNODE* iterator = _head;
	while (iterator != &_Nil)
	{
		if (iterator->iKey > key)
		{
			if (iterator->pLeft != &_Nil)
			{
				iterator = iterator->pLeft;
			}
			else if(iterator->pLeft == &_Nil)
			{
				// left가 nil이면 삽입
				iterator->pLeft = newNode;
				newNode->pParent = iterator;
				break;
			}
		}
		else if (iterator->iKey < key)
		{
			if (iterator->pRight != &_Nil)
			{
				iterator = iterator->pRight;
			}
			else if (iterator->pRight == &_Nil)
			{
				iterator->pRight = newNode;
				newNode->pParent = iterator;
				break;
			}
		}
		else
		{
			__debugbreak();
		}
	}
	

	// key에 맞는 자리에 삽입 후 balance함수 호출
	InsertBalance(newNode);
	_head->Color = BLACK;
}



void CRedBlackTree::InsertBalance(stNODE* pNode)
{
	// 신규 노드의 부모 노드 
	stNODE* parentNode = pNode->pParent;
	if (parentNode->Color == BLACK)
		return;

	// 삼촌 노드 찾기
	stNODE* uncleNode;
	if (parentNode == parentNode->pParent->pRight)
		uncleNode = parentNode->pParent->pLeft;
	else
		uncleNode = parentNode->pParent->pRight;


	if (uncleNode->Color == RED)
	{
		InsertCase1(pNode,uncleNode);
	}
	else// if (uncleNode->Color == BLACK)
	{
		// 부모가 오른쪽 노드였을 때
		if (parentNode == parentNode->pParent->pRight)
		{
			InsertCase2(pNode);
		}
		// 부모가 왼쪽 노드였을 때 
		else if (parentNode == parentNode->pParent->pLeft)
		{
			InsertCase3(pNode);
		}
	}

}

/// <summary>
/// 1. 부모 레드, 삼촌도 레드.
/// </summary>
void CRedBlackTree::InsertCase1(stNODE* pNode,stNODE* uncleNode)
{
	stNODE* grandNode = uncleNode->pParent;
	grandNode->Color = RED;
	uncleNode->Color = BLACK;
	pNode->pParent->Color = BLACK;

	if (grandNode == _head)
	{
		return;
	}
	else
	{
		InsertBalance(grandNode);
	}
}

/// <summary>
/// 2. 부모 레드, 삼촌은 블랙, 부모는 오른쪽노드
/// </summary>
void CRedBlackTree::InsertCase2(stNODE* pNode)
{
	stNODE* parentNode = pNode->pParent;

	// 나는 부모의 오른쪽 레드
	if (pNode->pParent->pRight == pNode)
	{
		parentNode->Color = BLACK;
		parentNode->pParent->Color = RED;
		stNODE* check = LeftRotation(parentNode->pParent);
		if (check == parentNode)
			return;
	}
	// 나는 부모의 왼쪽 레드 
	else //if (pNode->pParent->pLeft == pNode)
	{
		pNode = RightRotation(parentNode);
		pNode->Color = BLACK;
		pNode->pParent->Color = RED;
		stNODE* check = LeftRotation(pNode->pParent);
		if (check == pNode)
			return;
	}
}

/// <summary>
/// 3. 부모 레드, 삼촌은 블랙, 부모는 왼쪽노드
/// </summary>
void CRedBlackTree::InsertCase3(stNODE* pNode)
{
	stNODE* parentNode = pNode->pParent;

	// 나는 부모의 오른쪽 레드
	if (pNode->pParent->pRight == pNode)
	{
		pNode = LeftRotation(parentNode);
		pNode->Color = BLACK;
		pNode->pParent->Color = RED;
		stNODE* check = RightRotation(pNode->pParent);
		if (check == pNode)
			return;
	}
	// 나는 부모의 왼쪽 레드 
	else //if (pNode->pParent->pLeft == pNode)
	{
		parentNode->Color = BLACK;
		parentNode->pParent->Color = RED;
		stNODE* check = RightRotation(parentNode->pParent);
		if (check == parentNode)
			return;
	}
}

void CRedBlackTree::Remove(int key)
{

	if (_head == nullptr)
		return;

	// 삭제할 노드 위치찾기
	stNODE* nextNode = _head;
	while (nextNode->iKey != key)
	{
		if (nextNode->iKey > key)
		{
			nextNode = nextNode->pLeft;
		}
		else if(nextNode->iKey < key)
		{
			nextNode = nextNode->pRight;
		}
		else
			// 못찾음
			__debugbreak();
	}
	// 삭제할 노드 : nextNode


	
	stNODE* deleteNode = nullptr; 
	
	// 삭제할 노드의 자식이 2개인 경우 
	if (nextNode->pLeft != &_Nil && nextNode->pRight != &_Nil)
	{
		stNODE* replaceNode = nextNode->pLeft;
		while (replaceNode->pRight != &_Nil)
		{
			replaceNode = replaceNode->pRight;
		}
		nextNode->iKey = replaceNode->iKey;	// value도 복사 

		deleteNode = replaceNode;
	}
	// 삭제할 노드의 자식이 1개 이하인 경우 
	else
	{
		deleteNode = nextNode;
	}

	//==============================================================
	// 삭제 작업 시작	
	// 삭제 노드 deleteNode는 자식이 1개 이하 
	//==============================================================
	
	// 삭제할 노드가 root일 때
	if (deleteNode->pParent == nullptr)
	{
		if (deleteNode->pLeft != &_Nil)
		{
			_head = deleteNode->pLeft;
			_head->Color = BLACK;
		}
		else if (deleteNode->pRight != &_Nil)
		{
			_head = deleteNode->pRight;
			_head->Color = BLACK;
		}
		else
			_head = nullptr;

		delete deleteNode;
		return;
	}

	// 기준 노드
	stNODE* newNode;
	
	// 삭제할 노드가 부모의 왼쪽 노드일 때 
	if (deleteNode == deleteNode->pParent->pLeft)
	{
		// 삭제할 노드의 왼쪽 자식이 있을 때
		if (deleteNode->pLeft != &_Nil)
		{
			newNode = deleteNode->pLeft;
			deleteNode->pParent->pLeft = newNode;
			newNode->pParent = deleteNode->pParent;
		}
		// 삭제할 노드의 오른쪽 자식이 있을 때 
		else if (deleteNode->pRight != &_Nil)
		{
			newNode = deleteNode->pRight;
			deleteNode->pParent->pLeft = newNode;
			newNode->pParent = deleteNode->pParent;
		}
		else
		{
			// 삭제노드가 리프노드일때
			deleteNode->pParent->pLeft = &_Nil;
			newNode = &_Nil;
			newNode->pParent = deleteNode->pParent;
		}
		

		if (deleteNode->Color == BLACK)
			LeftRemoveBalance(newNode);
	}

	// 삭제할 노드가 부모의 오른쪽 노드일 때 
	else
	{
		if (deleteNode->pLeft != &_Nil)
		{
			newNode = deleteNode->pLeft;
			deleteNode->pParent->pRight = newNode;
			newNode->pParent = deleteNode->pParent;
		}
		else if (deleteNode->pRight != &_Nil)
		{
			newNode = deleteNode->pRight;
			deleteNode->pParent->pRight = newNode;
			newNode->pParent = deleteNode->pParent;
		}
		else
		{
			// 삭제노드가 리프노드일때
			deleteNode->pParent->pRight = &_Nil;
			newNode = &_Nil;
			newNode->pParent = deleteNode->pParent;
		}
		
		if (deleteNode->Color == BLACK)
			RightRemoveBalance(newNode);
	}

	//if(newNode != &_Nil)
	delete deleteNode;
	_head->Color = BLACK;
	return;
}


/// <summary>
/// 삭제 밸런스 함수 - 왼쪽 : 반복 검사
/// 삭제할 노드가 부모의 왼쪽 자식일 경우
/// </summary>
/// <param name="pNode">
/// 기준 노드 (삭제 노드의 자식)
/// </param>
void CRedBlackTree::LeftRemoveBalance(stNODE* pNode)
{
	// 삭제할 노드의 형제 노드 
	stNODE* sibNode;

	while(1)
	{
		sibNode = pNode->pParent->pRight;

		if (pNode->Color == RED)
		{
			pNode->Color = BLACK;
			break;
		}
		else if (sibNode->Color == RED)
		{
			sibNode->Color = BLACK;
			LeftRotation(sibNode->pParent);// ->pLeft->Color = RED;
			pNode->pParent->Color = RED;
			// 기준 노드를 기준으로 처음부터 재작업 ??
			continue;
		}
		else if (sibNode->Color == BLACK)
		{
			if (sibNode->pLeft->Color == BLACK && sibNode->pRight->Color == BLACK)
			{
				sibNode->Color = RED;
				pNode = pNode->pParent;
				// 내 부모를 기준으로 다시 처리 
				continue;
			}
			if (sibNode->pLeft->Color == RED && sibNode->pRight->Color == BLACK)
			{
				sibNode->pLeft->Color = BLACK;
				sibNode->Color = RED;
				RightRotation(sibNode);
			}
			if (sibNode->pRight->Color == RED)
			{
				sibNode->Color = sibNode->pParent->Color;
				sibNode->pParent->Color = BLACK;
				sibNode->pRight->Color = BLACK;
				LeftRotation(sibNode->pParent);
				break;
			}
		}
		else
		{
			__debugbreak();
		}

	}
}

/// <summary>
/// 삭제 밸런스 함수 - 오른쪽 
/// 삭제할 노드가 부모의 오른쪽 함수일 경우 
/// </summary>
/// <param name="pNode">
/// 기준 노드 (삭제할 노드의 자식)
/// 실제 자식 or _Nil
/// </param>
void CRedBlackTree::RightRemoveBalance(stNODE* pNode)
{
	stNODE* sibNode;

	while (1)
	{
		sibNode = pNode->pParent->pLeft;

		if (pNode->Color == RED)
		{
			pNode->Color = BLACK;
			break;
		}
		// 기준노드가 블랙인 경우들 
		else if (sibNode->Color == RED)
		{
			sibNode->Color = BLACK;
			RightRotation(sibNode->pParent);
			pNode->pParent->Color = RED;
			continue;
		}
		else if (sibNode->Color == BLACK)
		{
			if (sibNode->pLeft->Color == BLACK && sibNode->pRight->Color == BLACK)
			{
				sibNode->Color = RED;
				pNode = pNode->pParent;
				continue;
			}
			if (sibNode->pRight->Color == RED && sibNode->pLeft->Color == BLACK)
			{
				sibNode->pRight->Color = BLACK;
				sibNode->Color = RED;
				LeftRotation(sibNode);
			}
			if (sibNode->pLeft->Color == RED)
			{
				sibNode->Color = sibNode->pParent->Color;
				sibNode->pParent->Color = BLACK;
				sibNode->pLeft->Color = BLACK;
				RightRotation(sibNode->pParent);
				break;
			}
		}
	}
}