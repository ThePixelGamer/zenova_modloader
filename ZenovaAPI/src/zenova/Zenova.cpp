#include "Zenova.h"

#include <iostream> //std::cout (Zenova::Console)
#include <algorithm> //std::find_if
#include <fstream>
#include <cstdio>

#include "Zenova/StorageResolver.h"

namespace Zenova {
	//setup these macros with cmake
	#ifdef _WINDOWS
		std::shared_ptr<OS> hostOS = std::make_shared<Windows>();
	#elif Linux
		std::shared_ptr<OS> hostOS = std::make_shared<Linux>();
	#endif

	class MessageRedirection { //currently used to make a console for the app
		FILE* console = nullptr;

	public:
		MessageRedirection() {
			if(!AllocConsole()) {
				return;
			}

			if(!console) {
				//Handle std::cout, std::clog, std::cerr, std::cin
				freopen_s(&console, "CONOUT$", "w", stdout);
				freopen_s(&console, "CONOUT$", "w", stderr);
				freopen_s(&console, "CONIN$", "r", stdin);
				std::cout.clear();
				std::clog.clear();
				std::cerr.clear();
				std::cin.clear();

				//add something for wide stuff? seems to work fine for the time being...
			}
		}
		~MessageRedirection() {
			if(!FreeConsole()) {
				//Show error with GetLastError();
			}

			if(console) {
				fclose(console);
			}
		}
	};
	
#ifdef ZENOVA_API
	std::string ZenovaFolder = "D:/Zenova/";

	void LoadSymbolMaps() {

	}

	std::vector<std::pair<Mod*, HMODULE>> LoadMods(const std::vector<std::string>& mods) {
		std::vector<std::pair<Mod*, HMODULE>> ret;

		for(auto& mod : mods) {
			HMODULE hModule = LoadLibraryA(mod.c_str());
			if(hModule) {
				FARPROC createMod = GetProcAddress(hModule, "CreateMod");

				if(createMod) {
					ret.push_back(std::make_pair(reinterpret_cast<Mod*>(createMod()), hModule));
					continue;
				}
			}

			Log::info("LoadMods", mod.substr(mod.rfind("/") + 1, mod.rfind(".dll")) + " failed to load");
		}
		
		return ret;
	}
	
	struct Profile {
		std::vector<std::string> libraryNames;
	};

	Profile LoadProfile() {
		std::vector<std::string> libNames;
		
		WIN32_FIND_DATAA fileData;
		HANDLE dir;

		if((dir = FindFirstFileA((ZenovaFolder + "/Mods/*").c_str(), &fileData)) != INVALID_HANDLE_VALUE) {
			do {
				const std::string fileName = fileData.cFileName;
				const std::string filePath = ZenovaFolder + "/Mods/" + fileName;

				if((fileName[0] == '.') || ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
					continue;

				if(fileName.rfind(".dll") != fileName.npos) {
					libNames.push_back(filePath);
				}
			} while(FindNextFile(dir, &fileData));
		}

		return Profile{libNames};
	}

	unsigned long __stdcall Start(void* dllHandle) {
		Hook::BaseAddress = Util::GetModuleBaseAddress("Minecraft.Windows.exe");
		MessageRedirection console;
		Log logger("Zenova::Start");

		logger.info("Zenova Started");
		logger.info("Minecraft at: " + Util::to_hex_string(Hook::BaseAddress));
	
		StorageResolver storage(L"minecraftWorlds/", L"D:/MinecraftBedrock/Worlds");
		
		if(hostOS->type == OSType::Windows) {
			hostOS->SetupOS(dllHandle);
		}
		
		Profile profile = LoadProfile();
		std::vector<std::pair<Mod*, HMODULE>> mods = LoadMods(profile.libraryNames);
		//HMODULE module = LoadLibraryA();
		
		for(auto [first, second] : mods) {
			first->Start();
		}

		bool run = true;
		while(run) {
			for(auto [first, second] : mods) {
				first->Update();
			}
		}
		
		for(auto [first, second] : mods) {
			first->Stop();
			FreeLibrary(second);
		}

		logger.info("Zenova Stopped");
		return 0;
	}
#endif // ZENOVA_API
}