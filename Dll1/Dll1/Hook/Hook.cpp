#include "Hook.h"

#include <Windows.h>
#include <cstdint>

uintptr_t BaseAddress;

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
    if (Module32First(hSnap, &xModule)) {
        while (Module32Next(hSnap, &xModule)) {
            if (!strncmp((char*)xModule.szModule, module, 8)) {
                CloseHandle(hSnap);
                return (DWORD)xModule.modBaseSize;
            }
        }
    }
    CloseHandle(hSnap);
    return 0;
}

/*
TODO:
	-Implement x86 decoder for checking the function length + automatically nopping the rest of the bytes in that instruction 
*/
void Hook::safeHook(void* addr, uintptr_t replacement, void** p_original, int len) {
	DWORD oldProc;
	HANDLE minecraft = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	BOOL virtualProcResult = VirtualProtectEx(minecraft, addr, len, PAGE_EXECUTE_READWRITE, &oldProc); //change the protection in order to be able to write to it
	//print1 = VirtualProtectEx((LPVOID)BaseAddress, 0x1a15400, PAGE_EXECUTE_READWRITE, &oldProc);
	//print2 = VirtualProtectEx((LPVOID)GetModuleBaseAddress(GetCurrentProcessId(), "Dll1.dll"), 0x4400, PAGE_EXECUTE_READWRITE, &oldProc2);
	if(virtualProcResult) {
		void* p_og = VirtualAlloc(NULL, len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if(p_og) {
			*p_original = memcpy(p_og, addr, len);
			*((uint64_t*)(static_cast<uint8_t*>(*p_original) + len)) = (reinterpret_cast<uintptr_t>(addr) + len);
		}
		else
			return;

		uint8_t shellcode[] = {0xFF, 0x25, 0x00, 0x00, 0x00, 0x00};
		memcpy(addr, shellcode, sizeof(shellcode));
		*((uint64_t*)(static_cast<uint8_t*>(addr) + 6)) = replacement;

		virtualProcResult = VirtualProtect(addr, 0x12, oldProc, &oldProc); //restore the old protection and store our protection
	}

	if(minecraft)
		CloseHandle(minecraft);
}