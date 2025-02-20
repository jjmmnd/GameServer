
#include <Windows.h>
#include <iostream>
#include "Parser.h"

CParser::CParser()
{

}

CParser::~CParser()
{
	free(_chpBegin);
}

BOOL CParser::LoadFile(const char* FileName)
{
	errno_t err = fopen_s(&_pFile, FileName, "rb");
	if (err != 0)
	{
		wprintf(L"File open error\n");
		return false;
	}
	fseek(_pFile, 0, SEEK_END);
	long fileSize = ftell(_pFile);
	fseek(_pFile, 0, SEEK_SET);
	_chpBegin = (char*)malloc(fileSize);
	fread(_chpBegin, 1, fileSize, _pFile);
	fclose(_pFile);
}

BOOL CParser::SkipNoneCommand(char** chppBuffer)
{
	while (1)
	{
		if (**chppBuffer == '/')
		{
			// "//"으로 시작하는 경우 캐리지 리턴까지 쭉 건너띄기
			if (*((*chppBuffer) + 1) == '/')
			{
				(*chppBuffer)++;
				(*chppBuffer)++;
				while (**chppBuffer != 0x0d)
				{
					(*chppBuffer)++;
				}
				(*chppBuffer)++;
				continue;
			}
			// "/*"으로 시작하는 경우 "*//"까지 건너띄기
			else if (*((*chppBuffer) + 1) == '*')
			{
				(*chppBuffer)++;
				(*chppBuffer)++;
				while (1)
				{
					if (**chppBuffer == '*')
					{
						if (*((*chppBuffer) + 1) == '/')
						{
							(*chppBuffer)++;
							(*chppBuffer)++;
							break;
						}
					}
					(*chppBuffer)++;
				}
				continue;
			}
		}

		if (**chppBuffer == ',' || **chppBuffer == '"' || **chppBuffer == 0x20 ||
			**chppBuffer == 0x08 || **chppBuffer == 0x09 || **chppBuffer == 0x0a ||
			**chppBuffer == 0x0d || **chppBuffer == '{' || **chppBuffer == '}')
		{
			(*chppBuffer)++;
			continue;
		}

		break; // 어떤 continue에도 안 걸렸으면 단어 시작 위치인 것
	}

	return true;
}


/*--------------------------------------------------------------------------
* 다음 단어 얻기.
* Parameters: (char**)다음 단어의 시작 위치(받음). (int*)다음 단어의 길이(받음).
* Return : (BOOL)TRUE, FALSE.
---------------------------------------------------------------------------*/
BOOL CParser::GetNextWord(char** chppBuffer, int* ipLength)
{
	// chppBuffer에 다음단어랑 길이를 줘야함 
	char* chpBuffer = nullptr;
	int iLength = 0;
	if (SkipNoneCommand(chppBuffer))
	{
		// *chppBuffer는 다음 단어의 시작 위치가 됨 
		chpBuffer = *chppBuffer;
	}
	while (1)
	{
		if (chpBuffer == nullptr)
		{
			return false;
		}

		// 단어의 끝 찾기 
		if (*chpBuffer == ',' || *chpBuffer == '"' || *chpBuffer == 0x20 ||
			*chpBuffer == 0x08 || *chpBuffer == 0x09 || *chpBuffer == 0x0a ||
			*chpBuffer == 0x0d)
		{
			break;
		}
		chpBuffer++;
		iLength++;
	}
	*ipLength = iLength;

	return true;
}


// Parameters: (const char*)찾고자 하는 항목, int *outValue 
BOOL CParser::GetValue(const char* key, int* ipValue)
{
	char* chpBuff = _chpBegin;
	char chWord[256];
	int	iLength;
	while (GetNextWord(&chpBuff, &iLength)) // chpBuff는 다음단어의 시작위치 
	{
		// Word 버퍼에 찾은 단어를 저장한다.
		memset(chWord, 0, 256);
		memcpy(chWord, chpBuff, iLength);
		if (chWord[iLength] != '\0')
			chWord[iLength] = '\0';
		if (0 == strcmp(key, chWord))
		{
			chpBuff += iLength;
			if (GetNextWord(&chpBuff, &iLength))
			{
				memset(chWord, 0, 256);
				memcpy(chWord, chpBuff, iLength);
				if (0 == strcmp(chWord, "="))
				{
					chpBuff += iLength;
					if (GetNextWord(&chpBuff, &iLength))
					{
						memset(chWord, 0, 256);
						memcpy(chWord, chpBuff, iLength);
						if (chWord[iLength] != '\0')
							chWord[iLength] = '\0';
						*ipValue = atoi(chWord);
						return TRUE;
					}
					return FALSE;
				}
			}
			return FALSE;
		}
		chpBuff += iLength;
	}
	return FALSE;
}


BOOL CParser::GetValue(const char* key, float* fpValue)
{
	char* chpBuff = _chpBegin;
	char chWord[256];
	int	iLength;
	while (GetNextWord(&chpBuff, &iLength))
	{
		memset(chWord, 0, 256);
		memcpy(chWord, chpBuff, iLength);
		if (chWord[iLength] != '\0')
			chWord[iLength] = '\0';
		if (0 == strcmp(key, chWord))
		{
			chpBuff += iLength;
			if (GetNextWord(&chpBuff, &iLength))
			{
				memset(chWord, 0, 256);
				memcpy(chWord, chpBuff, iLength);
				if (0 == strcmp(chWord, "="))
				{
					chpBuff += iLength;
					if (GetNextWord(&chpBuff, &iLength))
					{
						memset(chWord, 0, 256);
						memcpy(chWord, chpBuff, iLength);
						if (chWord[iLength] != '\0')
							chWord[iLength] = '\0';
						*fpValue = atof(chWord);
						return TRUE;
					}
					return FALSE;
				}
			}
			return FALSE;
		}
		chpBuff += iLength;
	}
	return FALSE;
}

BOOL CParser::GetValue(const char* key, WCHAR* wchpValue)
{
	char* chpBuff = _chpBegin;
	char chWord[256];
	int	iLength;
	while (GetNextWord(&chpBuff, &iLength))
	{
		memset(chWord, 0, 256);
		memcpy(chWord, chpBuff, iLength);
		if (chWord[iLength] != '\0')
			chWord[iLength] = '\0';
		if (0 == strcmp(key, chWord))
		{
			chpBuff += iLength;
			if (GetNextWord(&chpBuff, &iLength))
			{
				memset(chWord, 0, 256);
				memcpy(chWord, chpBuff, iLength);
				if (0 == strcmp(chWord, "="))
				{
					chpBuff += iLength;
					if (GetNextWord(&chpBuff, &iLength))
					{
						memset(chWord, 0, 256);
						memcpy(chWord, chpBuff, iLength);
						if (chWord[iLength] != '\0')
							chWord[iLength] = '\0';

						WCHAR temp[256] = { 0, };
						size_t wchSZ = 256;
						mbstowcs_s(&wchSZ, temp, chWord, iLength);
						wcscpy_s(wchpValue, iLength * 2, temp);

						return TRUE;
					}
					return FALSE;
				}
			}
			return FALSE;
		}
		chpBuff += iLength;
	}
	return FALSE;
}