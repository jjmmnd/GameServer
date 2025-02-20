#include <iostream>
#include <Windows.h>
#include "SerializeBuffer.h"

/*
* <todo>
* ������ �����ε����� int DWORD long Ȯ��
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
/// 1. ������ ���� �Լ�
/// </summary>
/// <returns>(int)������ ������</returns>
int	CMessage::GetData(char* chpDest, int iSize)
{
	// �䱸�� ����� �� Ŭ �� �� �� �ִ� ��ŭ ����
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
/// 1. ������ ���� �Լ�
/// 2. Resize ���
/// 3. defaultSize �� �б� �α�
/// </summary>
/// <returns>(int)������ ������</returns>
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
		* �޸�Ǯ �������� ���� Resize �Ǿ��� ��Ȳ�� Ƽ�� �� ���� �α� X
		* ����ϴ� bufferSize�� ����Ʈ���� ũ�ٶ�� �α׿� ��������
		--------------------------------------------------------------*/
		if (m_iDataSize > eBUFFER_DEFAULT)
		{
			/*------------------------
			* <todo>
			* ��������� ��Ȳ���� �α�
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
	* ������������� �α�
	------------------------*/

	return iSize;
}


/*------------------
* ������ �����ε�
------------------*/
CMessage& CMessage::operator = (CMessage& clSrcMessage)
{
	*this = clSrcMessage;
	return *this;
}

/*----------------------------
* ������ �ֱ�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
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
		* ��������� ��Ȳ���� �α�
		------------------------*/
	}
	return *this;
}


/*----------------------------
* ������ ����
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
