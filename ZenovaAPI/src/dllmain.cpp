//Windows
#include <Windows.h>
#include <TlHelp32.h>
#include <atlbase.h>
#include <Shlobj.h>
#include <direct.h>

//C
#include <cstdio>
#include <cstdint>

//C++
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include <fstream>
#include <algorithm>

#include "MinHook.h"
#include "ZenovaCore.h"

#include "ExampleMod/ExampleItems.h"

//Minecraft Wide String Path
std::wstring MC_WS_PATH;
//Minecraft String Path
std::string MC_S_PATH;

std::string MC_MIRROR_DIR;

HMODULE dllInstance = NULL;
extern uintptr_t BaseAddress;

void (*_registerItems)(bool);
void registerItems(bool b1) {
	_registerItems(b1);

	ExampleItems::init();
}

void (*_initCreativeItemsCallback)(class ActorInfoRegistry*, class BlockDefinitionGroup*, bool);
void initCreativeItemsCallback(class ActorInfoRegistry* actorInfoRegistry, class BlockDefinitionGroup* blockDefinitionGroup, bool b1) {
	_initCreativeItemsCallback(actorInfoRegistry, blockDefinitionGroup, b1);

	ExampleItems::initCreativeItems();
}

void (*_initClientData)();
void initClientData() {
	_initClientData();

	ExampleItems::loadResources();
}

bool FileExists(LPCWSTR fileName) {
    if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(fileName) && GetLastError() == ERROR_FILE_NOT_FOUND) return false;
    return true;
}

std::string getAccessRight(DWORD& dwDesiredAccess) {
	switch(dwDesiredAccess) {
		case GENERIC_ALL:
			return "GENERIC_ALL";
		case GENERIC_EXECUTE:
			return "GENERIC_EXECUTE";
		case GENERIC_READ:
			return "GENERIC_READ";
		case GENERIC_WRITE:
			return "GENERIC_WRITE";
		default:
			return std::to_string(dwDesiredAccess);
	}
}

std::string getShareRight(DWORD& dwShareMode) {
	switch(dwShareMode) {
		case 0:
			return "No Sharing";
		case FILE_SHARE_DELETE:
			return "FILE_SHARE_DELETE";
		case FILE_SHARE_READ:
			return "FILE_SHARE_READ";
		case FILE_SHARE_WRITE:
			return "FILE_SHARE_WRITE";
		default:
			return std::to_string(dwShareMode);
	}
}

std::string getCreationDisposition(DWORD& dwCreationDisposition) {
	switch(dwCreationDisposition) {
		case CREATE_ALWAYS:
			return "CREATE_ALWAYS";
		case CREATE_NEW:
			return "CREATE_NEW";
		case OPEN_ALWAYS:
			return "OPEN_ALWAYS";
		case OPEN_EXISTING:
			return "OPEN_EXISTING";
		case TRUNCATE_EXISTING:
			return "TRUNCATE_EXISTING";
		default:
			return std::to_string(dwCreationDisposition);
	}
}

std::string getFlagsAndAttributes(DWORD& dwFlagsAndAttributes) {
	switch(dwFlagsAndAttributes) { //only some of the basic ones
		case FILE_ATTRIBUTE_ARCHIVE:
			return "FILE_ATTRIBUTE_ARCHIVE";
		case FILE_ATTRIBUTE_ENCRYPTED:
			return "FILE_ATTRIBUTE_ENCRYPTED";
		case FILE_ATTRIBUTE_HIDDEN:
			return "FILE_ATTRIBUTE_HIDDEN";
		case FILE_ATTRIBUTE_NORMAL:
			return "FILE_ATTRIBUTE_NORMAL";
		case FILE_ATTRIBUTE_OFFLINE:
			return "FILE_ATTRIBUTE_OFFLINE";
		case FILE_ATTRIBUTE_READONLY:
			return "FILE_ATTRIBUTE_READONLY";
		case FILE_ATTRIBUTE_SYSTEM:
			return "FILE_ATTRIBUTE_SYSTEM";
		case FILE_ATTRIBUTE_TEMPORARY:
			return "FILE_ATTRIBUTE_TEMPORARY";
		default:
			return std::to_string(dwFlagsAndAttributes);
	}
}

HANDLE(WINAPI *pfnCreateFileW)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
HANDLE WINAPI HfnCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
	//Can't redirect File Creates to folders the UWP app doesn't have access to
	std::wstring filePath(lpFileName);
	
	// Check if it's accessing resources; this method should work in all future updates
	if(filePath.find(MC_WS_PATH) != filePath.npos) {
		std::wcout << "File: " << filePath << "\n";
		std::cout << "{ " << getAccessRight(dwDesiredAccess) << ", " 
			<< getShareRight(dwShareMode) <<  ", "
			<< getCreationDisposition(dwCreationDisposition) << ", " 
			<< getFlagsAndAttributes(dwFlagsAndAttributes) << " }" << "\n\n" << std::flush;
	
		std::wstring newPath(L"D:/minecraftWorlds/");
		std::wstring strToFind(L"/minecraftWorlds/");
		newPath += filePath.substr(filePath.find(strToFind) + strToFind.size(), filePath.size());

		// Check if a file exists at that path and reroute access to that file
		//if(FileExists(newPath.c_str()))
			//return pfnCreateFileW(newPath.c_str(), dwAccess, dwSharing, saAttributes, dwCreation, dwAttributes, hTemplate);
	}

	return pfnCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE(WINAPI *pfnCreateDirectoryA)(LPCSTR, LPSECURITY_ATTRIBUTES);
