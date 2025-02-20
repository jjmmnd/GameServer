#pragma once

// 아군 구조체 선언
struct st_PLAYER
{
	bool iVisible;				

	int iHP;					
	int iX;						
	int iY;

};


// 적 구조체 선언
struct st_Enemy {
	bool visible;		// 적군의 생존 여부

	int enemyX;			// 랜더 좌표
	int enemyY;

	// 파일 입력으로 받을 멤버 변수
	char type;
	int dirX;
	int dirY;
	int damage;
};


// 총알 구조체 선언
struct st_Bullet {
	bool visible;		// 총알의 생존 여부 
	bool player;		// 플레이어 총알이면 true, 적군이면 false

	int bulletX;
	int bulletY;

	int damage;			// 적군 타입에 종속된 변수 
};

// enemy 파일의 헤더 구조체
struct Enemy_Info {
	int enemyNum;
	char enemyName[256];
};

//-------------------------------------------
// enemy[type].txt의 저장 순서
// 
// 타입
// x 방향 (-1,0,1)
// y 방향 (-1,0,1)
// 데미지
//-------------------------------------------
struct Enemy_Type {
	char type;
	int dirX;
	int dirY;
	int damage;
};


// stage 파일의 헤더 구조체 
struct Stage_Info {
	int stageNum;
	char stageName[256];
};