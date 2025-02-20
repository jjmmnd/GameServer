#pragma once

class CRingBuffer
{
public:

	CRingBuffer(int iBufferSize);
	CRingBuffer();
	~CRingBuffer();

	//==============================================
	// �� ������ ũ��
	int	GetBufferSize(void);

	//==============================================
	// ���� ������� �뷮
	int	GetUseSize(void);

	//==============================================
	// ���� ���ۿ� ���� �뷮
	int	GetFreeSize(void);

	//==============================================
	// _RearPos�� ������ ����
	int	Enqueue(const char* chpData, int iSize);

	//==============================================
	// _FrontPos���� ������ �о�� (������ �̵� O)
	int	Dequeue(char* chpDest, int iSize);

	//==============================================
	// _FrontPos���� ������ �о�� (������ �̵� X)
	int	Peek(char* chpDest, int iSize);

	//==============================================
	// ������ ��� ������ �ʱ�ȭ
	void ClearBuffer(void);


public:
	//====================================================
	// ���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����
	int	DirectEnqueueSize(void);
	int	DirectDequeueSize(void);

	//====================================================
	// ���ϴ� ���̸�ŭ �б�, ���� ��ġ���� �̵�
	int	MoveRear(int iSize);
	int	MoveFront(int iSize);

	//====================================================
	// _FrontPos ���
	char* GetFrontPtr(void);

	//====================================================
	// _RearPos ���
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

