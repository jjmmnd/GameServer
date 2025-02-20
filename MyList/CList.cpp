
#include <iostream>
#include <cstdlib>
#include "CList.h"

using namespace std;

int main()
{
	CList<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	for (auto a = list.begin(); a != list.end(); ++a)
	{
		cout << *a;
	}

//	cout << *list.begin();

	return 0;
}