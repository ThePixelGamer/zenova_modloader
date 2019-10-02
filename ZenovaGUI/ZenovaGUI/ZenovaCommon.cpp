#include "ZenovaCommon.h"

namespace ZenovaCommon {
	ZenovaDebugger dbg;
	ZenovaLogger logger;
	std::string currentState = "Unknown";
	bool done = false;
};

void StateChangeCallbackFunc(PACKAGE_EXECUTION_STATE state) {
	switch(state) {
		case PACKAGE_EXECUTION_STATE::PES_UNKNOWN:
			ZenovaCommon::currentState = "Unknown";
			break;
		case PACKAGE_EXECUTION_STATE::PES_RUNNING:
			ZenovaCommon::currentState = "Running";
			break;
		case PACKAGE_EXECUTION_STATE::PES_SUSPENDING:
			ZenovaCommon::currentState = "Suspending";
			break;
		case PACKAGE_EXECUTION_STATE::PES_SUSPENDED:
			ZenovaCommon::currentState = "Suspended";
			break;
		case PACKAGE_EXECUTION_STATE::PES_TERMINATED:
			ZenovaCommon::currentState = "Terminated";
			break;
		default:
			ZenovaCommon::currentState = "Invalid State";
			break;
	}
}

