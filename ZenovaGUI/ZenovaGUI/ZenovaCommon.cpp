#include "ZenovaCommon.h"
#include <array>

namespace Zenova {
	std::string currentState = "Unknown";
	
	Launcher::Launcher() {
		if(SUCCEEDED(CoInitialize(NULL))) {
			app = std::make_shared<AppUtils::AppDebugger>(AppUtils::GetMinecraftPackageId());
			app->setStateChangeCallback(StateChangeCallbackFunc);
			StateChangeCallbackFunc(app->GetPackageExecutionState());
		}
	}

	Launcher::~Launcher() {
		CoUninitialize();
	}

	void Launcher::Start(bool forceRestart) {
		HRESULT hresult = S_OK;

		if(app.get()) {
			if(app->GetPackageExecutionState() != PES_UNKNOWN) {
				if(forceRestart) {
					app->TerminateAllProcesses();
					hresult = app->GetHRESULT();
					if(hresult != S_OK) {
						return;
					}
				}
				else
					return;
			}
			
			std::wstring ModLoaderPath = Util::GetCurrentDirectory();
			if(ModLoaderPath.length() == 0) {
				return;
			}

			ModLoaderPath += L"ZenovaModLoader.exe";
			if(!PathFileExists(ModLoaderPath.c_str())) {
				return;
			}

			app->EnableDebugging(ModLoaderPath);
			hresult = app->GetHRESULT();
			if(hresult != S_OK) {
				return;
			}

			std::wstring ApplicationId = AppUtils::GetMinecraftApplicationId();
			if(ApplicationId.length() == 0) {
				return;// E_FAIL;
			}

			hresult = AppUtils::LaunchApplication(ApplicationId.c_str(), &dwProcessId);
			if(hresult != S_OK) {
				return;
			}

			//could be commented out for a temp perma injection (till the user reboots)
			app->DisableDebugging(); 
			hresult = app->GetHRESULT();
			if(hresult != S_OK) {
				return;
			}
		}

		return;//  S_OK;
	}

	std::array<std::string, 5> states = {
			"Unknown",
			"Running",
			"Suspending",
			"Suspended",
			"Terminated"
	};

	void Launcher::StateChangeCallbackFunc(PACKAGE_EXECUTION_STATE state) {
		currentState = states.at(state);
	}
};