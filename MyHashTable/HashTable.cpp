// HashTable.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <stdlib.h>

#define Table_Size 20

typedef struct node {
	int key;
	int value;
	struct node* before;
	struct node* next;
}Node;

typedef struct bucket{
	Node* head;
	int count;
}Bucket;

Bucket hashTable[Table_Size];

void InitTable();
void PrintMenu();
void AddData();
Node* CreateNode(int key, int value);
int Hashing(int key);
bool DeleteData();
void Show();
void Search();

int main()
{
	InitTable();

	while (1)
	{
		PrintMenu();
		int num;
		scanf_s("%d", &num);
		switch (num)
		{
		case 1:
			AddData();
			continue;
		case 2:
			DeleteData();
			continue;
		case 3:
			Show();
			continue;
		case 4:
			Search();
			continue;
		}
	}
}

void InitTable()
{
	for (int i = 0; i < Table_Size; i++)
	{
		hashTable[i].count = 0;
		hashTable[i].head = NULL;
	}
}

void PrintMenu()
{
	printf("1.데이터 추가\n2.데이터 삭제\n3.전체보기\n4. 찾기\n");
}

void AddData()
{
	int key, value;
	printf("key: ");
	scanf_s("%d", &key);
	printf("value: ");
	scanf_s("%d", &value);

	Node* newNode = CreateNode(key, value);

	int hashIndex = Hashing(key);

	if (hashTable[hashIndex].head != NULL)
	{
		hashTable[hashIndex].head->before = newNode;
		newNode->next = hashTable[hashIndex].head;
	}
	hashTable[hashIndex].head = newNode;
	hashTable[hashIndex].count++;
}

Node* CreateNode(int key, int value)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->value = value;
	newNode->before = NULL;
	newNode->next = NULL;
	return newNode;
}

int Hashing(int key)
{
	return key % Table_Size;
}

bool DeleteData()
{
	int key;
	printf("key: ");
	scanf_s("%d", &key);

	int hashIndex = Hashing(key);

	Node* iterator = hashTable[hashIndex].head;
	while (iterator != NULL)
	{
		if (iterator->key == key)
		{
			if (iterator == hashTable[hashIndex].head)
			{
				hashTable[hashIndex].head = iterator->next;
			}
			else
			{
				iterator->before->next = iterator->next;
				iterator->next->before = iterator->before;
			}
			break;
		}
		iterator = iterator->next;
	}

	if (iterator == NULL)
	{
		printf("Could not add data\n");
		return false;
	}
	hashTable[hashIndex].count--;
	free(iterator);
	return true;
}

void Show()
{
	Node* iterator;
	for (int i = 0; i < Table_Size; i++)
	{
		printf("[hashIndex: %d]", i);
		iterator = hashTable[i].head;
		while (iterator != NULL)
		{
			printf("> key: %d value: %d ", iterator->key, iterator->value);
			iterator = iterator->next;
		}
		printf("\n");
	}
}

void Search()
{
	int key;
	printf("key: ");
	scanf_s("%d", &key);

	Node* iterator;
	int hashIndex = Hashing(key);
	iterator = hashTable[hashIndex].head;

	while (iterator != NULL)
	{
		if (iterator->key == key)
		{
			printf("Found. key: %d value: %d\n", iterator->key, iterator->value);
			return;
		}
		iterator = iterator->next;
	}
}