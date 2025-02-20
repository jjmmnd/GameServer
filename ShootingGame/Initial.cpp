#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Initial.h"

// 전역 구조체 및 전역 변수들
st_PLAYER g_stPlayer;
st_Enemy g_stEnemy[ENEMY_COUNT];
st_Bullet g_stBullet[BULLET_MAX];

Stage_Info* st_stageInfo;	// 스테이지 파일 헤더
Enemy_Info* st_enemyInfo;	// 적군 파일 헤더
Enemy_Type* st_enemyType;	// 타입별 적군 속성
char* g_stageBuffer;

int g_stage;
int g_scene;
int g_typeCnt;
int g_stageCnt;

//--------------------------------------------------------------------
// 게임 시작시 전체 데이터 초기화용.
//--------------------------------------------------------------------
void Initial(void)
{
	srand((unsigned)time(NULL));

	// 씬 초기화
	// g_scene = TITLE;

	// 플레이어 초기화 
	InitPlayer();

	// 총알 초기화
	InitBullet();

	//------------------------------------
	// 적군 파일 헤더 읽기
	// 모든 적군타입 파일을 열어서(ReadEnemy) st_enemyType에 저장
	//------------------------------------ 
	ReadEnemyFileInfo();

	//-----------------------------------
	// 스테이지 파일 헤더 읽기
	// st_stageInfo에 스테이지 파일 이름 저장
	// 매 스테이지가 시작되기 전 해당 스테이지 파일을 연다
	// st_enemyType을 이용해 적군의 초기화(InitEnemy) 함께 진행 
	//-----------------------------------
	ReadStageFileInfo();
}


// 플레이어 초기화
void InitPlayer()
{
	//-------------------------------------------------------------------
	// 플레이어 최초 데이터 초기화.
	// HP : 5 
	// 39,19 위치에서 시작 함.
	//-------------------------------------------------------------------
	InitHP();
	g_stPlayer.iVisible = true;

	g_stPlayer.iX = 39;
	g_stPlayer.iY = 19;
}


void InitHP()
{
	g_stPlayer.iHP = 5;
}


// 총알 초기화
void InitBullet()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_stBullet[i].visible = false;
		g_stBullet[i].damage = 0;
	}
}



