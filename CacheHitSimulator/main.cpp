// CacheHitSimulator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>
#include "HashTable.h"
#include "CacheHitSimulator.h"

struct Test
{
    alignas(64) int t2;
    int ii;
};


int main()
{
    CCacheSimulator cacheS;
    int a;
    char b;
    cacheS.CacheHit(&a);
    a = 4;
    cacheS.CacheHit(&b);
    b = 8;
    cacheS.CacheHit(&a);
    a = 7;

    Test t;
//    alignas(64) int t2;
    cacheS.CacheHit(&t.t2);
    t.t2 = 65535;

//    int ii;
    cacheS.CacheHit(&t.ii);
    t.ii = 0;

    alignas(64) int y;
    cacheS.CacheHit(&y);
    y = 9;


    unsigned long ptr1 = (unsigned long)&a;
    unsigned long ptr2 = (unsigned long)&b;
    unsigned long ptr3 = (unsigned long)&t;
    unsigned long ptr4 = (unsigned long)&y;
//    unsigned long ptr5 = (unsigned long)&ii;

    // 캐시라인을 찾는 방법 
    printf("0x%p \n", ptr1 & 0xffffffc0);
    printf("0x%p \n", ptr2 & 0xffffffc0);
    printf("0x%p \n", ptr3 & 0xffffffc0);
    printf("0x%p \n", ptr4 & 0xffffffc0);
//    printf("0x%p \n", ptr5 & 0xffffffc0);

    std::cout << "Hello World!\n";
}