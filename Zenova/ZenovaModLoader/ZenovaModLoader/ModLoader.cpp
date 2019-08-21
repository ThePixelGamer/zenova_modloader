#include <Windows.h>
#include <ShlObj.h>
#include <AclAPI.h>
#include <Shlwapi.h>
#include <Sddl.h>
#include "ModLoader.h"
#include "utils.h"

DWORD ModLoader::AdjustGroupPolicy(std::wstring wstrFilePath) {
	PACL pOldDACL = NULL, pNewDACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS eaAccess;
	SECURITY_INFORMATION siInfo = DACL_SECURITY_INFORMATION;
	DWORD dwResult = ERROR_SUCCESS;
	PSID pSID;

	// Get a pointer to the existing DACL (Conditionaly).
	dwResult = GetNamedSecurityInfo(wstrFilePath.c_str(), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDACL, NULL, &pSD);
	if (dwResult != ERROR_SUCCESS)
		goto Cleanup;

	ConvertStringSidToSid(L"S-1-15-2-1", &pSID);
	if (pSID == NULL)
		goto Cleanup;

	ZeroMemory(&eaAccess, sizeof(EXPLICIT_ACCESS));
	eaAccess.grfAccessPermissions = GENERIC_READ | GENERIC_EXECUTE;
	eaAccess.grfAccessMode = SET_ACCESS;
	eaAccess.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
	eaAccess.Trustee.TrusteeForm = TRUSTEE_IS_SID;
	eaAccess.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	eaAccess.Trustee.ptstrName = (LPWSTR)pSID;

	// Create a new ACL that merges the new ACE into the existing DACL.
	dwResult = SetEntriesInAcl(1, &eaAccess, pOldDACL, &pNewDACL);
	if (ERROR_SUCCESS != dwResult)
		goto Cleanup;

	// Attach the new ACL as the object's DACL.
	dwResult = SetNamedSecurityInfo((LPWSTR)wstrFilePath.c_str(), SE_FILE_OBJECT, siInfo, NULL, NULL, pNewDACL, NULL);
	if (ERROR_SUCCESS != dwResult)
		goto Cleanup;

Cleanup:
	if (pSD != NULL)
		LocalFree((HLOCAL)pSD);
	if (pNewDACL != NULL)
		LocalFree((HLOCAL)pNewDACL);

	return dwResult;
}

BOOL ModLoader::InjectDLL(DWORD dwProcessId, std::wstring dllPath)
{
	BOOL status = TRUE;

	/* Find the address of the LoadLibrary API */
	HMODULE hLocKernel32 = GetModuleHandle(L"Kernel32");
	if (hLocKernel32 == NULL)
	{
		//if (Util::is_open()) Util::log(L"Could not get a handle on Kernel32 in the process (" + std::to_wstring(dwProcessId) + L"), HRESULT: " + std::to_wstring(GetLastError()));
		return FALSE;
	}

	FARPROC hLocLoadLibrary = GetProcAddress(hLocKernel32, "LoadLibraryW");
	if (hLocLoadLibrary == NULL)
	{
		//if (Util::is_open()) Util::log(L"Could not find the locatin of LoadLibraryW in the process (" + std::to_wstring(dwProcessId) + L"), HRESULT: " + std::to_wstring(GetLastError()));
		return FALSE;
	}

	/* Open the process with all access */
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProc == NULL)
	{
		//if (Util::is_open()) Util::log(L"Could not open the process (" + std::to_wstring(dwProcessId) + L"), HRESULT: " + std::to_wstring(GetLastError()));
		return FALSE;
	}

	/* Allocate memory to hold the path to the DLL File in the process's memory */
	dllPath += L'\0';
	SIZE_T dllPathSize = dllPath.size() * sizeof(wchar_t);
	LPVOID hRemoteMem = VirtualAllocEx(hProc, NULL, dllPathSize, MEM_COMMIT, PAGE_READWRITE);
	if (hRemoteMem == NULL)
	{
		//if (Util::is_open()) Util::log(L"Could not allocate memory in the process(" + std::to_wstring(dwProcessId) + L"), HRESULT: " + std::to_wstring(GetLastError()));
		return FALSE;
	}

	/* Write the path to the DLL File in the memory just allocated */
	SIZE_T numBytesWritten;
	status = WriteProcessMemory(hProc, hRemoteMem, dllPath.c_str(), dllPathSize, &numBytesWritten);
	if (!status)
	{
		//if (Util::is_open()) Util::log(L"Could not write memory in the process (" + std::to_wstring(dwProcessId) + L"), HRESULT: " + std::to_wstring(GetLastError()));
		return FALSE;
	}

	/* Create a remote thread that invokes LoadLibrary for our DLL */
	HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, hRemoteMem, 0, NULL);
	if (hRemoteThread == NULL)
	{
		//if (Util::is_open()) Util::log(L"Could not create a remote thread in the process (" + std::to_wstring(dwProcessId) + L"), HRESULT: " + std::to_wstring(GetLastError()));
		return FALSE;
	}

	CloseHandle(hProc);

	return status; 
}

HRESULT ModLoader::InjectMods(DWORD dwProcessId)
{
	std::wstring ModPath = Util::GetCommonFolder(FOLDERID_RoamingAppData) + L"\\Zenova\\Mods";
	//std::wstring ModPath = Util::GetCurrentDirectory() + L"\\Mods";
	if (ModPath.length() == 0 || !PathFileExists(ModPath.c_str()))
	{
		return E_FAIL;
	}

	HANDLE dir;
	WIN32_FIND_DATA fileData;

	//std::cout << "\nMod List:\n";

	if ((dir = FindFirstFile((ModPath + L"\\*").c_str(), &fileData)) == INVALID_HANDLE_VALUE)
		return E_FAIL; /* No files found */

	do {
		const std::wstring fileName = fileData.cFileName;
		const std::wstring filePath = ModPath + L"\\" + fileName;
		const bool is_directory = (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (fileName[0] == '.')
			continue;

		if (is_directory)
			continue;

		if(fileName.rfind(L".dll") != fileName.npos)
		{
			//std::wcout << fileName.c_str() << "\n";
			ModLoader::AdjustGroupPolicy(filePath);
			ModLoader::InjectDLL(dwProcessId, filePath.c_str());
		}
	} while (FindNextFile(dir, &fileData));

	FindClose(dir);

	//std::cout << "\n";
	//system("PAUSE");

	return S_OK;
};
