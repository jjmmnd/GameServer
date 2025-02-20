
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
// Direct �Լ� �� �� ��ġ�� Peek ��ġ�� ���� �̾ƾ��� �� �׽�Ʈ�ϱ�

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
	// �ְ��� �ϴ� ������ ũ�⺸�� �����ۿ� ���� �κ��� ���� ���
	int iFreeSize = GetFreeSize();
	if (iFreeSize < iSize)
		// + �α� �߰�
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


	if (iSize <= directSize)	// 1. ���� �����Ͱ� ���̷�Ʈ��ť���� �۰ų� ���� ���
								//		-> �ѹ��� �� �ֱ�
	{
		memcpy_s(_pBuffer + copyRear, iSize, chpData, iSize);

	}
	else
	{							// 2. ���� �����͸� �ɰ��� �־�� �ϴ� ���
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
	if (nowUseSize == 0)	// 0. �� �� ���� ��Ȳ
		return 0;


	// ���������� �̾Ƴ��� �������� �ݿ�
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
			// ���ۿ� iSize ��ŭ ������ ��迡 �����ֱ� ������ memcpy �� �� 
			memcpy_s(chpDest, directSize, _pBuffer + copyFront, directSize);
			int left = iSize - directSize;
			memcpy_s(chpDest + directSize, left, _pBuffer, left);

		}
		_FrontPos = (copyFront + iSize) % _iBufferSize;
		return iSize;
	}
	else
	{
		// _iUseSize�� iSize ���� ������ ��¶�� _iUseSize > 0 �� �ִ� ��ŭ�� ����
		if (nowUseSize <= directSize)
		{
			memcpy_s(chpDest, nowUseSize, _pBuffer + copyFront, nowUseSize);
		}
		else
		{
			// memcpy �ι�
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

	// ���������� �̾Ƴ��� �������� �ݿ�
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
			// ���ۿ� iSize ��ŭ ������ ��迡 �����ֱ� ������ memcpy �� �� 
			memcpy_s(chpDest, directSize, _pBuffer + copyFront, directSize);
			int left = iSize - directSize;
			memcpy_s(chpDest + directSize, left, _pBuffer, left);

		}
		return iSize;
	}
	else
	{
		// _iUseSize�� iSize ���� ������ ��¶�� _iUseSize > 0 �� �ִ� ��ŭ�� ����
		if (nowUseSize <= directSize)
		{
			memcpy_s(chpDest, nowUseSize, _pBuffer + copyFront, nowUseSize);
		}
		else
		{
			// memcpy �ι�
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
