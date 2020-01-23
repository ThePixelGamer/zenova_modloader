#pragma once

//MSVC uses WIN32, if it's something else in other compilers let me know or make a PR
#ifdef WIN32
	#include <Windows.h>
	#include <Shlobj.h>
#elif
	using BOOL = bool;
	using DWORD = unsigned long;
	using HANDLE = void*;
	using HMODULE = void;
	using LPCWSTR = const wchar_t*;
	using LPVOID = void*;
	using LPCSTR = const char*;
	using LPCWSTR = const wchar_t*;
	
	//for Window's Create "File" functions
	typedef struct _SECURITY_ATTRIBUTES {
		DWORD nLength;
		LPVOID lpSecurityDescriptor;
		BOOL bInheritHandle;
	} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
#endif

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <exception>

#include "MinHook.h"
#include <unordered_map>

struct Version {
	uint8_t major, minor, patch;
};

class ZenovaMod {
	std::string name, description;
	Version version;

protected:
	ZenovaMod(std::string name, Version version, std::string description) : name(name), version(version), description(description) {}
	
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Stop() = 0;
};

struct Symbol {
	Symbol* prev = nullptr;
	Symbol* next = nullptr;
};

//unsigned int types
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

//signed int types
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;


namespace Zenova {
	using exception = std::runtime_error;
	
	class StorageResolver {
	public:
		std::string_view minecraft_path_str;
		std::wstring_view minecraft_path_wstr;
		std::unordered_map<std::wstring, std::wstring> mirror_directory;

		StorageResolver();
		StorageResolver(const std::wstring& directory, const std::wstring& mirror);
		StorageResolver(const std::vector<std::wstring>& directories, const std::vector<std::wstring>& mirrors);

		//false on already exists, true on success
		bool addMirrorDirectory(const std::wstring& directory, const std::wstring& mirror);
		bool addMirrorDirectory(const std::vector<std::wstring>& directories, const std::vector<std::wstring>& mirrors);


	};

	enum class OSType {
		Windows,
		Linux,
		Unknown
	};

	class OS {
		OS() = delete;

	protected:
		OS(OSType type);

	public:
		OSType type;

		virtual void* FindAddress(const std::string& function);
		virtual void* FindAddress(const std::string& module, const std::string& function) = 0;
		virtual bool CreateHook(void* address, void* funcJump, void** funcTrampoline) = 0;
		virtual bool CreateHook(const std::string& module, const std::string& function, void* funcJump, void** funcTrampoline);
		virtual void CreateFileA(const std::string& path);
		//createdir

		static StorageResolver storage;
	};

	class Windows : public OS {
		std::shared_ptr<void> dllHandle;
		bool canHook;

	public:
		Windows(void* dll);

		~Windows();
		virtual void* FindAddress(const std::string& module, const std::string& function);
		virtual bool CreateHook(void* address, void* funcJump, void** funcTrampoline);

		static HANDLE __stdcall CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
		static HANDLE __stdcall CreateDirectoryW(LPCWSTR lpFileName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
		static HANDLE __stdcall CreateDirectoryA(LPCSTR lpFileName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

	};

	class Linux : public OS {
	public:
		Linux();
		
		~Linux();
		virtual void* FindAddress(const std::string& module, const std::string& function);
		virtual bool CreateHook(void* address, void* funcJump, void** funcTrampoline);
	};



	class Console {
		FILE* console = nullptr;

	public:
		Console();
		~Console();
	};

	

	namespace Util {
		template<typename T>
		std::string to_hex_string(T num) {
			std::stringstream stream;
			stream << "0x" << std::setfill('0') << std::setw(sizeof(T)*2) << std::hex << num;
			return stream.str();
		}
	}

	namespace WinAPI {
		std::string getAccessRightString(const DWORD& dwDesiredAccess);
		std::string getShareRightString(const DWORD& dwShareMode);
		std::string getCreationDispositionString(const DWORD& dwCreationDisposition);
		std::string getFlagsAndAttributesString(const DWORD& dwFlagsAndAttributes);
	}
}