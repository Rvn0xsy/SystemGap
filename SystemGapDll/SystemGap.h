#pragma once
#include <Windows.h>
#include <stdio.h>
#include <aclapi.h>

#define BUFF_SIZE 1024

BOOL GenerateEveryoneSecAttr(PSECURITY_ATTRIBUTES sa);

HANDLE CreateSystemGap(const char* gap_name);

void HandleGapMsg(HANDLE hGap);