#include <stdint.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include "Hook/Hook.h"

#include "mcpe/Block.h"
#include "mcpe/BlockLegacy.h"
#include "mcpe/Item.h"

DWORD error = 0;
DWORD error1 = 0;

std::unordered_set<std::string> dataSent;
HGLOBAL hMem;

void sendLogMessage(std::string name, std::string send, bool onetime = true) {
	std::string sData("[ZenovaLogger] " + name + ": " + send); //first is for the filter, 2nd is to identify your message from others, 3rd is the shit you wanna send
	
	if(onetime) {
		if(dataSent.empty() && (dataSent.count(sData) != 0))
			return;
	}
	
	dataSent.emplace(sData);

	if(!hMem)
		hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (sData.length() + 1) * sizeof(CHAR)); 
	if(hMem != NULL) {
		HGLOBAL pMem = GlobalLock(hMem);
		if(pMem != NULL) {
			CopyMemory(pMem, sData.c_str(), sData.length() * sizeof(CHAR));
			GlobalUnlock(hMem);
			OpenClipboard(FindWindowEx(NULL, NULL, "Zenova", NULL));
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
		}
	}
}

uintptr_t** vt1 = (uintptr_t**)0x12345678;

bool blockObstructsChests();
Brightness (*_GetLightEmission)(uintptr_t*, const Block&);
Brightness GetLightEmission(uintptr_t* self, const Block& a2) {
	//sendLogMessage("DLL1", "MyGetLightEmission triggered", false);

	//we get here and I can do custom things tho what I mostly do crashes

	//const_cast<Block&>(a2).light = ((BlockLegacy*)self)->light;
	const_cast<Block&>(a2).light = 0;

	return a2;// (_GetLightEmission)((uintptr_t*)self, a2);
}

bool blockObstructsChests() {
	return false;
}

bool removeBlockCollision() {
	return false;
}

uint32_t changeBlockColor() {
	return 0xFF00FF; //purple; 
}

void initItems() {

}

bool (*_isInsideBorderBlock)(float);
bool isInsideBorderBlock(float a2) {
	return false;
}

bool isInWall() {
	return false;
}

bool start() {
	vt1 = (uintptr_t**)SlideAddress(0x2460D70) + 1; //BedrockBlock Vtable

	vt1[3] = (uintptr_t*)&blockObstructsChests;
	vt1[8] = (uintptr_t*)&removeBlockCollision;
	vt1[117] = (uintptr_t*)&changeBlockColor;
	vt1[118] = (uintptr_t*)&changeBlockColor;
	vt1[119] = (uintptr_t*)&changeBlockColor;

	//vt1 = (uintptr_t**)SlideAddress(0x242D7E0) + 1;
	//vt1[45] = (uintptr_t*)&isInsideBorderBlock;

	Hook::safeHook((void*)SlideAddress(0x11EC710), (uintptr_t)GetLightEmission, (void**)&_GetLightEmission, 12);
	//Hook::safeHook(0x0EA39E0, (uintptr_t)isInsideBorderBlock, (uintptr_t*)&_isInsideBorderBlock, 12);

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	BaseAddress = GetModuleBaseAddress(GetCurrentProcessId(), "Minecraft.Windows.exe");
	
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			return start();
		} break;
		case DLL_THREAD_ATTACH: {
			//std::ostringstream o;
			//o << print1 << print2 << print3 << " " << std::hex << (uint64_t*)&GetLightEmission << " " << SlideAddress(0x11EC710);
			//sendLogMessage("DLL1", o.str());
		}
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			return FALSE;
	}
	sendLogMessage("DLL1", "pwp");
	return TRUE;
}