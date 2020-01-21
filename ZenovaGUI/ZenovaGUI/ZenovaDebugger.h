#pragma once

#include <Windows.h>
#include <functional>

void ZenovaDebugger();
void ZenovaDebugger(const std::wstring&);
void ZenovaDebugger(std::reference_wrapper<DWORD>);