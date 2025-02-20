
#ifndef  __PACKET__
#define  __PACKET__

class CMessage
{
public:

	enum en_PACKET
	{
		eBUFFER_DEFAULT = 1400		// 패킷의 기본 버퍼 사이즈.
	};

	CMessage();
	CMessage(int iBufferSize);
	virtual	~CMessage();
	void	Clear(void);
	int	GetBufferSize(void) { return m_iBufferSize; }
	int		GetDataSize(void) { return m_iDataSize; }
	char* GetBufferReadPtr(void) { return _chpReadPos; }
	char* GetBufferWritePtr(void) { return _chpWritePos; }
	int	MoveWritePos(unsigned int iSize);
	int	MoveReadPos(unsigned int iSize);
	int Resize(int iSize);


	// 연산자오버로딩 
	CMessage& operator = (CMessage& clSrcMessage);

	// 넣기
	CMessage& operator << (char chValue);
	CMessage& operator << (unsigned char byValue);
	CMessage& operator << (short shValue);
	CMessage& operator << (unsigned short wValue);
	CMessage& operator << (int iValue);
	CMessage& operator << (unsigned int iValue);
	CMessage& operator << (long lValue);
	CMessage& operator << (DWORD dwValue);
	CMessage& operator << (float fValue);
	CMessage& operator << (__int64 iValue);
	CMessage& operator << (double dValue);

	// 빼기
	CMessage& operator >> (BYTE& byValue);
	CMessage& operator >> (char& chValue);
	CMessage& operator >> (short& shValue);
	CMessage& operator >> (WORD& wValue);
	CMessage& operator >> (int& iValue);
	CMessage& operator >> (unsigned int& iValue);
	CMessage& operator >> (long& lValue);
	CMessage& operator >> (DWORD& dwValue);
	CMessage& operator >> (float& fValue);
	CMessage& operator >> (__int64& iValue);
	CMessage& operator >> (double& dValue);



public:
	int		GetData(char* chpDest, int iSize);
	int		PutData(char* chpSrc, int iSrcSize);


protected:

	char* m_chpBuffer;
	int	m_iBufferSize;
	int	m_iDataSize;

	char* _chpReadPos;
	char* _chpWritePos;
};



#endif
