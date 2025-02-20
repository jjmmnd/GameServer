#pragma once


class CParser
{
public:
	CParser();
	~CParser();

	BOOL LoadFile(const char* FileName);

	/*----------------------------------
	* 스페이스, 탭, 엔터코드, 주석 처리.
	* Parameters: 없음.
	* Return : (BOOL)TRUE, FALSE.
	----------------------------------*/
	BOOL SkipNoneCommand(char** chppBuffer);

	/*--------------------------------------------------------------------------
	* 다음 단어 얻기.
	* Parameters: (char**)다음 단어의 시작 위치(받음). (int*)다음 단어의 길이(받음).
	* Return : (BOOL)TRUE, FALSE.
	---------------------------------------------------------------------------*/
	BOOL GetNextWord(char** chppBuffer, int* ipLength);

	/*--------------------------------------------------------------------------
	* 문자열 얻기(“ ~~~~”)
	* Parameters: (char**)다음 단어의 시작 위치(받음). (int*)다음 단어의 길이(받음).
	* Return : (BOOL)TRUE, FALSE.
	---------------------------------------------------------------------------*/
	// BOOL GetStringWord(char** chppBuffer, int* ipLength);


	// 인자1: 찾고자 하는 항목 이름 / 인자2 : 데이터를 넣을 포인터.
	BOOL GetValue(const char* key, int* ipValue);
	BOOL GetValue(const char* key, float* ipValue);
	BOOL GetValue(const char* key, WCHAR* ipValue);


private:
	FILE* _pFile;
	char* _chpBegin;

};
