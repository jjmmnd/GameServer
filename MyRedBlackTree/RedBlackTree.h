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

	stNODE _Nil;	// �� �������. ������ �� / ������ �� / NULL �� ���� �뵵.
	stNODE* _head;	// root ��带 ����Ŵ 
public:
	
	// ������ �Ҹ���
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
	// Nil ��� �ʱ�ȭ 
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
			// ��ã��
			return false;
	}
	*outValue = nextNode->iValue;
	return true;
}

CRedBlackTree::stNODE* CRedBlackTree::LeftRotation(stNODE* pNode)
{
	stNODE* newNode = pNode->pRight; // ������ �� ��� 
	pNode->pRight = newNode->pLeft;
	newNode->pParent = pNode->pParent;

	if (newNode->pLeft != &_Nil)
		newNode->pLeft->pParent = pNode;
	// 1. pNode�� root�� ��
	if (pNode == _head)
		_head = newNode;
	// 2. pNode�� ���� �ڽ��� ��
	else if (pNode->pParent->pLeft == pNode)
		pNode->pParent->pLeft = newNode;
	// 3. pNode�� ������ �ڽ��� ��
	else
		pNode->pParent->pRight = newNode;

	newNode->pLeft = pNode;
	pNode->pParent = newNode;

	// ������ �� ��带 ��ȯ
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
		// ù ����
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
				// left�� nil�̸� ����
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
	

	// key�� �´� �ڸ��� ���� �� balance�Լ� ȣ��
	InsertBalance(newNode);
	_head->Color = BLACK;
}



void CRedBlackTree::InsertBalance(stNODE* pNode)
{
	// �ű� ����� �θ� ��� 
	stNODE* parentNode = pNode->pParent;
	if (parentNode->Color == BLACK)
		return;

	// ���� ��� ã��
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
		// �θ� ������ ��忴�� ��
		if (parentNode == parentNode->pParent->pRight)
		{
			InsertCase2(pNode);
		}
		// �θ� ���� ��忴�� �� 
		else if (parentNode == parentNode->pParent->pLeft)
		{
			InsertCase3(pNode);
		}
	}

}

/// <summary>
/// 1. �θ� ����, ���̵� ����.
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
/// 2. �θ� ����, ������ ��, �θ�� �����ʳ��
/// </summary>
void CRedBlackTree::InsertCase2(stNODE* pNode)
{
	stNODE* parentNode = pNode->pParent;

	// ���� �θ��� ������ ����
	if (pNode->pParent->pRight == pNode)
	{
		parentNode->Color = BLACK;
		parentNode->pParent->Color = RED;
		stNODE* check = LeftRotation(parentNode->pParent);
		if (check == parentNode)
			return;
	}
	// ���� �θ��� ���� ���� 
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
/// 3. �θ� ����, ������ ��, �θ�� ���ʳ��
/// </summary>
void CRedBlackTree::InsertCase3(stNODE* pNode)
{
	stNODE* parentNode = pNode->pParent;

	// ���� �θ��� ������ ����
	if (pNode->pParent->pRight == pNode)
	{
		pNode = LeftRotation(parentNode);
		pNode->Color = BLACK;
		pNode->pParent->Color = RED;
		stNODE* check = RightRotation(pNode->pParent);
		if (check == pNode)
			return;
	}
	// ���� �θ��� ���� ���� 
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

	// ������ ��� ��ġã��
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
			// ��ã��
			__debugbreak();
	}
	// ������ ��� : nextNode


	
	stNODE* deleteNode = nullptr; 
	
	// ������ ����� �ڽ��� 2���� ��� 
	if (nextNode->pLeft != &_Nil && nextNode->pRight != &_Nil)
	{
		stNODE* replaceNode = nextNode->pLeft;
		while (replaceNode->pRight != &_Nil)
		{
			replaceNode = replaceNode->pRight;
		}
		nextNode->iKey = replaceNode->iKey;	// value�� ���� 

		deleteNode = replaceNode;
	}
	// ������ ����� �ڽ��� 1�� ������ ��� 
	else
	{
		deleteNode = nextNode;
	}

	//==============================================================
	// ���� �۾� ����	
	// ���� ��� deleteNode�� �ڽ��� 1�� ���� 
	//==============================================================
	
	// ������ ��尡 root�� ��
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

	// ���� ���
	stNODE* newNode;
	
	// ������ ��尡 �θ��� ���� ����� �� 
	if (deleteNode == deleteNode->pParent->pLeft)
	{
		// ������ ����� ���� �ڽ��� ���� ��
		if (deleteNode->pLeft != &_Nil)
		{
			newNode = deleteNode->pLeft;
			deleteNode->pParent->pLeft = newNode;
			newNode->pParent = deleteNode->pParent;
		}
		// ������ ����� ������ �ڽ��� ���� �� 
		else if (deleteNode->pRight != &_Nil)
		{
			newNode = deleteNode->pRight;
			deleteNode->pParent->pLeft = newNode;
			newNode->pParent = deleteNode->pParent;
		}
		else
		{
			// ������尡 ��������϶�
			deleteNode->pParent->pLeft = &_Nil;
			newNode = &_Nil;
			newNode->pParent = deleteNode->pParent;
		}
		

		if (deleteNode->Color == BLACK)
			LeftRemoveBalance(newNode);
	}

	// ������ ��尡 �θ��� ������ ����� �� 
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
			// ������尡 ��������϶�
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
/// ���� �뷱�� �Լ� - ���� : �ݺ� �˻�
/// ������ ��尡 �θ��� ���� �ڽ��� ���
/// </summary>
/// <param name="pNode">
/// ���� ��� (���� ����� �ڽ�)
/// </param>
void CRedBlackTree::LeftRemoveBalance(stNODE* pNode)
{
	// ������ ����� ���� ��� 
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
			// ���� ��带 �������� ó������ ���۾� ??
			continue;
		}
		else if (sibNode->Color == BLACK)
		{
			if (sibNode->pLeft->Color == BLACK && sibNode->pRight->Color == BLACK)
			{
				sibNode->Color = RED;
				pNode = pNode->pParent;
				// �� �θ� �������� �ٽ� ó�� 
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
/// ���� �뷱�� �Լ� - ������ 
/// ������ ��尡 �θ��� ������ �Լ��� ��� 
/// </summary>
/// <param name="pNode">
/// ���� ��� (������ ����� �ڽ�)
/// ���� �ڽ� or _Nil
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
		// ���س�尡 ���� ���� 
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