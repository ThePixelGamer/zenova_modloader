#include "ZenovaCore.h"
#include <TlHelp32.h>

extern uintptr_t BaseAddress(0);

template<typename T, typename... Targs>
T Call(const char* func, Targs... args) {
	//setup a lookup map using a file
}

template<typename T, typename... Targs>
T Call(uintptr_t func, Targs... args) {
	((T (*)(Targs))SlideAddress(func))(args...);
}

uintptr_t GetAddress(uintptr_t result) {
	return result - BaseAddress;
}

uintptr_t SlideAddress(uintptr_t offset) {
	return BaseAddress + offset;
}

void DebugString(std::string name, const char* str) {
	OutputDebugString(std::string("[" + name + "] " + str).c_str());
}

void print(const char* str) {
	DebugString("test", str);
}

uintptr_t GetModuleBaseAddress(const char* modName) {
	DWORD procId = GetCurrentProcessId();
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

uintptr_t GetModuleBaseAddress(const wchar_t* modName) {
	DWORD procId = GetCurrentProcessId();
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if(hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32W modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if(Module32FirstW(hSnap, &modEntry)) {
			do {
				if(!_wcsicmp(modEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while(Module32NextW(hSnap, &modEntry));
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


/*
All of the RTTI class object loaders start from 1, if there's an exception
to this rule (in the original code that is) please let me know by filling an
issue on the github page.
*/
uintptr_t GetSizeOfVtable(uintptr_t** vtableToCheck) {
	for(uintptr_t size = 0; true; size++) {
		if(*(vtableToCheck[size]) == 1) {
			return size;
		}
	}
	
	return -1;
}

/*
I'll have to adjust this for classes we make that inherit multiple vtables
(Also in no way should this be used for our "template" classes of the originals, instead 
the class' virtual functions should instead be filled with calls to the originals to allow 
for this [calling Item::setMaxStackSize from ExampleItem::setMaxStackSize])
*/