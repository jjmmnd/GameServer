#include <iostream>
#include <Windows.h>
#include "SerializeBuffer.h"

/*
* <todo>
* 연산자 오버로딩에서 int DWORD long 확인
*/

CMessage::CMessage()
{
	m_chpBuffer = (char*)malloc(eBUFFER_DEFAULT);
	m_iBufferSize = eBUFFER_DEFAULT;
	m_iDataSize = 0;
	_chpReadPos = m_chpBuffer;
	_chpWritePos = m_chpBuffer;
}

CMessage::CMessage(int iBufferSize)
{
	m_chpBuffer = (char*)malloc(iBufferSize);
	m_iBufferSize = iBufferSize;
	m_iDataSize = 0;
	_chpReadPos = m_chpBuffer;
	_chpWritePos = m_chpBuffer;
}

CMessage::~CMessage()
{
	Clear();
}

void CMessage::Clear()
{
	free(m_chpBuffer);
	m_iBufferSize = 0;
	m_iDataSize = 0;
}


int CMessage::MoveWritePos(unsigned int iSize)
{
	_chpWritePos += iSize;
	m_iDataSize += iSize;
	return iSize;
}
int CMessage::MoveReadPos(unsigned int iSize)
{
	_chpReadPos += iSize;
	m_iDataSize -= iSize;
	return iSize;
}


/// <summary>
/// 1. 데이터 추출 함수
/// </summary>
/// <returns>(int)복사한 사이즈</returns>
int	CMessage::GetData(char* chpDest, int iSize)
{
	// 요구한 사이즈가 더 클 때 줄 수 있는 만큼 제공
	if (iSize > m_iDataSize)
	{
		memcpy_s(chpDest, iSize, _chpReadPos, m_iDataSize);
		MoveReadPos(m_iDataSize);
		return m_iDataSize;
	}
	else
	{
		memcpy_s(chpDest, iSize, _chpReadPos, iSize);
		MoveReadPos(iSize);
		return iSize;
	}
}


/// <summary>
/// 1. 데이터 삽입 함수
/// 2. Resize 기능
/// 3. defaultSize 비교 분기 로그
/// </summary>
/// <returns>(int)복사한 사이즈</returns>
int	CMessage::PutData(char* chpSrc, int iSrcSize)
{

	if (iSrcSize > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
		PutData(chpSrc, iSrcSize);
	}
	else
	{
		memcpy_s(_chpWritePos, (m_chpBuffer + m_iBufferSize) - _chpWritePos, chpSrc, iSrcSize);
		MoveWritePos(iSrcSize);

		/*-------------------------------------------------------------
		* 메모리풀 재사용으로 가면 Resize 되야할 상황에 티가 안 나서 로그 X
		* 사용하는 bufferSize가 디폴트보다 크다라면 로그에 찍혀야함
		--------------------------------------------------------------*/
		if (m_iDataSize > eBUFFER_DEFAULT)
		{
			/*------------------------
			* <todo>
			* 리사이즈될 상황임을 로그
			------------------------*/
		}

		return iSrcSize;
	}
}


int CMessage::Resize(int iSize)
{
	char* chpNewBuffer = (char*)malloc(iSize);
	memcpy_s(chpNewBuffer, iSize, m_chpBuffer, m_iBufferSize);
	_chpWritePos = chpNewBuffer + (_chpWritePos - m_chpBuffer);
	_chpReadPos = chpNewBuffer + (_chpReadPos - m_chpBuffer);
	m_iBufferSize = iSize;
	free(m_chpBuffer);
	m_chpBuffer = chpNewBuffer;

	/*------------------------
	* <todo>
	* 리사이즈됐음을 로그
	------------------------*/

	return iSize;
}


/*------------------
* 연산자 오버로딩
------------------*/
CMessage& CMessage::operator = (CMessage& clSrcMessage)
{
	*this = clSrcMessage;
	return *this;
}

