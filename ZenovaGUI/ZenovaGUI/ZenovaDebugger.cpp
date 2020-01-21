#define WIN32_LEAN_AND_MEAN
#include "ZenovaCommon.h"

#include <iostream>
#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "ProcessUtils.h"

DWORD OnCreateThreadDebugEvent(const LPDEBUG_EVENT);
DWORD OnCreateProcessDebugEvent(const LPDEBUG_EVENT);
DWORD OnExitThreadDebugEvent(const LPDEBUG_EVENT);
DWORD OnExitProcessDebugEvent(const LPDEBUG_EVENT);
DWORD OnLoadDllDebugEvent(const LPDEBUG_EVENT);
DWORD OnUnloadDllDebugEvent(const LPDEBUG_EVENT);
DWORD OnOutputDebugStringEvent(const LPDEBUG_EVENT);
DWORD OnRipEvent(const LPDEBUG_EVENT);

DWORD FindProcessId(const std::wstring& processName);
bool EnableDebugPrivilege();

HANDLE process;
void launchProcess(std::string app, std::string arg) {
	// Prepare handles.
	STARTUPINFOA si;
	PROCESS_INFORMATION pi; // The function returns this
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process.
	if(!CreateProcessA(app.c_str(), const_cast<char*>(arg.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		throw std::exception("Could not create child process");
	}
	
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void _stdcall ZenovaDebugger() {
	ZenovaDebugger(L"Minecraft.Windows.exe");
}

/*temporary solution till I can figure out why the fuck my own 
doesn't work has to do with STATUS_STACK_BUFFER_OVERRUN 
(my debugger gets in earlier than visual studio's so maybe 
it has to do with ActivateApplication doing some bullshit, I'll probably
make a thread on stackoverflow or some of MS' bullshit)*/
void _stdcall ZenovaDebugger(const std::wstring& title) {
	DWORD pid = 0;

	while(pid == 0) {
		pid = FindProcessId(title);
	}
	
	ProcessUtils::SuspendProcess(pid);

	//vsjitdebugger.exe -p pid
	launchProcess("C:\\Windows\\System32\\vsjitdebugger.exe", std::string(" -p " + std::to_string(pid)));

	printf("%ls found: %d\n", title.c_str(), pid);

	BOOL debuggerPresent = false;
	process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	while(!debuggerPresent) {
		CheckRemoteDebuggerPresent(process, &debuggerPresent);
	}
	
	ProcessUtils::ResumeProcess(pid);
	//ZenovaDebugger(pid);
}

void _stdcall ZenovaDebugger(std::reference_wrapper<DWORD> pid) {
	EnableDebugPrivilege();

	while(pid == 0) {} //pid never changes with std::ref (doesn't matter the scope)

	process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	DebugActiveProcess(pid);

	DEBUG_EVENT DebugEv; //create a DEBUG_EVENT to get and handle debug events
	DWORD dwContinueStatus = DBG_CONTINUE; // exception continuation 
	bool bContinueDebugging = true;

	while(bContinueDebugging) {
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
						ZenovaCommon::logger.AddLog("[EXCEPTION_ACCESS_VIOLATION]\n");
						dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
					} break;

					case EXCEPTION_BREAKPOINT:
						// First chance: Display the current 
						// instruction and register values. 
						ZenovaCommon::logger.AddLog("[EXCEPTION_BREAKPOINT]\n");
						dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
						break;

					case EXCEPTION_DATATYPE_MISALIGNMENT:
						// First chance: Pass this on to the system. 
						// Last chance: Display an appropriate error. 
						ZenovaCommon::logger.AddLog("[EXCEPTION_DATATYPE_MISALIGNMENT]\n");
						dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
						break;

					case EXCEPTION_SINGLE_STEP:
						// First chance: Update the display of the 
						// current instruction and register values. 
						ZenovaCommon::logger.AddLog("[EXCEPTION_SINGLE_STEP]\n");
						dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
						break;

					case DBG_CONTROL_C:
						// First chance: Pass this on to the system. 
						// Last chance: Display an appropriate error.
						ZenovaCommon::logger.AddLog("[DBG_CONTROL_C]\n");
						dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
						break;

					default:
						// Handle other exceptions.
						OnCreateProcessDebugEvent(&DebugEv);
						ZenovaCommon::logger.AddLog("[DEFAULT_EXCEPTION] 0x%X\n", DebugEv.u.Exception.ExceptionRecord.ExceptionCode);
						dwContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
						break;
				}

				

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
				bContinueDebugging = false;
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

		ContinueDebugEvent(DebugEv.dwProcessId, DebugEv.dwThreadId, dwContinueStatus);
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
        if (!processName.compare(processInfo.szExeFile)) {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

bool EnableDebugPrivilege() {
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		ZenovaCommon::logger.AddLog("OpenProcessToken failed\n");
		return false;
    }

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
		ZenovaCommon::logger.AddLog("LookupPrivilegeValue failed\n");
        return false;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL)) {
		ZenovaCommon::logger.AddLog("AdjustTokenPrivileges failed\n");
        return false;
    }

    if (!CloseHandle(hToken)) {
		ZenovaCommon::logger.AddLog("CloseHandle failed\n");
        return false;
    }

	ZenovaCommon::logger.AddLog("Enabled Debug Privileges\n");

    return true;
}

DWORD OnCreateThreadDebugEvent(const LPDEBUG_EVENT) {
	ZenovaCommon::logger.AddLog("[CREATE_THREAD_DEBUG_EVENT]\n");
	return DBG_CONTINUE;
}

DWORD OnCreateProcessDebugEvent(const LPDEBUG_EVENT) {
	ZenovaCommon::logger.AddLog("[CREATE_PROCESS_DEBUG_EVENT]\n");
	return DBG_CONTINUE;
}

DWORD OnExitThreadDebugEvent(const LPDEBUG_EVENT) {
	ZenovaCommon::logger.AddLog("[EXIT_THREAD_DEBUG_EVENT]\n");
	return DBG_CONTINUE;
}

DWORD OnExitProcessDebugEvent(const LPDEBUG_EVENT) {
	ZenovaCommon::logger.AddLog("[EXIT_PROCESS_DEBUG_EVENT]\n");
	return DBG_CONTINUE;
}

DWORD OnLoadDllDebugEvent(const LPDEBUG_EVENT) {
	ZenovaCommon::logger.AddLog("[LOAD_DLL_DEBUG_EVENT]\n");
	return DBG_CONTINUE;
}

DWORD OnUnloadDllDebugEvent(const LPDEBUG_EVENT) {
	ZenovaCommon::logger.AddLog("[UNLOAD_DLL_DEBUG_EVENT]\n");
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
	ZenovaCommon::logger.AddLog("%s", temp.c_str());

	return DBG_CONTINUE;
}

DWORD OnRipEvent(const LPDEBUG_EVENT) {
	ZenovaCommon::logger.AddLog("[RIP_EVENT]\n");
	return DBG_CONTINUE;
}