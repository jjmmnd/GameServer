#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Initial.h"

// ���� ����ü �� ���� ������
st_PLAYER g_stPlayer;
st_Enemy g_stEnemy[ENEMY_COUNT];
st_Bullet g_stBullet[BULLET_MAX];

Stage_Info* st_stageInfo;	// �������� ���� ���
Enemy_Info* st_enemyInfo;	// ���� ���� ���
Enemy_Type* st_enemyType;	// Ÿ�Ժ� ���� �Ӽ�
char* g_stageBuffer;

int g_stage;
int g_scene;
int g_typeCnt;
int g_stageCnt;

//--------------------------------------------------------------------
// ���� ���۽� ��ü ������ �ʱ�ȭ��.
//--------------------------------------------------------------------
void Initial(void)
{
	srand((unsigned)time(NULL));

	// �� �ʱ�ȭ
	// g_scene = TITLE;

	// �÷��̾� �ʱ�ȭ 
	InitPlayer();

	// �Ѿ� �ʱ�ȭ
	InitBullet();

	//------------------------------------
	// ���� ���� ��� �б�
	// ��� ����Ÿ�� ������ ���(ReadEnemy) st_enemyType�� ����
	//------------------------------------ 
	ReadEnemyFileInfo();

	//-----------------------------------
	// �������� ���� ��� �б�
	// st_stageInfo�� �������� ���� �̸� ����
	// �� ���������� ���۵Ǳ� �� �ش� �������� ������ ����
	// st_enemyType�� �̿��� ������ �ʱ�ȭ(InitEnemy) �Բ� ���� 
	//-----------------------------------
	ReadStageFileInfo();
}


// �÷��̾� �ʱ�ȭ
void InitPlayer()
{
	//-------------------------------------------------------------------
	// �÷��̾� ���� ������ �ʱ�ȭ.
	// HP : 5 
	// 39,19 ��ġ���� ���� ��.
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


// �Ѿ� �ʱ�ȭ
void InitBullet()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_stBullet[i].visible = false;
		g_stBullet[i].damage = 0;
	}
}



// ���� Ÿ�� ��� �б�
// �ʱ�ȭ �ܰ迡�� ȣ��
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

	// ��� ������ ũ��+1�� ���� ���� ���� 
	char* enemyBuffer = (char*)malloc(fileSize + 1);	// �ι��� �߰��� ���� +1
	if (enemyBuffer == NULL) {
		printf("Memory allocation failed\n");
		fclose(fPtr);
		return;
	}

	fread(enemyBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	enemyBuffer[fileSize] = '\n';

	/*******���� Ÿ�� ����� �����Ҵ����� �б� �Ϸ�********/


	char* bufferPtr = enemyBuffer;
	char* context = NULL;
	char* token = strtok_s(bufferPtr, "\n", &context);
	int enemyFileCnt = atoi(token);		// ù��° �ٿ��� ������ ������ ����
	bufferPtr = NULL;

	// ���� ������ Ÿ���� ���� ���� 
	g_typeCnt = enemyFileCnt;

	// ������ ������ŭ �����Ҵ�
	st_enemyInfo = (Enemy_Info*)malloc(sizeof(Enemy_Info) * enemyFileCnt);
	if (st_enemyInfo == NULL)
	{
		printf("Memory Allocation fail\n");
		free(enemyBuffer);
		return;
	}

	// ������ ������ŭ ���� �̸��� ����� ���� 
	for (int i = 0; i < enemyFileCnt; i++)
	{
		token = strtok_s(NULL, "\n", &context);
		// ����ó��
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
* ������ ���� ���� �а� ���� ����ü�� �����ϱ�
* �ʱ�ȭ �ܰ迡�� ReadEnemyFileInfo�� ���� ȣ��Ǵ� �Լ�
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

		char* enemyBuffer = (char*)malloc(fileSize + 1);		// ���� ������ ������ ������ �ӽ� ���� 
		if (enemyBuffer == NULL)
		{
			printf("EnemyBuffer Memory Allocation fail\n");
			fclose(fPtr);
			return;
		}

		fread(enemyBuffer, 1, fileSize, fPtr);
		fclose(fPtr);
		enemyBuffer[fileSize] = '\n';


		// ���� ����ü�� ����
		char* token = enemyBuffer;
		char* context = NULL;
		st_enemyType[i].type = *token;
		token = strtok_s(token, "\n", &context); // ���� �ٷ� �̵�

		token = strtok_s(NULL, "\n", &context);
		if (token != NULL)
			st_enemyType[i].dirX = atoi(token);
		token = strtok_s(NULL, "\n", &context);
		if (token != NULL)
			st_enemyType[i].dirY = atoi(token);
		token = strtok_s(NULL, "\n", &context);
		if (token != NULL)
			st_enemyType[i].damage = atoi(token);


		// ���� �� �� free
		free(enemyBuffer);
	}
}


