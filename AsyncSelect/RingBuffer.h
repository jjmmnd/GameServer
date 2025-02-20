#pragma once

class CRingBuffer
{
public:

	CRingBuffer(int iBufferSize);
	CRingBuffer();
	~CRingBuffer();

	//==============================================
	// 총 버퍼의 크기
	int	GetBufferSize(void);

	//==============================================
	// 현재 사용중인 용량
	int	GetUseSize(void);

	//==============================================
	// 현재 버퍼에 남은 용량
	int	GetFreeSize(void);

	//==============================================
	// _RearPos에 데이터 넣음
	int	Enqueue(const char* chpData, int iSize);

	//==============================================
	// _FrontPos에서 데이터 읽어옴 (포지션 이동 O)
	int	Dequeue(char* chpDest, int iSize);

	//==============================================
	// _FrontPos에서 데이터 읽어옴 (포지션 이동 X)
	int	Peek(char* chpDest, int iSize);

	//==============================================
	// 버퍼의 모든 데이터 초기화
	void ClearBuffer(void);


public:
	//====================================================
	// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이
	int	DirectEnqueueSize(void);
	int	DirectDequeueSize(void);

	//====================================================
	// 원하는 길이만큼 읽기, 쓰기 위치에서 이동
	int	MoveRear(int iSize);
	int	MoveFront(int iSize);

	//====================================================
	// _FrontPos 얻기
	char* GetFrontPtr(void);

	//====================================================
	// _RearPos 얻기
	char* GetRearPtr(void);


private:
	enum en_BUFFER
	{
		eBUFFER_DEFAULT = 10000
	};

    char* _pBuffer;
    int _RearPos;
    int _FrontPos;
	int _iBufferSize;
};

