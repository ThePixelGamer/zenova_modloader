//Meant to be used with ZenovaAPI, not for general use

#pragma once

#include <string>
#include <unordered_map>

namespace Zenova {
	class StorageResolver {
	public:
		static std::string minecraft_path_str;
		static std::wstring minecraft_path_wstr;
		static std::unordered_map<std::wstring, std::wstring> mirror_directory;

		StorageResolver();
		StorageResolver(const std::wstring& directory, const std::wstring& mirror);
		StorageResolver(const std::vector<std::wstring>& directories, const std::vector<std::wstring>& mirrors);

		//false on already exists, true on success
		bool addMirrorDirectory(const std::wstring& directory, const std::wstring& mirror);
		bool addMirrorDirectory(const std::vector<std::wstring>& directories, const std::vector<std::wstring>& mirrors);
	};
}