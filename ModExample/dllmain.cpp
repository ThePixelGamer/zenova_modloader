// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "Zenova.h"

#define DLL_API __declspec(dllexport)

class ExampleMod : public ZenovaMod {
public:
	ExampleMod() : ZenovaMod("Example Mod", {1, 0, 0}, "Description") {}

	virtual void Start() {
		std::cout << "ModExample Start" << std::endl;
	}

	virtual void Update() {
		std::cout << "ModExample Update" << std::endl;
	}

	virtual void Stop() {
		std::cout << "ModExample Stop" << std::endl;
	}
};

extern "C" {
	DLL_API ZenovaMod* CreateMod() {
		return new ExampleMod;
	}

	DLL_API void DeleteMod(ZenovaMod* mod) {
		delete mod;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	if(ul_reason_for_call == DLL_PROCESS_ATTACH) {
		OutputDebugStringA("Hi.\n");

		if(GetModuleBaseAddress("Minecraft.Windows.exe")) { //to avoid being attached to the runtime broker
			HANDLE tHandle = CreateThread(nullptr, 0, Zenova::Start, new HMODULE(hModule), 0, nullptr);
			if(tHandle) 
				CloseHandle(tHandle);
		}
	}
	return TRUE;
}