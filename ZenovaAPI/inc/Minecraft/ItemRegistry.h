#pragma once

#include "Zenova/Hook.h"

#include <vector>
#include <string>

#include "Minecraft/Ptr.h"

class Item;

class ItemRegistry {
public:
	template<typename T, typename... Args>
	static WeakPtr<T> registerItem(const std::string& name, short id, Args... args) {
		return ItemRegistry::registerItemShared<T>(name, id + 256, args...);
	}

	template<typename T, typename... Args>
	static WeakPtr<T> registerItemShared(Args... args) {
		SharedPtr<Item> itemReg = SharedPtr<T>::make(args...);
		ItemRegistry::registerItem(itemReg);
		WeakPtr<T> item(itemReg);
		return item;
	}

	static void registerItem(SharedPtr<Item>);
	static void shutdown();
	
	static std::vector<SharedPtr<Item>> mItemRegistry;
};