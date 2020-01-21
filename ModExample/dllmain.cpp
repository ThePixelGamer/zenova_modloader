// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "inc/Zenova.h"

#define DLL_API __declspec(dllexport)

//Will be in a Zenova Header


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
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}