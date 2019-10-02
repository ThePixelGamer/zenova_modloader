#pragma once

#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_

#include <Windows.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <string>
#include <unordered_set>

static uintptr_t BaseAddress;

uintptr_t SlideAddress(uintptr_t offset);
uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName);
DWORD GetModuleSize(DWORD, const char*);