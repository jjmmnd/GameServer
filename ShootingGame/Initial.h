#pragma once

#include "Global_Struct.h"

#define ENEMY_COUNT 60
#define BULLET_MAX 50
#define dfSCREEN_WIDTH	81		// �ܼ� ���� 80ĭ + NULL
#define dfSCREEN_HEIGHT	24		// �ܼ� ���� 25ĭ + NULL


//--------------------------------------------------------------------
// ���� ���۽� ��ü ������ �ʱ�ȭ
//--------------------------------------------------------------------
void Initial(void);
void InitPlayer();
void InitHP();
void InitBullet();
void ReadEnemyFileInfo();	// ���� Ÿ�� ��� �б�	=> ���� ����ü�� ����
void ReadEnemy();			// Ÿ�Ժ� ������ �Ӽ� �б� => ���� ����ü�� ���� 
void ReadStageFileInfo();	// �������� ��� �б�		=> ���� ����ü�� ����

//--------------------------------------------------------------------
// ���� �ε��� �������� �� ���� �ʱ�ȭ
//--------------------------------------------------------------------
void ReadStage();
void InitEnemy(int);
void SettingAttribute(int index, char type);
