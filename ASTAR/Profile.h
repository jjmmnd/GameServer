#pragma once

struct stPROFILE
{
	int			lFlag;				
	char			szName[64];			

	LARGE_INTEGER	lStartTime;			

	__int64			iTotalTime;			
	__int64			iMin[2];			
	__int64			iMax[2];			

	__int64			iCall;				

};

enum class callType
{
	START = 1, END
};


class Profile
{
public:
	Profile(const char* tag);
	~Profile();
	const char* _tag;
};

void Begin(const char* tag);
void End(const char* tag);

void DataOutText(const char* szFileName);
void ProfileReset();


//---------------------------------
// 초기화 함수 정의 
//---------------------------------
void Initial();								// QueryPerformanceFrequency 초기화
const char* SetFileName();					// 로깅 파일명 세팅