HANDLE WINAPI HfnCreateDirectoryA(LPCSTR lpFilename, LPSECURITY_ATTRIBUTES saAttributes) {
	//Can't redirect Directory Creates to folders the UWP app doesn't have access to
	std::string filePath(lpFilename);
	std::cout << filePath << std::endl;

	// Check if it's accessing resources; this method should work in all future updates
	if(filePath.find(MC_S_PATH) != filePath.npos) {
		std::string newPath("D:/minecraftWorlds/");
		std::string newDir(newPath);
		std::string strToFind("/minecraftWorlds/");
		size_t startPoint = filePath.find(strToFind) + strToFind.size();
		newPath += filePath.substr(startPoint);
		
		size_t endPoint = filePath.find("/", startPoint) - startPoint;
		newDir += filePath.substr(startPoint, endPoint);

		// Check if a file exists at that path and reroute access to that file
		//if(FileExists(newPath.c_str()))
			//return pfnCreateDirectoryW(newPath.c_str(), saAttributes);
	}

	return pfnCreateDirectoryA(lpFilename, saAttributes);
}

//namespace Core {
//	class FilePathManager;
//}
//
//namespace Social {
//	class IUserManager;
//}
//
//void (*_init)(class ClientInstance*, Core::FilePathManager*, class NetworkHandler&, class Timer&, class Timer&, Social::IUserManager&, int);
//void init(class ClientInstance* ci_this, Core::FilePathManager* core_FilePathManager, class NetworkHandler& networkHandler, class Timer& timer1, class Timer& timer2, Social::IUserManager& social_IUserManager, int i1) {
//	_init(ci_this, core_FilePathManager, networkHandler, timer1, timer2, social_IUserManager, i1);
//
//	ExampleItemManager::passInstance(ci_this);
//}

uintptr_t FindAddress(const char* strModule, const char* strFunction) {
	HMODULE hModule = GetModuleHandleA(strModule);
	if(hModule == NULL)
		return 9;

	FARPROC hFunction = GetProcAddress(hModule, strFunction);
	if(hFunction == NULL)
		return 0;

	return (uintptr_t)hFunction;
}

bool CreateHook(void* address, void* funcJump, void** funcTrampoline) {
	if(MH_CreateHook(address, funcJump, funcTrampoline) != MH_OK) 
		return false;

	if(MH_EnableHook(address) != MH_OK) 
		return false;

	//MSHookFunction for android

	return true;
}

bool CreateHook(std::string funcName, void* funcJump, void** funcTrampoline) {
	//setup a lookup map to grab the address
}

void setBaseDir() {
	std::array<wchar_t, MAX_PATH> szPath;
	if(SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath.data()))) {
		// Get the path to the textures folder
		std::wstring appData(szPath.data());

		appData = appData.substr(0, appData.rfind(L"AC"));
		appData += L"LocalState/games/com.mojang/";

		//Change all the trailing backslashes into forward slash
		std::wstring replaced = L"\\";
		for(size_t start = 0, pos = 0, end = appData.find(L"LocalState"); start != end; start = pos + replaced.length()) {
			pos = appData.find(replaced, start);
			appData.replace(pos, replaced.length(), L"/");
		}

		//SHGetFolderPathW doesn't properly captialize this, use lowercase in the future?
		replaced = L"microsoft.minecraftuwp";
		appData.replace(appData.find(replaced), replaced.length(), L"Microsoft.MinecraftUWP");

		MC_WS_PATH = appData;
		MC_S_PATH = std::string(appData.begin(), appData.end());
	}
}

void setMirrorRoot(const char* mirror) {
	MC_MIRROR_DIR = mirror;
}

//Basically all the bare minimum hooks to get this thing running
void setupHooks() {
	//uintptr_t** vtable = (uintptr_t**)SlideAddress(0x2460D70) + 1; //to avoid calling the virtual deconstructor
	//CreateHook((void*) SlideAddress(0x11848A0), (void*) &registerItems, (void**) &_registerItems); //VanillaItems::registerItems(bool)
	//CreateHook((void*) SlideAddress(0x1193AF0), (void*) &initCreativeItemsCallback, (void**) &_initCreativeItemsCallback); //VanillaItems::initCreativeItemsCallback(ActorInfoRegistry*, BlockDefinitionGroup*, bool)
	//CreateHook((void*) SlideAddress(0x118F7F0), (void*) &initClientData, (void**) &_initClientData); //VanillaItems::initClientData()
	CreateHook((void*) FindAddress("KernelBase.dll", "CreateFileW"), (void*) &HfnCreateFileW, (void**) &pfnCreateFileW);
	CreateHook((void*) FindAddress("KernelBase.dll", "CreateDirectoryA"), (void*) &HfnCreateDirectoryA, (void**) &pfnCreateDirectoryA);
	//CreateHook((void*) SlideAddress(0x008F060), (void*) &init, (void**) &_init); //ClientInstance::init()
}

void __stdcall ZenovaStart() {
	if(MH_Initialize() != MH_OK) {
		return;
	}

	print("Zenova Started");

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONIN$", "r", stdin);

	setBaseDir();
	setMirrorRoot("minecraftWorlds/");
	setupHooks();

	//HMODULE module = LoadLibraryA();
	
	std::cout << std::hex << BaseAddress << std::endl;

	bool run = true;
	while(run) {} //maybe do something with this? /shrug
	
	if(f) fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(dllInstance, 0);

	print("Zenova Stopped");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	BaseAddress = GetModuleBaseAddress("Minecraft.Windows.exe");
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			if(BaseAddress) { //to avoid being attached to the runtime broker
				dllInstance = hModule;
				HANDLE tHandle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ZenovaStart, NULL, 0, nullptr);
				if(tHandle) CloseHandle(tHandle);
			}
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			 break;
	}
	return TRUE;
}