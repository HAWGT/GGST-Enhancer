#pragma once

#include <Windows.h>
#include <basetsd.h>
#include <minwindef.h>
#include <memoryapi.h>
#include <vector>

BYTE* PatternScan(const char* signature);
void Patch(BYTE* src, BYTE* dst, const ULONG64 size);
bool Detour64(BYTE* src, BYTE* dst, const ULONG64 size);
BYTE* TrampHook64(BYTE* src, BYTE* dst, const ULONG64 size);
BYTE* RemoveHook(BYTE* src, BYTE* orig, const ULONG64 size);
