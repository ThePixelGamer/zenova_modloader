#pragma once

#include <Windows.h>
#include <TlHelp32.h>

#ifdef ZENOVA_DLL_COMPILE
#define ZENOVA_API extern "C" __declspec(dllexport)
#else
#define ZENOVA_API extern "C" __declspec(dllimport)
#endif

ZENOVA_API uintptr_t BaseAddress;

uintptr_t SlideAddress(uintptr_t offset);
uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName);
DWORD GetModuleSize(DWORD, const char*);

class Hook {
private:
	//members

public:
	static void safeHook(void*, uintptr_t, void**, int);
};