#include "Zenova/StorageResolver.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlobj.h>
#include <comdef.h>

#include <iostream>
#include <array>

#include "Zenova/Log.h"

namespace Zenova {
	std::string StorageResolver::minecraft_path_str;
	std::wstring StorageResolver::minecraft_path_wstr;
	std::unordered_map<std::wstring, std::wstring> StorageResolver::mirror_directory;

	StorageResolver::StorageResolver() {
		//will have to change this later to meet 1607 expectations of really long filepaths
		std::array<wchar_t, 1000> szPathW;
		HRESULT code = SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPathW.data());
		if(SUCCEEDED(code)) {
			// Get the path to the textures folder
			std::wstring_view szPathView(szPathW.data());
			std::wcout << szPathView << std::endl;
			std::wstring appData(szPathView.substr(0, szPathView.rfind(L"AC")));
			appData += L"LocalState/games/com.mojang/";

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
			Log::info(L"StorageResolver::PathW", minecraft_path_wstr);
		}
		else {
			_com_error err(code);
			LPCTSTR errMsg = err.ErrorMessage();
			std::cout << errMsg << std::endl;
		}
		
		std::array<char, MAX_PATH> szPathA;
		code = SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPathA.data());
		if(SUCCEEDED(code)) {
			// Get the path to the textures folder
			std::string_view szPathView(szPathA.data());
			std::string appData(szPathView.substr(0, szPathView.rfind("AC")));
			appData += "LocalState/games/com.mojang/";

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
			Log::info("StorageResolver::PathA", minecraft_path_str);
		}
		else {
			_com_error err(code);
			LPCTSTR errMsg = err.ErrorMessage();
			std::cout << errMsg << std::endl;
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
}