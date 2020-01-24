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

uintptr_t GetAddress(uintptr_t result);
void* SlideAddress(size_t offset);
uint8_t* GetModuleBaseAddress(const wchar_t* modName);
DWORD GetModuleSize(DWORD, const char*);

uintptr_t GetSizeOfVtable(uintptr_t**);

//TODO: Handle cases where the user implements the first virtual function
template<typename T>
__declspec(noinline) bool CompareAndReplace(uintptr_t* vtable, uintptr_t* copyVtable) {
	if(vtable[0] != copyVtable[0]) { //prevent from running more than once
		uintptr_t size = GetSizeOfVtable(reinterpret_cast<uintptr_t**>(copyVtable)); //should return 111 w/ the original Item vtable
		if(size == -1) return false;
		
		std::shared_ptr<T> classPtr = std::make_shared<T>();
		uintptr_t* tVtable = *reinterpret_cast<uintptr_t**>(classPtr.get());

		DWORD oldProt, unused;
		VirtualProtect(vtable, size, PAGE_EXECUTE_READWRITE, &oldProt);

		for(size_t i = 0; i < size; i++) {
			if(vtable[i] == tVtable[i])
				vtable[i] = copyVtable[i];
		}
		
		VirtualProtect(vtable, size, oldProt, &unused);

		return true;
	}
	
	return false;
}