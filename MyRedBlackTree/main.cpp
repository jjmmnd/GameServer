#include <iostream>
#include "RedBlackTree.h"


int main()
{
	std::cout << "0-BLACK 1-RED\n";
	CRedBlackTree RBT;
	RBT.Insert(5,7);
	RBT.Insert(82,8);
	RBT.Insert(24,9);
	RBT.Insert(40,10);
	RBT.Insert(28,33);
	RBT.Insert(3,67);
	RBT.Insert(57,8);
	RBT.Insert(9,111);
	RBT.Insert(77,732);
	RBT.Insert(154,4); // 여기서 head가 40으로 변화 
	RBT.Insert(99,5);
	RBT.Insert(49,15);
	RBT.Insert(11,77);
	RBT.Insert(33,774);
	
	RBT.PrintTree();
	std::cout << '\n';

	
	RBT.Remove(24);	// head 삭제
	RBT.PrintTree();
	std::cout << '\n';
	RBT.Remove(82);	// 제일 큰 값 삭제
	RBT.PrintTree();
	std::cout << '\n';
	RBT.Remove(33); // 중간값 삭제

	RBT.PrintTree();
	std::cout << '\n';
	RBT.Remove(40);
	RBT.Insert(29,88);

	RBT.PrintTree();
	std::cout << '\n';


	int value;
	RBT.Search(9,&value);
	std::cout << value;

}