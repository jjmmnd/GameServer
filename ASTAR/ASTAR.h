#pragma once

#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
using namespace std;

//#define RENDER

struct Pos
{
	int x;
	int y;
};

struct ASNode
{
	ASNode* _parentNode;
	int _iX;
	int _iY;
	double _G;	// 이동거리
	double _H;	// 목적지거리
	double _F;	// G + H
};

extern char g_Tile[GRID_HEIGHT][GRID_WIDTH];
extern Pos g_startPos;
extern Pos g_endPos;
extern HDC g_hMemDC;
extern HBRUSH g_hTileBrush;

void RenderPos(HDC hdc, Pos startPos, Pos endPos);
void RenderGrid(HDC hdc);

bool operator != (const Pos a, const Pos b)
{
	if (a.x != b.x || a.y != b.y)
		return true;
	return false;
}

bool operator == (const Pos a, const Pos b)
{
	return !(a != b);
}

extern ASNode* g_resultNode;

bool cmp(const ASNode* a, const ASNode* b)
{
	return a->_F > b->_F;
	// 내림차순 정렬 
}


namespace ASTAR
{
	vector<ASNode*> v_openlist;
	vector<ASNode*> v_closelist;
	ASNode* curNode;

	void ClearList()
	{
		for (vector<ASNode*>::iterator it = v_openlist.begin(); it != v_openlist.end(); ++it)
		{
			delete* it;
		}
		v_openlist.clear();
		vector<ASNode*>().swap(v_openlist);

		for (vector<ASNode*>::iterator it = v_closelist.begin(); it != v_closelist.end(); ++it)
		{
			delete* it;
		}
		v_closelist.clear();
		vector<ASNode*>().swap(v_closelist);
	}

#define Euclide(fromiX,fromiY,toiX,toiY) sqrt(((fromiX) - (toiX))* ((fromiX) - (toiX)) + ((fromiY) - (toiY)) * ((fromiY) - (toiY)))
	/*
	double Euclide(int fromiX, int fromiY, int toiX, int toiY)
	{
		return sqrt((
			fromiX - toiX) * (fromiX - toiX) + (fromiY - toiY) * (fromiY - toiY));
	}*/
#define Manhattan(fromiX,fromiY,toiX,toiY) abs((fromiX) - (toiX)) + abs((fromiY) - (toiY))
	/*
	int Manhattan(int fromiX, int fromiY, int toiX, int toiY)
	{
		return abs(fromiX - toiX) + abs(fromiY - toiY);
	}*/

	bool CheckPos(int x, int y)
	{
		if (g_Tile[y][x] == 1)
		{

			if (!(x == g_endPos.x && y == g_endPos.y))
				return false;
		}

		if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
			return false;

		for (vector<ASNode*>::iterator it = v_openlist.begin(); it != v_openlist.end(); ++it)
		{
			if (x == (*it)->_iX && y == (*it)->_iY)
			{
				if (curNode->_G + Euclide(curNode->_iX, curNode->_iY, x, y) < (*it)->_G)
				{
					// 부모 포인터를 나로 바꿔서 다시 삽입 + 정렬
					(*it)->_G = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x, y);
					(*it)->_F = (*it)->_G + (*it)->_H * 10;
					(*it)->_parentNode = curNode;
				}
				return false;
			}
		}

