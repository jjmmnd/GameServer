#include "framework.h"
#include "main.h"
#include <windowsx.h>
#include "Profile.h"

#define GRID_SIZE 16
#define GRID_WIDTH 100
#define GRID_HEIGHT 50

#include "ASTAR.h"

// 전역 변수:
HBRUSH g_hTileBrush;
HPEN g_hGridPen;
HBRUSH g_startBrush;
HBRUSH g_endBrush;
char g_Tile[GRID_HEIGHT][GRID_WIDTH];   // 0 장애물 없음 / 1 장애물 있음
HBRUSH g_openBrush;
HBRUSH g_closeBrush;


// 더블 버퍼링 전역 변수: 
HBITMAP g_hMemDCBitmap;
HDC g_hMemDC;
RECT g_MemDCRect;

Pos g_startPos;
Pos g_endPos;
bool g_bStart = false;
bool g_bEnd = false;

ASNode* g_resultNode;
bool g_bFind = false;


bool g_bErase = false;
bool g_bDrag = false;

void RenderGrid(HDC hdc)
{
    int iX = 0;
    int iY = 0;
    SelectObject(hdc, g_hGridPen);
    // 그리드의 마지막 선을 추가로 그리기 위해 <=의 반복 조건
    for (int iCntW = 0; iCntW <= GRID_WIDTH; iCntW++)
    {
        MoveToEx(hdc, iX, 0, NULL);
        LineTo(hdc, iX, GRID_HEIGHT * GRID_SIZE);
        iX += GRID_SIZE;
    }
    for (int iCntH = 0; iCntH <= GRID_HEIGHT; iCntH++)
    {
        MoveToEx(hdc, 0, iY, NULL);
        LineTo(hdc, GRID_WIDTH * GRID_SIZE, iY);
        iY += GRID_SIZE;
    }
}

void RenderObstacle(HDC hdc)
{
    int iX = 0;
    int iY = 0;
    SelectObject(hdc, g_hTileBrush);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
    {
        for (int iCntH = 0; iCntH < GRID_HEIGHT; iCntH++)
        {
            if (g_Tile[iCntH][iCntW])
            {
                iX = iCntW * GRID_SIZE;
                iY = iCntH * GRID_SIZE;
                // 테두리 크기가 있으므로 +2
                Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
            }
        }
    }
}


void RenderPath(HDC hdc, ASNode* endNode)
{
    HPEN pathPen = CreatePen(PS_SOLID, 3, RGB(255, 127, 0));

    SelectObject(hdc, pathPen);

    int iX = endNode->_iX * GRID_SIZE;
    int iY = endNode->_iY * GRID_SIZE;
    ASNode* parentNode = endNode->_parentNode;
    while (parentNode != nullptr)
    {
        MoveToEx(hdc, iX + GRID_SIZE/2, iY+GRID_SIZE/2, NULL);
        LineTo(hdc, parentNode->_iX * GRID_SIZE + GRID_SIZE / 2, parentNode->_iY * GRID_SIZE + GRID_SIZE / 2);
        iX = parentNode->_iX* GRID_SIZE;
        iY = parentNode->_iY* GRID_SIZE;
        parentNode = parentNode->_parentNode;
    }
}

void Renderlist(HDC hdc)
{
    int iX;
    int iY;

    SelectObject(hdc, g_openBrush);
    for (vector<ASNode*>::iterator it = ASTAR::v_openlist.begin(); it != ASTAR::v_openlist.end(); ++it)
    {
        iX = (*it)->_iX * GRID_SIZE;
        iY = (*it)->_iY * GRID_SIZE;
        Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
    }

    SelectObject(hdc, g_closeBrush);
    for (vector<ASNode*>::iterator it = ASTAR::v_closelist.begin(); it != ASTAR::v_closelist.end(); ++it)
    {
        iX = (*it)->_iX * GRID_SIZE;
        iY = (*it)->_iY * GRID_SIZE;
        Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
    }
}

void RenderPos(HDC hdc, Pos startPos, Pos endPos)
{
    int iX;
    int iY;

    if (g_bStart)
    {
        SelectObject(hdc, g_startBrush);
        SelectObject(hdc, GetStockObject(NULL_PEN));
        iX = startPos.x * GRID_SIZE;
        iY = startPos.y * GRID_SIZE;
        Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
    }
    if (g_bEnd)
    {
        SelectObject(hdc, g_endBrush);
        SelectObject(hdc, GetStockObject(NULL_PEN));
        iX = endPos.x * GRID_SIZE;
        iY = endPos.y * GRID_SIZE;
        Rectangle(hdc, iX , iY , iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);
    }
}

void ClearObstacle(HWND hWnd)
{
    // 장애물 삭제 
    for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
    {
        for (int iCntH = 0; iCntH < GRID_HEIGHT; iCntH++)
        {
            if (g_Tile[iCntH][iCntW])
                g_Tile[iCntH][iCntW] = 0;
        }
    }

    InvalidateRect(hWnd, NULL, false);
}

