#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32")
#include "framework.h"
#include "AsyncSelect.h"
#include <windowsx.h>
#include "RingBuffer.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#define WM_NETWORK (WM_USER + 1)



// 헤더
struct stHEADER
{
    unsigned short Len;
};

// 패킷
struct st_DRAW_PACKET
{
    int		iStartX;
    int		iStartY;
    int		iEndX;
    int		iEndY;
};


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
SOCKET g_socket;
CRingBuffer* g_sendQ;
CRingBuffer* g_recvQ;
HPEN g_pen;
bool g_bDrag = false;
int g_xPos;
int g_yPos;
bool g_serverConnect = false;
bool g_bsend = false;

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASYNCSELECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ASYNCSELECT);
    wcex.lpszClassName = L"Net Drawing";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    HWND hWnd;
    hWnd = CreateWindowW(L"Net Drawing", L"Net Drawing", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    g_sendQ = new CRingBuffer();
    g_recvQ = new CRingBuffer();
    g_pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

    // 네트워크 작업
    WSAData wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;
    g_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_socket == SOCKET_ERROR)
    {
        return 1;
    }
    // select
    WSAAsyncSelect(g_socket, hWnd, WM_NETWORK, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE);
    // connect
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // 175.116.13.182
    InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr.S_un.S_addr);
    serverAddr.sin_port = htons(25000);

    int retval = connect(g_socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    int Error;
    if (retval == SOCKET_ERROR)
    {
        Error = WSAGetLastError();
        if (Error != WSAEWOULDBLOCK)
        {
            TCHAR* text = (TCHAR*)Error;
            HDC hdc = GetDC(hWnd);
            int ret = MessageBox(hWnd, text, TEXT("conncet() err"), MB_OK);
            ReleaseDC(hWnd, hdc);
            return 1;
        }
    }
    g_serverConnect = true;

    MSG msg;
    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}


bool ReadEvent(HWND hWnd)
{
    int retval;
    int Error;
    retval = recv(g_socket, g_recvQ->GetRearPtr(), g_recvQ->DirectEnqueueSize(), 0);
    if (retval == 0)
    {
        // 정상 종료
        PostMessage(hWnd, WM_DESTROY, 0, 0);
        return true;
    }
    if (retval == SOCKET_ERROR)
    {
        Error = WSAGetLastError();
        if (Error != WSAEWOULDBLOCK)
        {
            return false;
        }
        return true;
    }
    g_recvQ->MoveRear(retval);

    HDC hdc = GetDC(hWnd);
    
    while (g_recvQ->GetUseSize() >= sizeof(st_DRAW_PACKET) + sizeof(stHEADER))
    {
        stHEADER header;
        st_DRAW_PACKET payload;
        retval = g_recvQ->Peek((char*)&header, sizeof(header));
        if (retval == 0)
            break;
        g_recvQ->MoveFront(sizeof(header));
        retval = g_recvQ->Dequeue((char*)&payload, sizeof(payload));
        if (retval == 0)
            break;
        MoveToEx(hdc, payload.iStartX, payload.iStartY, NULL);
        LineTo(hdc, payload.iEndX, payload.iEndY);
    }
    ReleaseDC(hWnd, hdc);
    return true;
}

