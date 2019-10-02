#define WIN32_LEAN_AND_MEAN
#include "ZenovaCommon.h"

#include <iostream>
#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

DWORD OnCreateThreadDebugEvent(const LPDEBUG_EVENT);
DWORD OnCreateProcessDebugEvent(const LPDEBUG_EVENT);
DWORD OnExitThreadDebugEvent(const LPDEBUG_EVENT);
DWORD OnExitProcessDebugEvent(const LPDEBUG_EVENT);
DWORD OnLoadDllDebugEvent(const LPDEBUG_EVENT);
DWORD OnUnloadDllDebugEvent(const LPDEBUG_EVENT);
DWORD OnOutputDebugStringEvent(const LPDEBUG_EVENT);
DWORD OnRipEvent(const LPDEBUG_EVENT);

DWORD FindProcessId(const std::wstring& processName);

HANDLE process;

void __stdcall ZenovaDebuggerTest() {
	DWORD pid = 0;

	while(pid == 0) {
		pid = FindProcessId(L"Minecraft.Windows.exe");
	}

	printf("Minecraft found: %d\n", pid);

	process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	DebugActiveProcess(pid);

	DEBUG_EVENT DebugEv; //create a DEBUG_EVENT to get and handle debug events

	DWORD dwContinueStatus = DBG_CONTINUE; // exception continuation 

	for(;;) {
		// Wait for a debugging event to occur. The second parameter indicates
		// that the function does not return until a debugging event occurs. 

		WaitForDebugEvent(&DebugEv, INFINITE);

		// Process the debugging event code. 

		switch(DebugEv.dwDebugEventCode) {
			case EXCEPTION_DEBUG_EVENT:
				// Process the exception code. When handling 
				// exceptions, remember to set the continuation 
				// status parameter (dwContinueStatus). This value 
				// is used by the ContinueDebugEvent function. 
				switch(DebugEv.u.Exception.ExceptionRecord.ExceptionCode) {
					case EXCEPTION_ACCESS_VIOLATION:
					{
						// First chance: Pass this on to the system. 
						// Last chance: Display an appropriate error.

						//RaiseException(DebugEv.u.Exception.ExceptionRecord.ExceptionCode, DebugEv.u.Exception.ExceptionRecord.ExceptionFlags, NULL, NULL);

					} break;

					case EXCEPTION_BREAKPOINT:
						// First chance: Display the current 
						// instruction and register values. 
						break;

					case EXCEPTION_DATATYPE_MISALIGNMENT:
						// First chance: Pass this on to the system. 
						// Last chance: Display an appropriate error. 
						break;

					case EXCEPTION_SINGLE_STEP:
						// First chance: Update the display of the 
						// current instruction and register values. 
						break;

					case DBG_CONTROL_C:
						// First chance: Pass this on to the system. 
						// Last chance: Display an appropriate error. 
						break;

					default:
						// Handle other exceptions. 
						break;
				}

				dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;

				break;

			case CREATE_THREAD_DEBUG_EVENT:
				// As needed, examine or change the thread's registers 
				// with the GetThreadContext and SetThreadContext functions; 
				// and suspend and resume thread execution with the 
				// SuspendThread and ResumeThread functions. 

				dwContinueStatus = OnCreateThreadDebugEvent(&DebugEv);
				break;

			case CREATE_PROCESS_DEBUG_EVENT:
				// As needed, examine or change the registers of the
				// process's initial thread with the GetThreadContext and
				// SetThreadContext functions; read from and write to the
				// process's virtual memory with the ReadProcessMemory and
				// WriteProcessMemory functions; and suspend and resume
				// thread execution with the SuspendThread and ResumeThread
				// functions. Be sure to close the handle to the process image
				// file with CloseHandle.

				dwContinueStatus = OnCreateProcessDebugEvent(&DebugEv);
				break;

			case EXIT_THREAD_DEBUG_EVENT:
				// Display the thread's exit code. 

				dwContinueStatus = OnExitThreadDebugEvent(&DebugEv);
				break;

			case EXIT_PROCESS_DEBUG_EVENT:
				// Display the process's exit code. 

				dwContinueStatus = OnExitProcessDebugEvent(&DebugEv);
				break;

			case LOAD_DLL_DEBUG_EVENT:
				// Read the debugging information included in the newly 
				// loaded DLL. Be sure to close the handle to the loaded DLL 
				// with CloseHandle.

				dwContinueStatus = OnLoadDllDebugEvent(&DebugEv);
				break;

			case UNLOAD_DLL_DEBUG_EVENT:
				// Display a message that the DLL has been unloaded. 

				dwContinueStatus = OnUnloadDllDebugEvent(&DebugEv);
				break;

			case OUTPUT_DEBUG_STRING_EVENT:
				// Display the output debugging string. 

				dwContinueStatus = OnOutputDebugStringEvent(&DebugEv);
				break;

			case RIP_EVENT:
				dwContinueStatus = OnRipEvent(&DebugEv);
				break;
		}

		// Resume executing the thread that reported the debugging event. 

		ContinueDebugEvent(DebugEv.dwProcessId,
			DebugEv.dwThreadId,
			dwContinueStatus);
	}

	CloseHandle(process);
}

DWORD FindProcessId(const std::wstring& processName) {
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile)) {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo)) {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

DWORD OnCreateThreadDebugEvent(const LPDEBUG_EVENT) {
	return DBG_CONTINUE;
}

DWORD OnCreateProcessDebugEvent(const LPDEBUG_EVENT) {
	return DBG_CONTINUE;
}

DWORD OnExitThreadDebugEvent(const LPDEBUG_EVENT) {
	return DBG_CONTINUE;
}

DWORD OnExitProcessDebugEvent(const LPDEBUG_EVENT) {
	return DBG_CONTINUE;
}

DWORD OnLoadDllDebugEvent(const LPDEBUG_EVENT) {
	return DBG_CONTINUE;
}

DWORD OnUnloadDllDebugEvent(const LPDEBUG_EVENT) {
	return DBG_CONTINUE;
}

DWORD OnOutputDebugStringEvent(const LPDEBUG_EVENT dbgEv) {
	//if(dbgEv->u.DebugString.fUnicode)
	//	SetConsoleOutputCP(CP_UTF8);
	//else
	//	SetConsoleOutputCP(CP_ACP);

	std::vector<char> debugStringChars(dbgEv->u.DebugString.nDebugStringLength);

	size_t lpNumberOfBytesRead = 0;
	ReadProcessMemory(process, dbgEv->u.DebugString.lpDebugStringData, debugStringChars.data(), debugStringChars.size(), &lpNumberOfBytesRead);
	std::string temp(debugStringChars.begin(), debugStringChars.end());
	ZenovaCommon::logger.AddLog("%s\n", temp.c_str());

	return DBG_CONTINUE;
}

DWORD OnRipEvent(const LPDEBUG_EVENT) {
	return DBG_CONTINUE;
}