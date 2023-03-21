// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include "Features.h"
#include <iostream>

void PatchAll()
{
    while (!PatchOnlineMultiplayerColorSelect()) {};
    while (!UncensorMuseum()) {};
    while (!PatchIsUnlocked()) {};
    while (!ImproveFishing()) {};
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PatchAll, hModule, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