void WriteEvent(HWND hWnd)
{
    // 송신 버퍼에 공간이 있을 때
    // send 
    if (!g_bsend)
        return;
    int Error;
    int retval;
    

    while (g_sendQ->GetUseSize() > 0)
    {
        retval = send(g_socket, g_sendQ->GetFrontPtr(), g_sendQ->DirectDequeueSize(), 0);
        if (retval == SOCKET_ERROR)
        {
            Error = WSAGetLastError();
            if (Error != WSAEWOULDBLOCK)
            {
                HDC hdc = GetDC(hWnd);
                int ret = MessageBox(hWnd, TEXT("WriteEvent fail"), TEXT("WriteEvent"), MB_OK);
                ReleaseDC(hWnd, hdc);
            }
            g_bsend = false;
            break;
        }
        g_sendQ->MoveFront(retval);
    }
    return;
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        if (!g_serverConnect)
        {
            hdc = GetDC(hWnd);
            MessageBox(hWnd, TEXT("서버와의 연결이 끊어졌습니다"), TEXT("WM_LBUTTONDOWN err"), MB_OK);
            ReleaseDC(hWnd, hdc);
            break;
        }
        g_bDrag = true;
        g_xPos = GET_X_LPARAM(lParam);
        g_yPos = GET_Y_LPARAM(lParam);
    }
    break;
    case WM_LBUTTONUP:
    {
        g_bDrag = false;
    }
    break;
    case WM_MOUSEMOVE:
    {
        int iX = GET_X_LPARAM(lParam);
        int iY = GET_Y_LPARAM(lParam);
        if (g_bDrag)
        {
            // 메시지 세팅 후 곧바로 send 
            stHEADER sendHeader;
            sendHeader.Len = 16;
            st_DRAW_PACKET sendPayload;
            sendPayload.iStartX = g_xPos;
            sendPayload.iStartY = g_yPos;
            sendPayload.iEndX = iX;
            sendPayload.iEndY = iY;
            if (g_sendQ->GetUseSize() >= g_sendQ->GetBufferSize())
            {
                // 더이상 못보내는 상황이라면 WM_DESTROY
                PostMessage(hWnd, WM_DESTROY, 0, 0);
            }
            g_sendQ->Enqueue((const char*)&sendHeader, sizeof(sendHeader));
            g_sendQ->Enqueue((const char*)&sendPayload, sizeof(sendPayload));
            WriteEvent(hWnd);
            //g_bDrag = false;
        }
        g_xPos = iX;
        g_yPos = iY;
    }
    break;
    case WM_NETWORK:
    {
        if (WSAGETSELECTERROR(lParam))
        {
            hdc = GetDC(hWnd);
            //658833440
            if (lParam == 658833440)
            {
                MessageBox(hWnd, TEXT("소켓 에러 종료합니다"), TEXT("Network error"), MB_OK);
                ReleaseDC(hWnd, hdc);
                // 이 뒤로는 "서버와의 연결이 끊어졌습니다"
                g_serverConnect = false;
                break;
            }
            //long lParamCopy = lParam;
            //TCHAR* text = (TCHAR*)lParamCopy;
            MessageBox(hWnd, TEXT("WSAGETSELECTERROR(lParam)"), TEXT("WM_NETWORT err"), MB_OK);
            ReleaseDC(hWnd, hdc);
            break;
        }
        switch (WSAGETSELECTEVENT(lParam))
        {
        case FD_CONNECT:
        {
            // 중간에 send 처리가 안 가도록 처리 필요 
            hdc = GetDC(hWnd);
            int ret = MessageBox(hWnd, TEXT("connect success"), TEXT("conncet success"), MB_OK);
            ReleaseDC(hWnd, hdc);
        }
        break;
        case FD_CLOSE:
            break;
        case FD_READ:
        {
            if (!ReadEvent(hWnd))
            {
                // 메시지 박스
                hdc = GetDC(hWnd);
                int ret = MessageBox(hWnd, TEXT("ReadEvent fail"), TEXT("ReadEvent() err"), MB_OK);
                ReleaseDC(hWnd, hdc);
            }
        }
        break;
        case FD_WRITE:
        {
            g_bsend = true;
            WriteEvent(hWnd);
        }
        break;
        default:
        {
            // 메시지 박스 
            hdc = GetDC(hWnd);
            int ret = MessageBox(hWnd, TEXT("lParam check"), TEXT("default err"), MB_OK);
            ReleaseDC(hWnd, hdc);
        }
        }
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        delete g_sendQ;
        delete g_recvQ;
        closesocket(g_socket);
        DeleteObject(g_pen);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}