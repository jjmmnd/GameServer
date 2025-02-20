#pragma once

#include <Windows.h>
#include <DbgHelp.h>
#include <crtdbg.h>
#include <Psapi.h>
#pragma comment(lib,"DbgHelp.lib")

class CCrashDump
{
public:
	CCrashDump()
	{
		_DumpCount = 0;
		_invalid_parameter_handler oldHandler;
		_invalid_parameter_handler newHandler;
		newHandler = myInvalidPrameterHandler;
		oldHandler = _set_invalid_parameter_handler(newHandler);	// crt 함수에 null 포인터 등을 넣었을 때
		_CrtSetReportMode(_CRT_WARN, 0);						// CRT 오류 메시지 표시 중단, 바로 덤프로 남도록
		_CrtSetReportMode(_CRT_ASSERT, 0);						// CRT 오류 메시지 표시 중단, 바로 덤프로 남도록
		_CrtSetReportMode(_CRT_ERROR, 0);						// CRT 오류 메시지 표시 중단, 바로 덤프로 남도록

		_CrtSetReportHook(_custom_Report_hook);
		
		//---------------------------------------------------------------------------
		// pure virtual function called 에러 핸들러를 사용자 정의 함수로 우회시킨다
		//---------------------------------------------------------------------------
		_set_purecall_handler(myPurecallHandler);

		SetHandlerDump(); // 모든 예외 다 캐치하는 API 래핑
	}

	static void Crash()
	{
		int* p = nullptr;
		*p = 0;
	}

	static LONG WINAPI MyExceptionFilter(__in PEXCEPTION_POINTERS pExceptionPointer)
	{
		SYSTEMTIME stNowTime;

		long DumpCount = InterlockedIncrement(&_DumpCount);

		//-----------------------------------------------------------------
		// 현재 날짜와 시간을 알아온다
		//-----------------------------------------------------------------
		WCHAR filename[MAX_PATH];
		GetLocalTime(&stNowTime);
		wsprintf(filename, L"Dump_%d%02d%02d_%02d.%02d.%02d_%d.dmp",
			stNowTime.wYear, stNowTime.wMonth, stNowTime.wDay, stNowTime.wHour, stNowTime.wMinute, stNowTime.wSecond, DumpCount);
		wprintf(L"\n\n\n!! Crash Error !! %d.%d.%d / %d:%d:%d\n",
			stNowTime.wYear, stNowTime.wMonth, stNowTime.wDay, stNowTime.wHour, stNowTime.wMinute, stNowTime.wSecond);
		wprintf(L"Now Save dump file...\n");


		HANDLE hDumpFile = ::CreateFile(filename,
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		if (hDumpFile != INVALID_HANDLE_VALUE)
		{
			_MINIDUMP_EXCEPTION_INFORMATION MinidumpExceptioninformation;
			MinidumpExceptioninformation.ThreadId = ::GetCurrentThreadId();
			MinidumpExceptioninformation.ExceptionPointers = pExceptionPointer;
			MinidumpExceptioninformation.ClientPointers = TRUE;

			MiniDumpWriteDump(GetCurrentProcess(),
				GetCurrentProcessId(),
				hDumpFile,
				MiniDumpWithFullMemory,
				&MinidumpExceptioninformation,
				NULL,
				NULL);

			CloseHandle(hDumpFile);
			wprintf(L"CrashDump Save Finish !\n");
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}

	static void SetHandlerDump()
	{
		SetUnhandledExceptionFilter(MyExceptionFilter);
	}

	// Invalid Parameter handler
	static void myInvalidPrameterHandler(const WCHAR* expression, const WCHAR* function, const WCHAR* file, unsigned int line, uintptr_t pReserved)
	{
		Crash();
	}

	static int _custom_Report_hook(int ireposttype, char* message, int* returnvalue)
	{
		Crash();
		return true;
	}
	static void myPurecallHandler()
	{
		Crash();
	}

	static long _DumpCount;
};

long CCrashDump::_DumpCount = 0;