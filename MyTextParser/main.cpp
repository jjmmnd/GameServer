#include <iostream>
#include <Windows.h>
#include "Parser.h"
#include <locale.h>

int main()
{
	_wsetlocale(LC_ALL, L"Korean");

	CParser Parser;
	int iValue;
	float fID;
	int iPort;
	int iThreadCnt;
	WCHAR buf[256];
	Parser.LoadFile("parserTEST.cnf");
	Parser.GetValue("Version", &iValue);
	Parser.GetValue("ServerID", &fID);
	Parser.GetValue("ServerBindPort", &iPort);
	Parser.GetValue("WorkerThread", &iThreadCnt);
	Parser.GetValue("ServerBindIP", buf);

	printf("%d %f %d %d ", iValue, fID, iPort, iThreadCnt);
	wprintf(L"%s", buf);

	return 0;
}