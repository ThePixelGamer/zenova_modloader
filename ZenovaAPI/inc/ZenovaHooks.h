#pragma once

static HANDLE(__stdcall *pfnCreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
static HANDLE(__stdcall *pfnCreateDirectoryW)(LPCWSTR, LPSECURITY_ATTRIBUTES);
static HANDLE(__stdcall *pfnCreateDirectoryA)(LPCSTR, LPSECURITY_ATTRIBUTES);