// �ʱ�ȭ �ܰ迡�� ȣ��
// stage ���� �̸��� stage ������� �ԷµǾ� �־�� �� 
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

	char* infoBuffer = (char*)malloc(fileSize + 1);	// �ι��� �߰��� ���� +1
	if (infoBuffer == NULL) {
		printf("Memory allocation failed\n");
		fclose(fPtr);
		return;
	}

	fread(infoBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	infoBuffer[fileSize] = '\n';
	/*******stage header������ �����Ҵ����� �б� �Ϸ�********/


	char* bufferPtr = infoBuffer;
	char* context = NULL;

	char* token = strtok_s(bufferPtr, "\n", &context);
	int stageFileCnt = atoi(token);		// ù��° �ٿ��� stage ������ ������ ����

	bufferPtr = NULL;

	g_stageCnt = stageFileCnt;

	// ������ ������ŭ �����Ҵ�
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
		// ����ó��
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
// LOAD �ܰ迡�� ȣ��Ǵ� �Լ�
//------------------------------
void ReadStage()
{
	const char* stageName = st_stageInfo[g_stage - 1].stageName;

	FILE* fPtr;
	fopen_s(&fPtr, stageName, "r");		// �ؽ�Ʈ���� �� ��ġ ���� �о���� 
	if (fPtr == NULL)
	{
		printf("stage%d.txt file open fail", st_stageInfo[g_stage - 1].stageNum);
		return;
	}

	fseek(fPtr, 0, SEEK_END);
	int fileSize = ftell(fPtr);
	fseek(fPtr, 0, SEEK_SET);

	// ���� �迭 ���ۿ� �ش� �������� ������ ��ġ�� �̸� �����صд� 
	g_stageBuffer = (char*)malloc(fileSize + 1);	// �ι��� �߰��� ���� +1
	if (g_stageBuffer == NULL) {
		printf("Memory allocation failed\n");
		fclose(fPtr);
		return;
	}

	fread(g_stageBuffer, 1, fileSize, fPtr);
	fclose(fPtr);
	g_stageBuffer[fileSize] = '\n';

	/*************�������� stage�� �Ҵ� �Ϸ� *******************/

	InitEnemy(fileSize + 1);
}


// ���� �ʱ�ȭ
// ������ �迭�� ����
void InitEnemy(int fileSize)
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		g_stEnemy[i].visible = false;
	}

	int posX = 0;
	int posY = 0;
	int k = 0;

	// ���� �迭 ���۸� ���鼭 ������ ��ǥ ����
	char* bPtr = g_stageBuffer;

	for (int i = 0; i < dfSCREEN_HEIGHT - 1; i++)
	{
		while (*bPtr != '\n')
		{
			if (*bPtr != ' ' && *bPtr != '\\')
			{
				g_stEnemy[k].visible = true;
				g_stEnemy[k].type = *bPtr;	// ���� ��ġ�� Ÿ���� ����Ǿ� �ֱ� ������ type�� �߰� 
				g_stEnemy[k].enemyX = posX;	// ��ǥ ����
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