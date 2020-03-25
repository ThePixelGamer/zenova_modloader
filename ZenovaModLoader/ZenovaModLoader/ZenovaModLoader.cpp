#include <Windows.h>
#include <ShObjIdl.h>
#include <fstream>

#include "../../Utils/AppUtils.h"
#include "../../Utils/ProcessUtils.h"
#include "ModLoader.h"

#define IMPORT extern __declspec(dllimport)

IMPORT int __argc;
IMPORT char** __argv;
//IMPORT wchar_t** __wargv;

void LaunchProcess(std::string app, std::string arg) {
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

// Turning this into a normal Windows program hides the GUI
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	DWORD dwProcessId = 0;

	for(int i = 1; i < __argc; i += 2) {
		//printf("%s, %s\n", __argv[i], __argv[i+1]);
		std::string arg(__argv[i]);
		if(arg == "-p") {
			dwProcessId = atoi(__argv[i + 1]);
		}
	}

    LaunchProcess("C:\\Windows\\System32\\vsjitdebugger.exe", std::string(" -p " + std::to_string(dwProcessId)));

	if(dwProcessId != 0 && SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
		std::wstring AppFullName = AppUtils::GetMinecraftPackageId();
		AppUtils::AppDebugger app(AppFullName);

		if(app.GetPackageExecutionState() == PES_UNKNOWN) {
			CoUninitialize();
			return E_FAIL;
		}

		// Assume the game is suspended and inject mods
		ModLoader::InjectMods(dwProcessId);
		
		// Resume the game
		ProcessUtils::ResumeProcess(dwProcessId);

		CoUninitialize();
	}

	return S_OK;
}