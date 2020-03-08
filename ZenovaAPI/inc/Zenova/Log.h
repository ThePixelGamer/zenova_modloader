#pragma once

#include <string>

#include "Common.h"

namespace Zenova {
	class Log {
		std::string name;

	public:
		Log(const std::string& name);

		void info(const std::string& message);
		void info(const std::wstring& message);
		static void info(const std::string& name, const std::string& message);
		static void info(const std::wstring& name, const std::wstring& message);
	};
}