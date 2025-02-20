#pragma once

#define dfSCREEN_WIDTH	81		// �ܼ� ���� 80ĭ + NULL
#define dfSCREEN_HEIGHT	24		// �ܼ� ���� 25ĭ + NULL

// �̱���
class CScreenBuffer
{
private:
	CScreenBuffer();
	~CScreenBuffer();
public:
	static CScreenBuffer* GetInstance();
	void cs_MoveCursor(int iPosX, int iPosY);
	void cs_ClearScreen(void);
	void Buffer_Clear(void);
	void Buffer_Flip();
	void Sprite_Draw(int refX, int refY, char chSprite);

private:
	char _screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
};