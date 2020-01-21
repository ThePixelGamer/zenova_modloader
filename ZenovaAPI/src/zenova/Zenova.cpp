#include "Zenova.h"
#include "ZenovaMod.h"

#include <algorithm> //std::find_if

//ZenovaModAPI& Zenova::FindAPI(const char* name) {
//	return *std::find_if(apis.begin(), apis.end(), [name](const ZenovaModAPI& api) -> bool { return api.m_name == name; });
//}

bool Zenova::CreateHook(void* address, void* funcJump, void** funcTrampoline) {
	if(MH_CreateHook(address, funcJump, funcTrampoline) != MH_OK) 
		return false;

	if(MH_EnableHook(address) != MH_OK) 
		return false;

	//MSHookFunction for android

	return true;
}

bool Zenova::CreateHook(std::string funcName, void* funcJump, void** funcTrampoline) {
	//setup a lookup map to grab the address
}