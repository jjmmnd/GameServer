
#include <iostream>
#include <Windows.h>
#include "RingBuffer.h"


CRingBuffer::CRingBuffer() : _iBufferSize(eBUFFER_DEFAULT), _FrontPos(0), _RearPos(0)
{
	_pBuffer = new char[eBUFFER_DEFAULT];
}

CRingBuffer::CRingBuffer(int iBufferSize) : _iBufferSize(iBufferSize), _FrontPos(0), _RearPos(0)
{
	_pBuffer = new char[eBUFFER_DEFAULT];
}


CRingBuffer::~CRingBuffer()
{
	ClearBuffer();
	delete[] _pBuffer;
}

int	CRingBuffer::GetBufferSize()
{
	return _iBufferSize;
}

int	CRingBuffer::GetUseSize()
{

	if (_RearPos >= _FrontPos)
		return _RearPos - _FrontPos;
	else
		return _iBufferSize - (_FrontPos - _RearPos);

}

int	CRingBuffer::GetFreeSize()
{
	return _iBufferSize - GetUseSize() - 1;
}

void CRingBuffer::ClearBuffer(void)
{
	_FrontPos = 0;
	_RearPos = 0;
}


/// Todo
// Direct 함수 두 개 고치고 Peek 고치고 직접 뽑아쓰는 거 테스트하기

int	CRingBuffer::DirectEnqueueSize(void)
{
	if (_RearPos == _FrontPos && _FrontPos == 0)
	{
		return _iBufferSize - _RearPos - 1;
	}
	else if (_RearPos >= _FrontPos)
	{
		return _iBufferSize - _RearPos;
	}
	else
	{
		return _FrontPos - _RearPos - 1;
	}
}

int	CRingBuffer::DirectDequeueSize(void)
{
	if (_FrontPos <= _RearPos)
	{
		return _RearPos - _FrontPos;
	}
	else
	{
		return _iBufferSize - _FrontPos;
	}
}


int	CRingBuffer::Enqueue(const char* chpData, int iSize)
{
	// 넣고자 하는 데이터 크기보다 링버퍼에 남은 부분이 적은 경우
	int iFreeSize = GetFreeSize();
	if (iFreeSize < iSize)
		// + 로그 추가
		return 0;


	int copyRear = _RearPos;
	int copyFront = _FrontPos;
	int directSize = -1;
	if (copyRear == copyFront && copyFront == 0)
	{
		directSize = _iBufferSize - copyRear - 1;
	}
	else if (copyRear >= copyFront)
	{
		directSize = _iBufferSize - copyRear;
	}
	else
	{
		directSize = copyFront - copyRear - 1;
	}


	if (iSize <= directSize)	// 1. 넣을 데이터가 다이렉트인큐보다 작거나 같은 경우
								//		-> 한번에 다 넣기
	{
		memcpy_s(_pBuffer + copyRear, iSize, chpData, iSize);

	}
	else
	{							// 2. 넣을 데이터를 쪼개서 넣어야 하는 경우
		int left = iSize - directSize;
		memcpy_s(_pBuffer + copyRear, directSize, chpData, directSize);
		memcpy_s(_pBuffer, left, chpData + directSize, left);
	}

	_RearPos = (copyRear + iSize) % _iBufferSize;

	return iSize;
}


int	CRingBuffer::Dequeue(char* chpDest, int iSize)
{
	int nowUseSize = GetUseSize();
	if (nowUseSize == 0)	// 0. 줄 게 없는 상황
		return 0;


	// 지역변수로 뽑아놓고 마지막에 반영
	int copyRear = _RearPos;
	int copyFront = _FrontPos;
	int directSize = -1;
	if (copyFront <= copyRear)
	{
		directSize = copyRear - copyFront;
	}
	else
	{
		directSize = _iBufferSize - copyFront;
	}

	if (iSize <= nowUseSize)
	{
		if (iSize <= directSize)
		{
			memcpy_s(chpDest, iSize, _pBuffer + copyFront, iSize);
		}
		else
		{
			// 버퍼에 iSize 만큼 있지만 경계에 걸쳐있기 때문에 memcpy 두 번 
			memcpy_s(chpDest, directSize, _pBuffer + copyFront, directSize);
			int left = iSize - directSize;
			memcpy_s(chpDest + directSize, left, _pBuffer, left);

		}
		_FrontPos = (copyFront + iSize) % _iBufferSize;
		return iSize;
	}
	else
	{
		// _iUseSize가 iSize 보단 작지만 어쨋든 _iUseSize > 0 라서 있는 만큼만 제공
		if (nowUseSize <= directSize)
		{
			memcpy_s(chpDest, nowUseSize, _pBuffer + copyFront, nowUseSize);
		}
		else
		{
			// memcpy 두번
			memcpy_s(chpDest, directSize, _pBuffer + copyFront, directSize);
			int left = nowUseSize - directSize;
			memcpy_s(chpDest + directSize, left, _pBuffer, left);
		}

		_FrontPos = (copyFront + nowUseSize) % _iBufferSize;
		return nowUseSize;
	}
}

int	CRingBuffer::Peek(char* chpDest, int iSize)
{
	int nowUseSize = GetUseSize();
	if (nowUseSize == 0)
		return 0;

	// 지역변수로 뽑아놓고 마지막에 반영
	int copyRear = _RearPos;
	int copyFront = _FrontPos;
	int directSize = -1;
	if (copyFront <= copyRear)
	{
		directSize = copyRear - copyFront;
	}
	else
	{
		directSize = _iBufferSize - copyFront;
	}

	if (iSize <= nowUseSize)
	{
		if (iSize <= directSize)
		{
			memcpy_s(chpDest, iSize, _pBuffer + copyFront, iSize);
		}
		else
		{
			// 버퍼에 iSize 만큼 있지만 경계에 걸쳐있기 때문에 memcpy 두 번 
			memcpy_s(chpDest, directSize, _pBuffer + copyFront, directSize);
			int left = iSize - directSize;
			memcpy_s(chpDest + directSize, left, _pBuffer, left);

		}
		return iSize;
	}
	else
	{
		// _iUseSize가 iSize 보단 작지만 어쨋든 _iUseSize > 0 라서 있는 만큼만 제공
		if (nowUseSize <= directSize)
		{
			memcpy_s(chpDest, nowUseSize, _pBuffer + copyFront, nowUseSize);
		}
		else
		{
			// memcpy 두번
			memcpy_s(chpDest, directSize, _pBuffer + copyFront, directSize);
			int left = nowUseSize - directSize;
			memcpy_s(chpDest + directSize, left, _pBuffer, left);
		}

		return nowUseSize;
	}

}

int	CRingBuffer::MoveRear(int iSize)
{
	int iFreeSize = GetFreeSize();
	if (iFreeSize < iSize)
		return 0;

	_RearPos = (_RearPos + iSize) % _iBufferSize;
	return iSize;
}

int	CRingBuffer::MoveFront(int iSize)
{
	int iUseSize = GetUseSize();
	if (iUseSize < iSize)
		return 0;


	_FrontPos = (_FrontPos + iSize) % _iBufferSize;
	return iSize;
}

char* CRingBuffer::GetFrontPtr(void)
{
	return _pBuffer + _FrontPos;
}

char* CRingBuffer::GetRearPtr(void)
{
	return _pBuffer + _RearPos;
}
