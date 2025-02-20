#include <iostream>
#include <Windows.h>
#include "SerializeBuffer.h"

class CItem
{
public:
	void Setting(int type, char x, char y)
	{
		_type = type;
		_x = x;
		_y = y;
	}
private:
	int _type;
	char _x;
	char _y;
};

// 전역 오버로딩 연습
CMessage& operator << (CMessage& message, CItem itValue)
{
	message.PutData((char*)&itValue, sizeof(CItem));
	return message;
}
CMessage& operator >> (CMessage& message, CItem& itValue)
{
	message.GetData((char*)&itValue, sizeof(itValue));
	return message;
}


int main()
{
	unsigned int h = 9990;
	float a = 3.2f;
	CMessage Message;
	Message << 4;

	CItem itTemp;
	itTemp.Setting(5, 1, 1);
	Message << itTemp;

	int iValue;
	float fValue;
	char chValue;
	double dbValue;
	long lValue;
	DWORD dwValue;
	CItem ctct;
	unsigned int gggd;

	Message << 'a' << a << 888.666;
	Message << (long)7 << (DWORD)44 << h;

	try
	{
		Message >> iValue;
		Message >> ctct;
		Message >> chValue;
		Message >> fValue;
		Message >> dbValue;
		Message >> lValue >> dwValue >> gggd;
	}
	catch(...)
	{

		return 0;
	}
	std::cout << iValue << chValue << fValue << dbValue;
	return 0;
}