#include "Zenova.h"

//uintptr_t BaseAddress;

uintptr_t SlideAddress(uintptr_t offset) {
	return BaseAddress + offset;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName) {
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if(hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if(Module32First(hSnap, &modEntry)) {
			do {
				if(!_stricmp(modEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while(Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

DWORD GetModuleSize(DWORD processID, const char* module) {
	HANDLE hSnap;
	MODULEENTRY32 xModule;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
	xModule.dwSize = sizeof(MODULEENTRY32);
	if(Module32First(hSnap, &xModule)) {
		while(Module32Next(hSnap, &xModule)) {
			if(!strncmp(xModule.szModule, module, 8)) {
				CloseHandle(hSnap);
				return (DWORD)xModule.modBaseSize;
			}
		}
	}
	CloseHandle(hSnap);
	return 0;
}