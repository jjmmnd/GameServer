#pragma once

#define dfSCREEN_WIDTH	81		// ÄÜ¼Ö °¡·Î 80Ä­ + NULL
#define dfSCREEN_HEIGHT	24		// ÄÜ¼Ö ¼¼·Î 25Ä­ + NULL

// ½Ì±ÛÅæ
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