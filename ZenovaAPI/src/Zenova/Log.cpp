#include "Zenova/Log.h"

#include <iostream>
#include <sstream>

#include "Zenova/OS/Windows.h"

namespace Zenova {
	Log::Log(const std::string& t_name) : name(t_name) {}

	void Log::info(const std::string& message) {
		Log::info(name, message);
	}

	void Log::info(const std::wstring& message) {
		std::wstringstream str2wstr;
		str2wstr << name.c_str();
		Log::info(str2wstr.str(), message);
	}

	void Log::info(const std::string& name, const std::string& message) {
		std::string str("[" + name + "] " + message + "\n");

		OutputDebugStringA(str.c_str());
		std::cout << str << std::flush;
	}

	void Log::info(const std::wstring& name, const std::wstring& message) {
		std::wstring str(L"[" + name + L"] " + message + L"\n");

		OutputDebugStringW(str.c_str());
		std::wcout << str << std::flush;
	}
}