#include "Zenova.h"
#include "ZenovaMod.h"
#include "ZenovaHooks.h"

#include <algorithm> //std::find_if

//ZenovaModAPI& FindAPI(const char* name) {
//	return *std::find_if(apis.begin(), apis.end(), [name](const ZenovaModAPI& api) -> bool { return api.m_name == name; });
//}

namespace Zenova {
	Console::Console() {
		if(!AllocConsole()) {
			return;
		}

		if(!console) {
			//Handle std::cout, std::clog, std::cerr, std::cin
			freopen_s(&console, "CONOUT$", "w", stdout);
			freopen_s(&console, "CONOUT$", "w", stderr);
			freopen_s(&console, "CONIN$", "r", stdin);
			std::cout.clear();
			std::clog.clear();
			std::cerr.clear();
			std::cin.clear();

			//add something for wide stuff? seems to work fine for the time being...
		}
	}

	Console::~Console() {
		if(!FreeConsole()) {
			//Show error with GetLastError();
		}

		if(console) {
			fclose(console);
		}
	}

	StorageResolver::StorageResolver() {
		std::wstring szPathW;
		if(SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPathW.data()))) {
			// Get the path to the textures folder
			std::wstring appData(szPathW.substr(0, szPathW.rfind(L"AC")) + L"LocalState/games/com.mojang/");

			//Change all the trailing backslashes into forward slash
			std::wstring_view replaced = L"\\";
			for(size_t start = 0, pos = 0, end = appData.find(L"LocalState"); start != end; start = pos + replaced.length()) {
				pos = appData.find(replaced, start);
				appData.replace(pos, replaced.length(), L"/");
			}

			//SHGetFolderPathW doesn't properly captialize this, use lowercase in the future?
			replaced = L"microsoft.minecraftuwp";
			appData.replace(appData.find(replaced), replaced.length(), L"Microsoft.MinecraftUWP");

			minecraft_path_wstr = appData;
		}

		std::string szPath;
		if(SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath.data()))) {
			// Get the path to the textures folder
			std::string appData(szPath.substr(0, szPath.rfind("AC")) + "LocalState/games/com.mojang/");

			//Change all the trailing backslashes into forward slash
			std::string_view replaced = "\\";
			for(size_t start = 0, pos = 0, end = appData.find("LocalState"); start != end; start = pos + replaced.length()) {
				pos = appData.find(replaced, start);
				appData.replace(pos, replaced.length(), "/");
			}

			//SHGetFolderPathW doesn't properly captialize this, use lowercase in the future?
			replaced = "microsoft.minecraftuwp";
			appData.replace(appData.find(replaced), replaced.length(), "Microsoft.MinecraftUWP");

			minecraft_path_str = appData;
		}
	}

	StorageResolver::StorageResolver(const std::wstring& directory, const std::wstring& mirror) : StorageResolver() {
		addMirrorDirectory(directory, mirror);
	}

	StorageResolver::StorageResolver(const std::vector<std::wstring>& directories, const std::vector<std::wstring>& mirrors) : StorageResolver() {
		addMirrorDirectory(directories, mirrors);
	}

	bool StorageResolver::addMirrorDirectory(const std::wstring& directory, const std::wstring& mirror) {


		return true;
	}

	bool StorageResolver::addMirrorDirectory(const std::vector<std::wstring>& directories, const std::vector<std::wstring>& mirrors) {
		
		return true;
	}

	OS::OS(OSType os) : type(os) {}

	void* OS::FindAddress(const std::string& function) {
		//todo: setup a dictionary for file input

		return nullptr;
	}

	bool OS::CreateHook(const std::string& module, const std::string& function, void* funcJump, void** funcTrampoline) {
		return CreateHook(FindAddress(module, function), funcJump, funcTrampoline);
	}

	void OS::CreateFileA(const std::string& path) {

	}

	//void* OS::FindAddress(const std::string& module, const std::string& function) {
	//	throw exception("Unitialized OS");
	//}
	//
	//bool OS::CreateHook(void* address, void* funcJump, void** funcTrampoline) {
	//	throw exception("Unitialized OS");
	//}

	Windows::Windows(void* dll) : OS(OSType::Windows), canHook(true), dllHandle(dll) {
		if(MH_Initialize() != MH_OK) {
			throw exception("MinHook failed to initialize, normal launch without hooks");
			canHook = false;
		}
	}

	Windows::~Windows() {
		FreeLibraryAndExitThread(*reinterpret_cast<HMODULE*>(dllHandle.get()), 0);
	}

	void* Windows::FindAddress(const std::string& module, const std::string& function) {
		HMODULE hModule = GetModuleHandleA(module.c_str());
		if(hModule == NULL)
			return 0;

		FARPROC hFunction = GetProcAddress(hModule, function.c_str());
		if(hFunction == NULL)
			return 0;

		return hFunction;
	}

	bool Windows::CreateHook(void* address, void* funcJump, void** funcTrampoline) {
		if(canHook) {
			if(MH_CreateHook(address, funcJump, funcTrampoline) != MH_OK)
				return false;

			if(MH_EnableHook(address) != MH_OK)
				return false;

			return true;
		}
		
		return false;
	}

	/*
	bool FileExists(LPCWSTR fileName) {
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(fileName) && GetLastError() == ERROR_FILE_NOT_FOUND) return false;
		return true;
	}
	*/
	
	HANDLE Windows::CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
		//Can't redirect File Creates to folders the UWP app doesn't have access to
		std::wstring_view filePath(lpFileName);
	
		// Check if it's accessing resources; this method should work in all future updates
		if(filePath.find(storage.minecraft_path_wstr) != filePath.npos) {
			std::wcout << "File: " << filePath << "\n";
			std::cout << "{ " << WinAPI::getAccessRightString(dwDesiredAccess) << ", " 
				<< WinAPI::getShareRightString(dwShareMode) <<  ", "
				<< WinAPI::getCreationDispositionString(dwCreationDisposition) << ", " 
				<< WinAPI::getFlagsAndAttributesString(dwFlagsAndAttributes) << " }" << "\n\n" << std::flush;
	
			std::wstring newPath(L"D:/minecraftWorlds/");
			std::wstring_view strToFind(L"/minecraftWorlds/");
			newPath += filePath.substr(filePath.find(strToFind) + strToFind.size(), filePath.size());

			// Check if a file exists at that path and reroute access to that file
			//if(FileExists(newPath.c_str()))
				//return pfnCreateFileW(newPath.c_str(), dwAccess, dwSharing, saAttributes, dwCreation, dwAttributes, hTemplate);
		}

		return pfnCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	HANDLE Windows::CreateDirectoryW(LPCWSTR lpFilename, LPSECURITY_ATTRIBUTES saAttributes) {
		//Can't redirect Directory Creates to folders the UWP app doesn't have access to
		std::wstring_view filePath(lpFilename);
		std::wcout << filePath << std::endl;

		// Check if it's accessing resources; this method should work in all future updates
		if(filePath.find(storage.minecraft_path_wstr) != filePath.npos) {
			std::wstring newPath(L"D:/minecraftWorlds/");
			std::wstring newDir(newPath);
			std::wstring_view strToFind(L"/minecraftWorlds/");
			size_t startPoint = filePath.find(strToFind) + strToFind.size();
			newPath += filePath.substr(startPoint);
		
			size_t endPoint = filePath.find(L"/", startPoint) - startPoint;
			newDir += filePath.substr(startPoint, endPoint);

			// Check if a file exists at that path and reroute access to that file
			//if(FileExists(newPath.c_str()))
				//return pfnCreateDirectoryW(newPath.c_str(), saAttributes);
		}

		return pfnCreateDirectoryW(lpFilename, saAttributes);
	}

	HANDLE Windows::CreateDirectoryA(LPCSTR lpFilename, LPSECURITY_ATTRIBUTES saAttributes) {
		//Can't redirect Directory Creates to folders the UWP app doesn't have access to
		std::string_view filePath(lpFilename);
		std::cout << filePath << std::endl;

		// Check if it's accessing resources; this method should work in all future updates
		if(filePath.find(storage.minecraft_path_str) != filePath.npos) {
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

	Linux::Linux() : OS(OSType::Linux) {}

	void* Linux::FindAddress(const std::string& module, const std::string& function) {

		return nullptr;
	}
	
	bool Linux::CreateHook(void* address, void* funcJump, void** funcTrampoline) {
		//MSHookFunction(address, funcJump, funcTrampoline);

		return true;
	}

	namespace WinAPI {
		std::string getAccessRightString(const DWORD& dwDesiredAccess) {
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

		std::string getShareRightString(const DWORD& dwShareMode) {
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

		std::string getCreationDispositionString(const DWORD& dwCreationDisposition) {
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

		std::string getFlagsAndAttributesString(const DWORD& dwFlagsAndAttributes) {
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
	}
}