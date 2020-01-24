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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	if(ul_reason_for_call == DLL_PROCESS_ATTACH) {
		if(Zenova::Util::GetModuleBaseAddress("Minecraft.Windows.exe")) { //to avoid being attached to the runtime broker
			HANDLE tHandle = CreateThread(nullptr, 0, Zenova::Start, new HMODULE(hModule), 0, nullptr);
			if(tHandle) 
				CloseHandle(tHandle);
		}
	}
	return TRUE;
}