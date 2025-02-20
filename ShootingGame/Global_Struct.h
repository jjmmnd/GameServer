#pragma once

// �Ʊ� ����ü ����
struct st_PLAYER
{
	bool iVisible;				

	int iHP;					
	int iX;						
	int iY;

};


// �� ����ü ����
struct st_Enemy {
	bool visible;		// ������ ���� ����

	int enemyX;			// ���� ��ǥ
	int enemyY;

	// ���� �Է����� ���� ��� ����
	char type;
	int dirX;
	int dirY;
	int damage;
};


// �Ѿ� ����ü ����
struct st_Bullet {
	bool visible;		// �Ѿ��� ���� ���� 
	bool player;		// �÷��̾� �Ѿ��̸� true, �����̸� false

	int bulletX;
	int bulletY;

	int damage;			// ���� Ÿ�Կ� ���ӵ� ���� 
};

// enemy ������ ��� ����ü
struct Enemy_Info {
	int enemyNum;
	char enemyName[256];
};

//-------------------------------------------
// enemy[type].txt�� ���� ����
// 
// Ÿ��
// x ���� (-1,0,1)
// y ���� (-1,0,1)
// ������
//-------------------------------------------
struct Enemy_Type {
	char type;
	int dirX;
	int dirY;
	int damage;
};


// stage ������ ��� ����ü 
struct Stage_Info {
	int stageNum;
	char stageName[256];
};