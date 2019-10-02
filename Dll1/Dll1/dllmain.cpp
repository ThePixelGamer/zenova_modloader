#include "Hook/Zenova.h"

#include "mcpe/Block.h"
#include "mcpe/BlockLegacy.h"
#include "mcpe/Item.h"

//#include "src/ExampleItemManager.h"

DWORD error = 0;
DWORD error1 = 0;

std::unordered_set<std::string> dataSent;
HGLOBAL hMem;

static bool loaded = false;
static int count = 0;

void (*_initCreativeItems)();
void initCreativeItems() {
	_initCreativeItems();

	if(!loaded) {
		//ExampleItemManager::init();
		loaded = true;
	}

	//ExampleItemManager::initCreativeItems();
}

void (*_initClientData)();
void initClientData() {
	_initClientData();

	//ExampleItemManager::loadResources();
}

unsigned long long qwq = 0x123456789101112;

bool blockObstructsChests() {
	return false;
}

bool removeBlockCollision() {
	return false;
}

uint32_t changeBlockColor() {
	return 0xFF00FF; //purple; 
}

bool start() {
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

	//char result[20];
	//_ui64toa(GetCurrentProcessId(), result, 10);
	//bool failed = sendToLogger(result);

	OutputDebugString("Hello from dll");

	//uintptr_t** vt1 = (uintptr_t**)SlideAddress(0x2460D70) + 1; //BedrockBlock Vtable
	//
	//vt1[3] = (uintptr_t*)& blockObstructsChests;
	//vt1[8] = (uintptr_t*)& removeBlockCollision;
	//vt1[117] = (uintptr_t*)& changeBlockColor;
	//vt1[118] = (uintptr_t*)& changeBlockColor;
	//vt1[119] = (uintptr_t*)& changeBlockColor;

	CloseHandle(process);
	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	BaseAddress = GetModuleBaseAddress(GetCurrentProcessId(), "Minecraft.Windows.exe");

	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		{
			if(BaseAddress)
				return start();
		} break;
		case DLL_THREAD_ATTACH:
		{
			OutputDebugString(std::string("Hello from dll (DLL_THREAD_ATTACH)" + std::to_string(count)).c_str());
			count++;
		} break;
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			return FALSE;
	}
	return TRUE;
}