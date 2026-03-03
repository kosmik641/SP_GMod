// Based on sources:
// https://github.com/Gaztoof/GMod-SDK/blob/main/GMod-SDK/Memory.c
// https://github.com/Gaztoof/GMod-SDK/blob/main/GMod-SDK/Memory.h

#ifndef _WIN32
typedef void *PVOID;
#else
#include <Windows.h>
#endif

#include <vector>


struct VMTHookData_t {
    PVOID** src;
    PVOID dst;
    int index;
};

extern std::vector<VMTHookData_t> g_VMTHooks;


bool SetMemoryProtect(void* address, size_t size, int access, int* orig_access)
{
#if defined(_WIN32)
    return VirtualProtect(address, size, access, (PDWORD)orig_access);
#else
    return false;
#endif // 
}

template<typename T>
T VMTHook(PVOID** src, PVOID dst, int index, bool noRestore = false)
{
    // I could do tramp hooking instead of vmt hooking, but I came across a few problems while implementing my tramp, and vmt just makes it easier.
    PVOID* vmt = *src;
    PVOID orig = (vmt[index]);
    int originalProtection;
    SetMemoryProtect(&vmt[index], sizeof(PVOID), PAGE_EXECUTE_READWRITE, &originalProtection);
    vmt[index] = dst;
    SetMemoryProtect(&vmt[index], sizeof(PVOID), originalProtection, &originalProtection);
    if (!noRestore)
    {
        VMTHookData_t currData = { src, orig, index };
        g_VMTHooks.push_back(currData);
    }
    return (T)orig;
};

void VMTHook(PVOID** src, PVOID dst, int index)
{
    PVOID* vmt = *src;
    int originalProtection;
    SetMemoryProtect(&vmt[index], sizeof(PVOID), PAGE_EXECUTE_READWRITE, &originalProtection);
    vmt[index] = dst;
    SetMemoryProtect(&vmt[index], sizeof(PVOID), originalProtection, &originalProtection);
}

void RestoreVMTHooks()
{
    for (auto hook : g_VMTHooks)
    {
        VMTHook(hook.src, hook.dst, hook.index);
    }
}