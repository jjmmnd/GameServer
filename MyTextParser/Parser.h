#pragma once


class CParser
{
public:
	CParser();
	~CParser();

	BOOL LoadFile(const char* FileName);

	/*----------------------------------
	* �����̽�, ��, �����ڵ�, �ּ� ó��.
	* Parameters: ����.
	* Return : (BOOL)TRUE, FALSE.
	----------------------------------*/
	BOOL SkipNoneCommand(char** chppBuffer);

	/*--------------------------------------------------------------------------
	* ���� �ܾ� ���.
	* Parameters: (char**)���� �ܾ��� ���� ��ġ(����). (int*)���� �ܾ��� ����(����).
	* Return : (BOOL)TRUE, FALSE.
	---------------------------------------------------------------------------*/
	BOOL GetNextWord(char** chppBuffer, int* ipLength);

	/*--------------------------------------------------------------------------
	* ���ڿ� ���(�� ~~~~��)
	* Parameters: (char**)���� �ܾ��� ���� ��ġ(����). (int*)���� �ܾ��� ����(����).
	* Return : (BOOL)TRUE, FALSE.
	---------------------------------------------------------------------------*/
	// BOOL GetStringWord(char** chppBuffer, int* ipLength);


	// ����1: ã���� �ϴ� �׸� �̸� / ����2 : �����͸� ���� ������.
	BOOL GetValue(const char* key, int* ipValue);
	BOOL GetValue(const char* key, float* ipValue);
	BOOL GetValue(const char* key, WCHAR* ipValue);


private:
	FILE* _pFile;
	char* _chpBegin;

};
