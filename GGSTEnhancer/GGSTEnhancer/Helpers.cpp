#include "Helpers.h"

void* module = GetModuleHandle(nullptr);

BYTE* PatternScan(const char* signature)
{
    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dosHeader = (PIMAGE_DOS_HEADER)module;
    auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = pattern_to_byte(signature);
    auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i) {
        bool found = true;
        for (auto j = 0ul; j < s; ++j) {
            if (scanBytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found) {
            return &scanBytes[i];
        }
    }
    return nullptr;
}

void Patch(BYTE* src, BYTE* dst, const ULONG64 size)
{
    DWORD curProtection;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &curProtection);
    memcpy_s(dst, size, src, size);
    VirtualProtect(dst, size, curProtection, &curProtection);
}

bool Detour64(BYTE* src, BYTE* dst, const ULONG64 size)
{
	if (size < 12) return false;
	DWORD curProtection;
	VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &curProtection);
	//mov rax, ULONG64
	*(BYTE*)src = 0x48;
	*(BYTE*)(src + 1) = 0xB8;
	*(ULONG64*)(src + 2) = (ULONG64)dst;
	//jmp rax
	*(BYTE*)(src + 10) = 0xFF;
	*(BYTE*)(src + 11) = 0xE0;
	VirtualProtect(src, size, curProtection, &curProtection);
	return true;
}

BYTE* TrampHook64(BYTE* src, BYTE* dst, const ULONG64 size)
{
	if (size < 12) return 0;
	BYTE* gateway = (BYTE*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, size, src, size);
	//mov rax, ULONG64
	*(BYTE*)(gateway + size) = 0x48;
	*(BYTE*)(gateway + size + 1) = 0xB8;
	*(ULONG64*)((ULONG64)gateway + size + 2) = (ULONG64)src + size;
	//jmp rax
	*(BYTE*)(gateway + size + 10) = 0xFF;
	*(BYTE*)(gateway + size + 11) = 0xE0;
	Detour64(src, dst, size);
	return gateway;
}

BYTE* RemoveHook(BYTE* src, BYTE* orig, const ULONG64 size)
{
	if (size < 12) return 0;
	DWORD curProtection;
	VirtualProtect(orig, size, PAGE_EXECUTE_READWRITE, &curProtection);
	memcpy_s(orig, size, src, size);
	VirtualProtect(orig, size, curProtection, &curProtection);
	VirtualFree(src, 0, MEM_RELEASE);
	return orig;
}
