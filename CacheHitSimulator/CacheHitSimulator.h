#pragma once
#define WAY 12

class CCacheSimulator
{
public:
	CCacheSimulator()
	{
		_Table.SetIndex(64);
		memset(_aCacheHit, 0, sizeof(int) * 64);
		memset(_aCacheMiss, 0, sizeof(int) * 64);
		_totalCnt = 0;
	}
	~CCacheSimulator()
	{
		// 파일에 인덱스 별 캐시히트, 캐시미스 통계 출력
		FILE* fPtr;
		fopen_s(&fPtr, "CacheHitMiss.txt", "w");
		if (fPtr == NULL)
		{
			std::cout << "File Open fail\n";
			return;
		}

		for (int i = 0; i < 64; i++)
		{
			fprintf(fPtr, "[%d]\t total Count: %d\t|\t", i + 1, _Table._hashTable[i].indexCnt);
			fprintf(fPtr, "Cache Hit: %d\t|\t", _aCacheHit[i]);
			fprintf(fPtr, "Cache Miss: %d\n", _aCacheMiss[i]);
		}

		fclose(fPtr);
	}

	void CacheHit(void* addr)
	{
		DWORD index = ((DWORD)addr >> 6) & 0x0000003f;
		_totalCnt++;

		if (_Table.Find((DWORD)addr, _totalCnt))
		{
			// 캐시히트
			_aCacheHit[index]++;
		}
		else
		{
			// 캐시미스
			_Table.Insert((DWORD)addr, _totalCnt);
			_aCacheMiss[index]++;
		}
	}

private:
	CHashTable _Table;
	int _aCacheHit[64];
	int _aCacheMiss[64];
	int _totalCnt;
};