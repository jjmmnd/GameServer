#include <Windows.h>
#include "Profile.h"
#include "sample.h"
#include <iostream>
#include <conio.h>


int main()
{

	Initial();


	while (1)
	{
		/*
		// �Էº�
		if (_kbhit())	// Ű �Է��� �ִٸ� 1�� ��ȯ�ϰ� ���ٸ� 0�� ��ȯ 
		{
			// �ܼ�Ű�� ���� ���� ��� 
			// � Ű�� ������ �������� ���� ��� ���� 
			// Ű���� �Է¹ް� ����� ������ �ʴ� �Լ� 
			int iChar = _getch();

			if (iChar == 'p' || iChar == 'P')
				ProfileDataOutText(SetFileName());

			if (iChar == 'i' || iChar == 'I')
				ProfileReset();

		}*/
		if (GetAsyncKeyState(VK_SHIFT))
			DataOutText(SetFileName());

		// ����
		MainLogic();

		// ����
		MainRander();
	}
}