///////////////////////////////////////////////////
// 코어만큼의 스레드를 생성해서 push pop을 진행
// 
// 240910
///////////////////////////////////////////////////

// * 주소값을 전부 콘솔에 로그로 남기면? > 콘솔에 찍히는 순서가 보장된다고 볼 수 없을듯 > 동일한 노드가 두 번 pop될 때 문제가 발생하지만 push에 대한 시나리오를 완벽히 눈으로 파악할 수가 없음
// * 메모리를 보면서 찾는방법밖에 없을듯하다..;;

#include <iostream>
#include <Windows.h>
#include <process.h>
#pragma comment(lib,"Winmm.lib")

//struct LogStruct
//{
//    volatile LONG counter;
//    DWORD threadId;
//    void* pushLocalTop;
//    void* popLocalTop;
//    void* newTop;
//    int data;
//};

//#define DEBUG

//CRITICAL_SECTION cs;
volatile LONG g_lLogCounter = 0;

class MyStack
{
private:
    struct stNode
    {
        int data;
        stNode* next;
    };

    stNode* _Top;

public:
    MyStack()
    {
        _Top = NULL;
    }

    void Push(int data, DWORD threadId = 0)
    {
        stNode* newNode = new stNode;
        newNode->data = data;

        stNode* localTop;
        do 
        {
            localTop = _Top;
            newNode->next = localTop;

        } while (!InterlockedCompareExchange64((LONGLONG*)&_Top, (LONGLONG)newNode, (LONGLONG)localTop)); // CAS

        //InterlockedIncrement(&g_lLogCounter);
        printf("[Push][%d] real_Top(%p) newNode(%p) localTop(%p) data(%d) [%ld]\n", threadId, _Top, newNode, localTop, data, InterlockedIncrement(&g_lLogCounter));

    }

    void Pop(DWORD threadId)
    {
        stNode* localTop;
        stNode* newTop;
        int data;
        do
        {
            localTop = _Top;
            newTop = localTop->next;
            data = localTop->data;

        } while (!InterlockedCompareExchange64((LONGLONG*)&_Top, (LONGLONG)newTop, (LONGLONG)localTop));

        
        printf("[Pop][%d] real_Top(%p) newTop(%p) localTop(%p) data(%d) [%ld]\n", threadId, _Top, newTop, localTop, data, InterlockedIncrement(&g_lLogCounter));

        delete localTop;
    }
};

MyStack g_Stack;

LONG g_data=0;

unsigned WINAPI LockFreeProc(void* arg)
{
    DWORD dwThreadId = GetCurrentThreadId();

   
    while (1)
    {
#ifndef DEBUG
        // push 많이
        for (int i = 0; i < 1000; i++)
        {
            g_Stack.Push(InterlockedIncrement(&g_data), dwThreadId);
            //InterlockedIncrement(&g_data);
        }
#endif

        // pop 많이 
        for (int i = 0; i < 1000; i++)
        {
            g_Stack.Pop(dwThreadId);
        }
    }
}

int main()
{
    timeBeginPeriod(1);

    SYSTEM_INFO si;
    GetSystemInfo(&si);
    DWORD dwThreadNum = 4;
    HANDLE* hThread = new HANDLE[dwThreadNum];

    //InitializeCriticalSection(&cs);


#ifdef DEBUG
    for (int i = 0; i < 100000; i++)
    {
        g_Stack.Push(i);
    }
#endif // DEBUG


    for (DWORD i = 0; i < dwThreadNum; i++)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, LockFreeProc, NULL, 0, NULL);
    }

    WaitForMultipleObjects(dwThreadNum, hThread, TRUE, INFINITE);

    return 0;
}