void RenderParent(HDC hdc, ASNode* curNode)
{
    ASNode* parentNode = curNode->_parentNode;
    

}


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"ABCD";
    wcex.hIconSm = NULL;

    RegisterClassExW(&wcex);

    HWND hWnd;
    hWnd = CreateWindowW(L"ABCD", L"Caption", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    // 애플리케이션 초기화를 수행합니다:
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;


    switch (message)
    {
    case WM_KEYDOWN:
    {
        // F1 : 한번에 길찾기
        // F2 : 누를 때마다 길찾기
        // F3 : 경로 초기화
        // F4 : 프로파일링 출력
        // F5 : 맵 랜덤 생성 (Todo) 
        switch (wParam)
        {
        case VK_F2:
        {
            g_bFind = false;
            ASTAR::ClearList();
            if (ASTAR::FindPath(g_startPos, g_endPos, hWnd))
                g_bFind = true;
        }
        break;
        case VK_F3:
        {
            ASTAR::ClearList();
            ClearObstacle(hWnd);
            g_bFind = false;
        }
            break;
        case VK_F4:
            DataOutText(SetFileName());
            break;
        case VK_F5:

            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_LBUTTONDOWN:
        g_bDrag = true;
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            int iTileX = xPos / GRID_SIZE;
            int iTileY = yPos / GRID_SIZE;
            // 첫 선택 타일이 장애물이면 지우기 모드
            if (g_Tile[iTileY][iTileX] == 1)
                g_bErase = true;
            else
                g_bErase = false;
        }
        break;
    case WM_LBUTTONUP:
        g_bDrag = false;
        break;
    case WM_MBUTTONDOWN:
    {
        // 휠 누르면 g_startPos 설정
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        int iTileX = xPos / GRID_SIZE;
        int iTileY = yPos / GRID_SIZE;
        g_startPos = { iTileX,iTileY };
    }
        g_bStart = true;
        g_bDrag = false;
        InvalidateRect(hWnd, NULL, false);
    break;
    case WM_RBUTTONDOWN:
    {
        // 우클릭 누르면 g_endPos 세팅
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        int iTileX = xPos / GRID_SIZE;
        int iTileY = yPos / GRID_SIZE;
        g_endPos = { iTileX,iTileY };
    }
        g_bEnd = true;
        g_bDrag = false;    // 한 점만 그리도록
        InvalidateRect(hWnd, NULL, false);
    break;
    case WM_MOUSEMOVE:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        if (g_bDrag)
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            int iTileX = xPos / GRID_SIZE;
            int iTIleY = yPos / GRID_SIZE;

            g_Tile[iTIleY][iTileX] = !g_bErase;
            InvalidateRect(hWnd, NULL, false);
        }
    }
    break;
    case WM_CREATE:
    {
        Initial();

        g_hGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        g_hTileBrush = CreateSolidBrush(RGB(100, 100, 100));

        g_startBrush = CreateSolidBrush(RGB(200, 0, 0));
        g_endBrush = CreateSolidBrush(RGB(0, 200, 0));

        g_openBrush = CreateSolidBrush(RGB(0, 0, 255));
        g_closeBrush = CreateSolidBrush(RGB(255, 255, 0));



        // 윈도우 생성 시 현 윈도우와 동일한 메모리 DC 생성
        hdc = GetDC(hWnd);
        GetClientRect(hWnd, &g_MemDCRect);
        g_hMemDCBitmap = CreateCompatibleBitmap(hdc, g_MemDCRect.right, g_MemDCRect.bottom);
        g_hMemDC = CreateCompatibleDC(hdc);
        ReleaseDC(hWnd, hdc);
    }
    break;
    case WM_PAINT:
    {
        // 메모리 DC를 클리어 하고
        PatBlt(g_hMemDC, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, WHITENESS);
        RenderObstacle(g_hMemDC);
        RenderGrid(g_hMemDC);

        //=======추가=============
        if (g_bFind)
        {
            Renderlist(g_hMemDC);
            RenderPath(g_hMemDC, g_resultNode);
        }

        if (g_bStart || g_bEnd)
            RenderPos(g_hMemDC, g_startPos, g_endPos);


        // 사용법 안내
        const wchar_t* howToStr = L"F2 : 누를 때마다 길찾기\
            F3 : 경로 초기화\
            F4 : 프로파일 결과 출력\
            F5 : 맵 랜덤 생성";
        SetBkMode(g_hMemDC, TRANSPARENT);
        TextOut(g_hMemDC, GRID_SIZE, GRID_SIZE, howToStr, wcslen(howToStr));

        
        hdc = BeginPaint(hWnd, &ps);
        BitBlt(hdc, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, g_hMemDC, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        DeleteObject(g_hMemDC);
        DeleteObject(g_hMemDCBitmap);
        DeleteObject(g_hTileBrush);
        DeleteObject(g_hGridPen);
        PostQuitMessage(0);
    break;
    case WM_SIZE:
    {
        DeleteObject(g_hMemDC);
        DeleteObject(g_hMemDCBitmap);

        hdc = GetDC(hWnd);
        GetClientRect(hWnd, &g_MemDCRect);
        g_hMemDCBitmap = CreateCompatibleBitmap(hdc, g_MemDCRect.right, g_MemDCRect.bottom);
        g_hMemDC = CreateCompatibleDC(hdc);
        ReleaseDC(hWnd, hdc);
        SelectObject(g_hMemDC, g_hMemDCBitmap);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

