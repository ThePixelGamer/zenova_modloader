// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "Zenova.h"

#include "ExampleMod/ExampleItems.h"

#define DLL_FUNCTION extern "C" __declspec(dllexport)

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

class ExampleMod : public Zenova::Mod {
public:
	ExampleMod() : Zenova::Mod("Example Mod", {1, 0, 0}, "Description") {}

	virtual void Start() {
		std::cout << "ModExample Start" << std::endl;

		//uintptr_t** vtable = (uintptr_t**)SlideAddress(0x2460D70) + 1; //to avoid calling the virtual deconstructor
		//CreateHook((void*) SlideAddress(0x11848A0), (void*) &registerItems, (void**) &_registerItems); //VanillaItems::registerItems(bool)
		//CreateHook((void*) SlideAddress(0x1193AF0), (void*) &initCreativeItemsCallback, (void**) &_initCreativeItemsCallback); //VanillaItems::initCreativeItemsCallback(ActorInfoRegistry*, BlockDefinitionGroup*, bool)
		//os->CreateHook((void*) SlideAddress(0x118F7F0), (void*) &initClientData, (void**) &_initClientData); //VanillaItems::initClientData()
		//CreateHook((void*) SlideAddress(0x008F060), (void*) &init, (void**) &_init); //ClientInstance::init()
	}

	virtual void Update() {
		std::cout << "ModExample Update" << std::endl;
	}

	virtual void Stop() {
		std::cout << "ModExample Stop" << std::endl;
	}
};

DLL_FUNCTION void* CreateMod() {
	return new ExampleMod();
}

DLL_FUNCTION void DeleteMod(Zenova::Mod* mod) {
	delete mod;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	return TRUE;
}