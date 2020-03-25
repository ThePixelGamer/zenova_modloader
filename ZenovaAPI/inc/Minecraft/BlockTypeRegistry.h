#pragma once

#include "Zenova/Hook.h"

#include <string>
#include <unordered_map>

#include "Minecraft/Util.h"
#include "Minecraft/Ptr.h"

class BlockLegacy;

class BlockTypeRegistry {
public:
	template<typename T, typename... Args>
	static T& registerBlock(Args&&... args) {
		SharedPtr<T> blockReg = SharedPtr<T>::make(args...);

		(*reinterpret_cast<std::unordered_map<std::string, SharedPtr<BlockLegacy>>*>(
			Zenova::Hook::FindSymbol("global", "mBlockTypeRegistry")))
			[Util::toLower(blockReg->getRawNameId())] = blockReg;

		return *blockReg;
	}
	
	static std::unordered_map<std::string, SharedPtr<BlockLegacy>> mBlockTypeRegistry;
};