/*----------------------------
* 데이터 넣기
-----------------------------*/
CMessage& CMessage::operator << (unsigned char byValue)
{
	if (sizeof(byValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(unsigned char*)_chpWritePos = byValue;
	MoveWritePos(sizeof(byValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}
CMessage& CMessage::operator << (char chValue)
{
	if (sizeof(chValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(char*)_chpWritePos = chValue;
	MoveWritePos(sizeof(chValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}

CMessage& CMessage::operator << (short shValue)
{
	if (sizeof(shValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(short*)_chpWritePos = shValue;
	MoveWritePos(sizeof(shValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}

CMessage& CMessage::operator << (unsigned short wValue)
{
	if (sizeof(wValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(unsigned short*)_chpWritePos = wValue;
	MoveWritePos(sizeof(wValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}

CMessage& CMessage::operator << (int iValue)
{
	if (sizeof(iValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(int*)_chpWritePos = iValue;
	MoveWritePos(sizeof(iValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}
CMessage& CMessage::operator << (unsigned int iValue)
{
	if (sizeof(iValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(int*)_chpWritePos = iValue;
	MoveWritePos(sizeof(iValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}

CMessage& CMessage::operator << (long lValue)
{
	if (sizeof(lValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(long*)_chpWritePos = lValue;
	MoveWritePos(sizeof(lValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}

CMessage& CMessage::operator << (DWORD dwValue)
{
	if (sizeof(dwValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(long*)_chpWritePos = dwValue;
	MoveWritePos(sizeof(dwValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}

CMessage& CMessage::operator << (float fValue)
{
	if (sizeof(fValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(float*)_chpWritePos = fValue;
	MoveWritePos(sizeof(fValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}

CMessage& CMessage::operator << (__int64 iValue)
{
	if (sizeof(iValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(__int64*)_chpWritePos = iValue;
	MoveWritePos(sizeof(iValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}
CMessage& CMessage::operator << (double dValue)
{
	if (sizeof(dValue) > (m_chpBuffer + m_iBufferSize) - _chpWritePos)
	{
		Resize(m_iBufferSize * 2);
	}
	*(double*)_chpWritePos = dValue;
	MoveWritePos(sizeof(dValue));
	if (m_iDataSize > eBUFFER_DEFAULT)
	{
		/*------------------------
		* <todo>
		* 리사이즈될 상황임을 로그
		------------------------*/
	}
	return *this;
}


/*----------------------------
* 데이터 빼기
-----------------------------*/
CMessage& CMessage::operator >> (BYTE& byValue)
{
	if (sizeof(byValue) > m_iDataSize)
	{
		throw;
	}
	byValue = *(unsigned char*)_chpReadPos;
	MoveReadPos(sizeof(byValue));
	return *this;
}
CMessage& CMessage::operator >> (char& chValue)
{
	if (sizeof(chValue) > m_iDataSize)
	{
		throw;
	}
	chValue = *(char*)_chpReadPos;
	MoveReadPos(sizeof(chValue));
	return *this;
}

CMessage& CMessage::operator >> (short& shValue)
{
	if (sizeof(shValue) > m_iDataSize)
	{
		throw;
	}
	shValue = *(short*)_chpReadPos;
	MoveReadPos(sizeof(shValue));
	return *this;
}

CMessage& CMessage::operator >> (WORD& wValue)
{
	if (sizeof(wValue) > m_iDataSize)
	{
		throw;
	}
	wValue = *(unsigned short*)_chpReadPos;
	MoveReadPos(sizeof(wValue));
	return *this;
}

CMessage& CMessage::operator >> (int& iValue)
{
	if (sizeof(iValue) > m_iDataSize)
	{
		throw;
	}
	iValue = *(int*)_chpReadPos;
	MoveReadPos(sizeof(iValue));
	return *this;
}

CMessage& CMessage::operator >> (unsigned int& iValue)
{
	if (sizeof(iValue) > m_iDataSize)
	{
		throw;
	}
	iValue = *(int*)_chpReadPos;
	MoveReadPos(sizeof(iValue));
	return *this;
}

CMessage& CMessage::operator >> (long& lValue)
{
	if (sizeof(lValue) > m_iDataSize)
	{
		throw;
	}
	lValue = *(unsigned int*)_chpReadPos;
	MoveReadPos(sizeof(lValue));
	return *this;
}

CMessage& CMessage::operator >> (DWORD& dwValue)
{
	if (sizeof(dwValue) > m_iDataSize)
	{
		throw;
	}
	dwValue = *(unsigned int*)_chpReadPos;
	MoveReadPos(sizeof(dwValue));
	return *this;
}
CMessage& CMessage::operator >> (float& fValue)
{
	if (sizeof(fValue) > m_iDataSize)
	{
		throw;
	}
	fValue = *(float*)_chpReadPos;
	MoveReadPos(sizeof(fValue));
	return *this;
}

CMessage& CMessage::operator >> (__int64& iValue)
{
	if (sizeof(iValue) > m_iDataSize)
	{
		throw;
	}
	iValue = *(__int64*)_chpReadPos;
	MoveReadPos(sizeof(iValue));
	return *this;
}
CMessage& CMessage::operator >> (double& dValue)
{
	if (sizeof(dValue) > m_iDataSize)
	{
		throw;
	}
	dValue = *(double*)_chpReadPos;
	MoveReadPos(sizeof(dValue));
	return *this;
}
