#pragma once

#include "Zenova.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

#include <iostream>

//extern ZENOVA_API Zenova* Zenova;

template<typename T, typename... Targs>
T Call(const char* func, Targs... args);
template<typename T, typename... Targs>
T Call(uintptr_t func, Targs... args);

void DebugString(std::string name, const char* str);
void print(const char*);
uintptr_t GetAddress(uintptr_t result);
uintptr_t SlideAddress(uintptr_t offset);
uintptr_t GetModuleBaseAddress(const char* modName);
uintptr_t GetModuleBaseAddress(const wchar_t* modName);
DWORD GetModuleSize(DWORD, const char*);

uintptr_t GetSizeOfVtable(uintptr_t**);

template<typename T>
__declspec(noinline) bool CompareAndReplace(uintptr_t* vtable, uintptr_t* copyVtable) {
	if(vtable[0] != copyVtable[0]) { //prevent from running more than once
		uintptr_t size = GetSizeOfVtable((uintptr_t**)copyVtable); //should return 111 w/ the original Item vtable
		if(size == -1) return false;
		
		T* classPtr = new T;
		uintptr_t* tVtable = *(uintptr_t**)classPtr;

		DWORD oldProt;
		VirtualProtect(vtable, size, PAGE_EXECUTE_READWRITE, &oldProt);

		for(uintptr_t i = 0; i < size; i++) {
			if(vtable[i] == tVtable[i])
				vtable[i] = copyVtable[i];
		}
		
		DWORD temp;
		VirtualProtect(vtable, size, oldProt, &temp);

		delete classPtr;
		return true;
	}
	
	return false;
}