		for (vector<ASNode*>::iterator it = v_closelist.begin(); it != v_closelist.end(); ++it)
		{
			if (x == (*it)->_iX && y == (*it)->_iY)
			{
				if (curNode->_G + Euclide(curNode->_iX, curNode->_iY, x, y) < (*it)->_G)
				{
					// 부모 포인터를 나로 바꿔서 다시 삽입 + 정렬
					(*it)->_G = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x, y);
					(*it)->_F = (*it)->_G + (*it)->_H * 10;
					(*it)->_parentNode = curNode;
				}
				return false;
			}
		}
		return true;
	}

	bool FindPath(Pos start, Pos end,HWND hWnd)
	{
#ifdef RENDER
		HBRUSH closeBrush = CreateSolidBrush(RGB(255, 255, 0));
		HBRUSH openBrush = CreateSolidBrush(RGB(0, 0, 255));

		HDC hdc = GetDC(hWnd);
#endif RENDER

		Profile p3("ASTAR-FindPath");

		// 스타트 노드 생성
		ASNode* startNode = new ASNode;
		startNode->_iX = g_startPos.x;
		startNode->_iY = g_startPos.y;
		startNode->_parentNode = nullptr;
		startNode->_G = 0;
		startNode->_H = Manhattan(g_startPos.x, g_startPos.y, g_endPos.x, g_endPos.y);
		startNode->_F = Manhattan(g_startPos.x, g_startPos.y, g_endPos.x, g_endPos.y) * 10;
		v_openlist.push_back(startNode);


		while (!v_openlist.empty())
		{
			curNode = v_openlist.back(); // F값이 가장 작은 노드 포인터를 뽑기 
			v_openlist.pop_back();

			// 클로즈리스트에 대입
			v_closelist.push_back(curNode);

			int x = curNode->_iX;
			int y = curNode->_iY;

			// endPos와 같다면 리턴 
			if (x == g_endPos.x && y == g_endPos.y)
			{
				// 함수가 반환될 때 WM_Paint 메시지를 발생 
				InvalidateRect(hWnd, NULL, false);
				g_resultNode = curNode;
				return true;
			}

			double newG;
			double newH;

			// 8방향 탐색
			if (CheckPos(x, y - 1))
			{
				newG = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x, y - 1);
				newH = Manhattan(x, y - 1, g_endPos.x, g_endPos.y);

				ASNode* newNode = new ASNode;
				newNode->_iX = x;
				newNode->_iY = y - 1;
				newNode->_parentNode = curNode;
				newNode->_G = newG;
				newNode->_H = newH;
				newNode->_F = newG + newH * 10;
				v_openlist.push_back(newNode);
			}

			if (CheckPos(x - 1, y))
			{
				newG = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x - 1, y);
				newH = Manhattan(x - 1, y, g_endPos.x, g_endPos.y);

				ASNode* newNode = new ASNode;
				newNode->_iX = x - 1;
				newNode->_iY = y;
				newNode->_parentNode = curNode;
				newNode->_G = newG;
				newNode->_H = newH;
				newNode->_F = newG + newH * 10;
				v_openlist.push_back(newNode);
			}

			if (CheckPos(x + 1, y))
			{
				newG = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x + 1, y);
				newH = Manhattan(x + 1, y, g_endPos.x, g_endPos.y);

				ASNode* newNode = new ASNode;
				newNode->_iX = x + 1;
				newNode->_iY = y;
				newNode->_parentNode = curNode;
				newNode->_G = newG;
				newNode->_H = newH;
				newNode->_F = newG + newH * 10;
				v_openlist.push_back(newNode);
			}

			if (CheckPos(x, y + 1))
			{
				newG = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x, y + 1);
				newH = Manhattan(x, y + 1, g_endPos.x, g_endPos.y);

				ASNode* newNode = new ASNode;
				newNode->_iX = x;
				newNode->_iY = y + 1;
				newNode->_parentNode = curNode;
				newNode->_G = newG;
				newNode->_H = newH;
				newNode->_F = newG + newH * 10;
				v_openlist.push_back(newNode);
			}

			if (CheckPos(x - 1, y - 1))
			{
				newG = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x - 1, y - 1);
				newH = Manhattan(x - 1, y - 1, g_endPos.x, g_endPos.y);

				ASNode* newNode = new ASNode;
				newNode->_iX = x - 1;
				newNode->_iY = y - 1;
				newNode->_parentNode = curNode;
				newNode->_G = newG;
				newNode->_H = newH;
				newNode->_F = newG + newH * 10;
				v_openlist.push_back(newNode);
			}

			if (CheckPos(x + 1, y - 1))
			{
				newG = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x + 1, y - 1);
				newH = Manhattan(x + 1, y - 1, g_endPos.x, g_endPos.y);

				ASNode* newNode = new ASNode;
				newNode->_iX = x + 1;
				newNode->_iY = y - 1;
				newNode->_parentNode = curNode;
				newNode->_G = newG;
				newNode->_H = newH;
				newNode->_F = newG + newH * 10;
				v_openlist.push_back(newNode);
			}

			if (CheckPos(x + 1, y + 1))
			{
				newG = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x + 1, y + 1);
				newH = Manhattan(x + 1, y + 1, g_endPos.x, g_endPos.y);

				ASNode* newNode = new ASNode;
				newNode->_iX = x + 1;
				newNode->_iY = y + 1;
				newNode->_parentNode = curNode;
				newNode->_G = newG;
				newNode->_H = newH;
				newNode->_F = newG + newH * 10;
				v_openlist.push_back(newNode);
			}

			if (CheckPos(x - 1, y + 1))
			{
				newG = curNode->_G + Euclide(curNode->_iX, curNode->_iY, x - 1, y + 1);
				newH = Manhattan(x - 1, y + 1, g_endPos.x, g_endPos.y);

				ASNode* newNode = new ASNode;
				newNode->_iX = x - 1;
				newNode->_iY = y + 1;
				newNode->_parentNode = curNode;
				newNode->_G = newG;
				newNode->_H = newH;
				newNode->_F = newG + newH * 10;
				v_openlist.push_back(newNode);
			}
#ifdef RENDER
			int iX;
			int iY;
			// 경로를 찾아가는 과정을 보임
			if (g_bF2)
			{
				for (vector<ASNode*>::iterator it = ASTAR::v_openlist.begin(); it != ASTAR::v_openlist.end(); ++it)
				{
					SelectObject(hdc, openBrush);
					iX = (*it)->_iX * GRID_SIZE;
					iY = (*it)->_iY * GRID_SIZE;
					Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
				}

				for (vector<ASNode*>::iterator it = ASTAR::v_closelist.begin(); it != ASTAR::v_closelist.end(); ++it)
				{
					SelectObject(hdc, closeBrush);
					iX = (*it)->_iX * GRID_SIZE;
					iY = (*it)->_iY * GRID_SIZE;
					Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
				}

				RenderPos(hdc, g_startPos, g_endPos);
				RenderGrid(hdc);
				Sleep(10);
			}
#endif RENDER
			sort(v_openlist.begin(), v_openlist.end(), cmp);
		}

		// no path
		InvalidateRect(hWnd, NULL, false);
		return false;
	}
}