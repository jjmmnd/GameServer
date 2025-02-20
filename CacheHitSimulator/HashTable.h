#pragma once
#define WAY 12

class CHashTable
{
public:
	CHashTable()
	{

	}
	~CHashTable()
	{
		stBucket* pBucket;
		stNode* pNode;
		for (int i = 0; i < _totalIndex; i++)
		{
			pBucket = _hashTable + i;
			if (pBucket == nullptr)
				break;
			for (int j = 0; j < WAY; j++)
			{
				pNode = pBucket->way[j];
				if (pNode == nullptr)
					break;
				delete pNode;
			}
		}
		delete _hashTable;
	}
	void SetIndex(int totalIndex)
	{
		_totalIndex = totalIndex;
		_hashTable = (stBucket*)malloc(sizeof(stBucket) * totalIndex);
		if (_hashTable == nullptr)
			return;
		else
			memset(_hashTable, NULL, sizeof(stBucket) * totalIndex);
	}
	bool Find(DWORD addr, int totalCnt)
	{
		DWORD index = (addr >> 6) & 0x0000003f;
		DWORD tag = addr >> 12;

		stNode* pNode;
		for (int i = 0; i < WAY; i++)
		{
			pNode = _hashTable[index].way[i];
			if (pNode == nullptr)
				break;
			if (pNode->tag == tag)
			{
				pNode->lastCnt = totalCnt;
				return true;
			}
		}

		return false;
	}
	void Insert(DWORD addr, int totalCnt)
	{
		DWORD index = (addr >> 6) & 0x0000003f;
		DWORD tag = addr >> 12;

		stNode* newNode = new stNode;
		newNode->addr = addr;
		newNode->tag = tag;
		newNode->lastCnt = totalCnt;

		bool bInsert = false;
		for (int i = 0; i < WAY; i++)
		{
			if (_hashTable[index].way[i] == nullptr)
			{
				_hashTable[index].way[i] = newNode;
				bInsert = true;
				break;
			}
		}

		if (!bInsert)
		{
			stNode* outNode = _hashTable[index].way[0];
			for (int i = 0; i < WAY; i++)
			{
				if (outNode->lastCnt >= _hashTable[index].way[i]->lastCnt)
					outNode = _hashTable[index].way[i];
				else
					continue;
			}

			stNode* deleteNode = outNode;
			outNode = newNode;
			bInsert = true;
			delete deleteNode;
		}
	}

private:
	struct stNode
	{
		DWORD addr;
		DWORD tag;
		int lastCnt;
	};

	struct stBucket
	{
		stNode* way[WAY];
		int indexCnt;
	};
	
	stBucket* _hashTable;
	int _totalIndex;

	friend class CCacheSimulator;
};