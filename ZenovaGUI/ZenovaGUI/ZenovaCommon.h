#pragma once

#include "../../Utils/AppUtils.h"
#include "../../Utils/ProcessUtils.h"
#include "../../Utils/utils.h"

#include <iostream>
#include <sstream>
#include <string>

#include "ZenovaDebugger.h"
#include "ZenovaLogger.h"


namespace ZenovaCommon {
	extern ZenovaDebugger dbg;
	extern ZenovaLogger logger;
	extern std::string currentState;
	extern bool done;
};

//ZenovaCommon
void StateChangeCallbackFunc(PACKAGE_EXECUTION_STATE);

//main
void LaunchMinecraft(bool);