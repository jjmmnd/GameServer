#include <iostream>
#include "MemoryPool.h"
#include "Profile.h"
#include "SerializeBuffer.h"


class Player
{
public:
	int64_t _hp;
	unsigned int _speed;
	Player()
	{
		_hp = 0;
		_speed = 8;
	}
};

int main()
{
	
	Initial();


	/*-------------------------------
	* 일반 오브젝트 활용
	-------------------------------*/
	CMemoryPool<Player> PlayerFreeList(0, true);
	for (int i = 0; i < 5000; i++)
	{
		Profile p1("Object - MemoryPool");

		Player* p = PlayerFreeList.Alloc();
		p->_hp = 952220;
		p->_speed = 99;
		//	printf("%lld", p->_hp);
		//	printf("%d", p->_speed);
		//////	printf("%x", p);
		//if (PlayerFreeList.Free(p))
		//	printf("p ok\n");
		PlayerFreeList.Free(p);
		Player* p22 = PlayerFreeList.Alloc();
		p22->_hp = 950;
		//	printf("%lld", p22->_hp);
		//	printf("%d", p22->_speed);
		//if (PlayerFreeList.Free(p22))
		//			printf("p22 ok");
		PlayerFreeList.Free(p22);
	}
	printf("\n\n");
	for (int i = 0; i < 5000; i++)
	{
		Profile p2("Object - new/delete");
		Player* p0 = new Player;
		p0->_hp = 99999;
		p0->_speed = 7979;
		delete p0;
		Player* p33 = new Player;
		p33->_hp = 753;
		delete p33;

	}

	/*---------------------------------------
	* 직렬화 버퍼 활용
	----------------------------------------*/
	CMemoryPool<CMessage> CMessageFreelist(0, false);
	CMessage* CPacekt = CMessageFreelist.Alloc();
	*CPacekt << 5;
	*CPacekt << 5.9f;

	int a;
	float f;

	*CPacekt >> a;
	*CPacekt >> f;

	CMessageFreelist.Free(CPacekt);

	printf("%d %f", a, f);

	DataOutText(SetFileName());
	return 0;
}