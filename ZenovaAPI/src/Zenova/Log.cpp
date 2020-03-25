#include "Zenova/Log.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "Zenova.h"

namespace Zenova {
	template <typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
	constexpr size_t EnumIndex(T value) {
		return static_cast<typename std::underlying_type<T>::type>(value);
	}

	std::vector<std::tstring> severityStrings {{
		"Info", "Warning", "Error"
	}};

	Message::Message(LogSeverity severity, std::tstring message, std::tstring name) : Message(severityStrings.at(EnumIndex(severity) - 1), message, name) {}

	Message::Message(std::tstring severity, std::tstring message, std::tstring name) {
		if(!name.empty()) {
			message.insert(0, L"[" + name + L"] ");
		}

		if(!severity.empty()) {
			message.insert(0, L"[" + severity + L"] ");
		}

		message += L"\n";

		OutputDebugStringW(message.c_str());
		std::wcout << message << std::flush;
	}
}