// 적군 타입 헤더 읽기
// 초기화 단계에서 호출
void ReadEnemyFileInfo()
{
	FILE* fPtr;
	fopen_s(&fPtr, "enemy.txt", "r");
	if (fPtr == NULL)
	{
		printf("enemy.txt file open fail\n");
		return;
	}

	fseek(fPtr, 0, SEEK_END);
	int fileSize = ftell(fPtr);
	fseek(fPtr, 0, SEEK_SET);

	// 헤더 파일의 크기+1의 동적 버퍼 생성 
	char* enemyBuffer = (char*)malloc(fileSize + 1);	// 널문자 추가를 위한 +1
	if (enemyBuffer == NULL) {
		printf("Memory allocation failed\n");
		fclose(fPtr);
		return;
	}

	fread(enemyBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	enemyBuffer[fileSize] = '\n';

	/*******적군 타입 헤더를 동적할당으로 읽기 완료********/


	char* bufferPtr = enemyBuffer;
	char* context = NULL;
	char* token = strtok_s(bufferPtr, "\n", &context);
	int enemyFileCnt = atoi(token);		// 첫번째 줄에서 파일의 개수를 얻음
	bufferPtr = NULL;

	// 정적 변수에 타입의 개수 저장 
	g_typeCnt = enemyFileCnt;

	// 파일의 개수만큼 동적할당
	st_enemyInfo = (Enemy_Info*)malloc(sizeof(Enemy_Info) * enemyFileCnt);
	if (st_enemyInfo == NULL)
	{
		printf("Memory Allocation fail\n");
		free(enemyBuffer);
		return;
	}

	// 파일의 개수만큼 파일 이름을 헤더에 저장 
	for (int i = 0; i < enemyFileCnt; i++)
	{
		token = strtok_s(NULL, "\n", &context);
		// 예외처리
		if (token == NULL)
		{
			printf("Fail to read enemyName\n");
			free(enemyBuffer);
			free(st_enemyInfo);
			return;
		}
		(st_enemyInfo + i)->enemyNum = i + 1;
		strcpy_s((st_enemyInfo + i)->enemyName, 256, token);
	}

	free(enemyBuffer);

	ReadEnemy();
}

/*
* 각각의 적군 정보 읽고 전역 구조체에 저장하기
* 초기화 단계에서 ReadEnemyFileInfo에 의해 호출되는 함수
*/
void ReadEnemy()
{
	st_enemyType = (Enemy_Type*)malloc(sizeof(Enemy_Type) * g_typeCnt);
	if (st_enemyType == NULL)
	{
		printf("Enemy Memory Allocaiton fail\n");
		return;
	}

	FILE* fPtr;

	for (int i = 0; i < g_typeCnt; i++)
	{
		fopen_s(&fPtr, st_enemyInfo[i].enemyName, "r");
		if (fPtr == NULL)
		{
			printf("enemy%d.txt file open fail\n", st_enemyInfo[i].enemyNum);
			return;
		}

		fseek(fPtr, 0, SEEK_END);
		int fileSize = ftell(fPtr);
		fseek(fPtr, 0, SEEK_SET);

		char* enemyBuffer = (char*)malloc(fileSize + 1);		// 적군 개별의 정보를 저장할 임시 버퍼 
		if (enemyBuffer == NULL)
		{
			printf("EnemyBuffer Memory Allocation fail\n");
			fclose(fPtr);
			return;
		}

		fread(enemyBuffer, 1, fileSize, fPtr);
		fclose(fPtr);
		enemyBuffer[fileSize] = '\n';


		// 전역 구조체에 복사
		char* token = enemyBuffer;
		char* context = NULL;
		st_enemyType[i].type = *token;
		token = strtok_s(token, "\n", &context); // 다음 줄로 이동

		token = strtok_s(NULL, "\n", &context);
		if (token != NULL)
			st_enemyType[i].dirX = atoi(token);
		token = strtok_s(NULL, "\n", &context);
		if (token != NULL)
			st_enemyType[i].dirY = atoi(token);
		token = strtok_s(NULL, "\n", &context);
		if (token != NULL)
			st_enemyType[i].damage = atoi(token);


		// 복사 후 힙 free
		free(enemyBuffer);
	}
}


// 초기화 단계에서 호출
// stage 파일 이름은 stage 순서대로 입력되어 있어야 함 
void ReadStageFileInfo()
{
	FILE* fPtr;
	fopen_s(&fPtr, "stage.txt", "r");
	if (fPtr == NULL)
	{
		printf("stage.txt file open fail\n");
		return;
	}

	fseek(fPtr, 0, SEEK_END);
	int fileSize = ftell(fPtr);
	fseek(fPtr, 0, SEEK_SET);

	char* infoBuffer = (char*)malloc(fileSize + 1);	// 널문자 추가를 위한 +1
	if (infoBuffer == NULL) {
		printf("Memory allocation failed\n");
		fclose(fPtr);
		return;
	}

	fread(infoBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	infoBuffer[fileSize] = '\n';
	/*******stage header파일을 동적할당으로 읽기 완료********/


	char* bufferPtr = infoBuffer;
	char* context = NULL;

	char* token = strtok_s(bufferPtr, "\n", &context);
	int stageFileCnt = atoi(token);		// 첫번째 줄에서 stage 파일의 개수를 얻음

	bufferPtr = NULL;

	g_stageCnt = stageFileCnt;

	// 파일의 개수만큼 동적할당
	st_stageInfo = (Stage_Info*)malloc(sizeof(Stage_Info) * stageFileCnt);
	if (st_stageInfo == NULL)
	{
		printf("malloc fail");
		free(infoBuffer);
		return;
	}

	for (int i = 0; i < stageFileCnt; i++)
	{
		token = strtok_s(NULL, "\n", &context);
		// 예외처리
		if (token == NULL)
		{
			printf("Failed to read enemy name\n");
			free(infoBuffer);
			free(st_stageInfo);
			return;
		}
		(st_stageInfo + i)->stageNum = i + 1;
		strcpy_s((st_stageInfo + i)->stageName, 256, token);
	}

	free(infoBuffer);
}


//------------------------------
// LOAD 단계에서 호출되는 함수
//------------------------------
void ReadStage()
{
	const char* stageName = st_stageInfo[g_stage - 1].stageName;

	FILE* fPtr;
	fopen_s(&fPtr, stageName, "r");		// 텍스트모드로 적 배치 파일 읽어오기 
	if (fPtr == NULL)
	{
		printf("stage%d.txt file open fail", st_stageInfo[g_stage - 1].stageNum);
		return;
	}

	fseek(fPtr, 0, SEEK_END);
	int fileSize = ftell(fPtr);
	fseek(fPtr, 0, SEEK_SET);

	// 전역 배열 버퍼에 해당 스테이지 적군의 배치를 미리 저장해둔다 
	g_stageBuffer = (char*)malloc(fileSize + 1);	// 널문자 추가를 위한 +1
	if (g_stageBuffer == NULL) {
		printf("Memory allocation failed\n");
		fclose(fPtr);
		return;
	}

	fread(g_stageBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	g_stageBuffer[fileSize] = '\n';

	/*************동적으로 stage를 할당 완료 *******************/

	InitEnemy(fileSize + 1);
}


// 적군 초기화
// 적군의 배열을 세팅
void InitEnemy(int fileSize)
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		g_stEnemy[i].visible = false;
	}

	int posX = 0;
	int posY = 0;
	int k = 0;

	// 전역 배열 버퍼를 보면서 적군의 좌표 세팅
	char* bPtr = g_stageBuffer;

	for (int i = 0; i < dfSCREEN_HEIGHT - 1; i++)
	{
		while (*bPtr != '\n')
		{
			if (*bPtr != ' ' && *bPtr != '\\')
			{
				g_stEnemy[k].visible = true;
				g_stEnemy[k].type = *bPtr;	// 적의 위치에 타입이 저장되어 있기 때문에 type에 추가 
				g_stEnemy[k].enemyX = posX;	// 좌표 세팅
				g_stEnemy[k].enemyY = posY;
				SettingAttribute(k, g_stEnemy[k].type);
				k++;
			}
			posX++;
			bPtr++;
		}
		posX = 0;
		posY++;
		bPtr++;
	}
}

void SettingAttribute(int index, char type)
{

	for (int i = 0; i < g_typeCnt; i++)
	{
		if (g_stEnemy[index].type == st_enemyType[i].type)
		{
			g_stEnemy[index].dirX = st_enemyType[i].dirX;
			g_stEnemy[index].dirY = st_enemyType[i].dirY;
			g_stEnemy[index].damage = st_enemyType[i].damage;
		}
	}
}