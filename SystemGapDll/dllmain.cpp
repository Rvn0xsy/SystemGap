﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "SystemGap.h"

HANDLE hGap = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hGap = CreateSystemGap("\\\\.\\pipe\\SystemGap");
        if (hGap != INVALID_HANDLE_VALUE)
        {
            HandleGapMsg(hGap);
        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

