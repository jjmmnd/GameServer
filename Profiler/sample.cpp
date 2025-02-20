#include "sample.h"
#include <iostream>
#include <ctime>
#include <Windows.h>
#include "Profile.h"


int iCnt = 0;
int logic = 3;

void MainLogic()
{
	
	Profile p3("MainLogic");

	//logic1(3);
	//logic2();
	//logic3();
	//logic4();

	Sleep(1000);
}

void MainRander()
{
	Profile p2("MainRander");
	//Rander1();
	//Rander2();
	//Rander3();
	Sleep(500);
}
