#include "ZenovaCore.h"
#include "ZenovaHooks.h"

//Windows
#include <TlHelp32.h>
#include <atlbase.h>
#include <direct.h>

//C
#include <cstdio>
#include <cstdint>

//C++
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include <fstream>
#include <algorithm>

#include "ExampleMod/ExampleItems.h"

extern uint8_t* BaseAddress;

void (*_registerItems)(bool);
void (*_initCreativeItemsCallback)(class ActorInfoRegistry*, class BlockDefinitionGroup*, bool);
void (*_initClientData)();

void registerItems(bool b1) {
	_registerItems(b1);

	ExampleItems::init();
}

void initCreativeItemsCallback(class ActorInfoRegistry* actorInfoRegistry, class BlockDefinitionGroup* blockDefinitionGroup, bool b1) {
	_initCreativeItemsCallback(actorInfoRegistry, blockDefinitionGroup, b1);

	ExampleItems::initCreativeItems();
}

void initClientData() {
	_initClientData();

	ExampleItems::loadResources();
}

//namespace Core {
//	class FilePathManager;
//}
//
//namespace Social {
//	class IUserManager;
//}
//
//void (*_init)(class ClientInstance*, Core::FilePathManager*, class NetworkHandler&, class Timer&, class Timer&, Social::IUserManager&, int);
//void init(class ClientInstance* ci_this, Core::FilePathManager* core_FilePathManager, class NetworkHandler& networkHandler, class Timer& timer1, class Timer& timer2, Social::IUserManager& social_IUserManager, int i1) {
//	_init(ci_this, core_FilePathManager, networkHandler, timer1, timer2, social_IUserManager, i1);
//
//	ExampleItemManager::passInstance(ci_this);
//}

//Basically all the bare minimum hooks to get this thing running
void setupHooks(const std::shared_ptr<Zenova::OS>& os) {
	switch(os->type) {
		case Zenova::OSType::Windows: {
			os->CreateHook("KernelBase.dll", "CreateFileW", (void*) &Zenova::Windows::CreateFileW, (void**) &pfnCreateFileW);
			os->CreateHook("KernelBase.dll", "CreateDirectoryW", (void*) &Zenova::Windows::CreateDirectoryW, (void**) &pfnCreateDirectoryW);
			os->CreateHook("KernelBase.dll", "CreateDirectoryA", (void*) &Zenova::Windows::CreateDirectoryA, (void**) &pfnCreateDirectoryA);
		} break;
	}
	//uintptr_t** vtable = (uintptr_t**)SlideAddress(0x2460D70) + 1; //to avoid calling the virtual deconstructor
	//CreateHook((void*) SlideAddress(0x11848A0), (void*) &registerItems, (void**) &_registerItems); //VanillaItems::registerItems(bool)
	//CreateHook((void*) SlideAddress(0x1193AF0), (void*) &initCreativeItemsCallback, (void**) &_initCreativeItemsCallback); //VanillaItems::initCreativeItemsCallback(ActorInfoRegistry*, BlockDefinitionGroup*, bool)
	//os->CreateHook((void*) SlideAddress(0x118F7F0), (void*) &initClientData, (void**) &_initClientData); //VanillaItems::initClientData()
	//CreateHook((void*) SlideAddress(0x008F060), (void*) &init, (void**) &_init); //ClientInstance::init()
}


DWORD __stdcall ZenovaStart(void* dllHandle) {
	Zenova::Console console;

	console.Print("ZenovaAPI", "Zenova Started\n");
	console.Print("ZenovaAPI", "Minecraft at: " + Zenova::Util::to_hex_string(BaseAddress) + "\n");
	
	//determine the os we're running on
#ifdef WIN32
	std::shared_ptr<Zenova::OS> os = std::make_shared<Zenova::Windows>(dllHandle);
#elif __ANDROID__
	std::shared_ptr<Zenova::OS> os = std::make_shared<Zenova::Linux>();
#endif
	
	Zenova::StorageResolver storage(L"minecraftWorlds/", L"D:/MinecraftBedrock/Worlds");

	setupHooks(os);

	//HMODULE module = LoadLibraryA();

	bool run = true;
	while(run) {} //maybe do something with this? /shrug

	console.Print("ZenovaAPI", "Zenova Stopped\n");
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	HMODULE* dllHandle = new HMODULE(hModule);

	BaseAddress = GetModuleBaseAddress("Minecraft.Windows.exe");
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			if(BaseAddress) { //to avoid being attached to the runtime broker
				HANDLE tHandle = CreateThread(nullptr, 0, ZenovaStart, dllHandle, 0, nullptr);
				if(tHandle) CloseHandle(tHandle);
			}
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			 break;
	}
	return TRUE;
}