#include "SceneBase.h"
#include "SceneManager.h"
#include <Windows.h>


int main()
{

    while (1)
    {
        if(!CSceneManager::GetInstance()->run())
            break;

        Sleep(100);
    }
}