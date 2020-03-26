#pragma once

#include "../../Utils/AppUtils.h"
#include "../../Utils/ProcessUtils.h"
#include "../../Utils/utils.h"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

namespace Zenova {
	extern std::string currentState;
	extern bool done;

	class Launcher {
	public:
		AppUtils::AppDebugger* app; //avoid a smart pointer to avoid COM library issues
		DWORD dwProcessId = 0;

		Launcher();
		~Launcher();
		
		void Start(bool);

		static void StateChangeCallbackFunc(PACKAGE_EXECUTION_STATE);
	};
};

//ZenovaCommon


//main