#include "Minecraft/ItemRegistry.h"

#include "Minecraft/Item.h"

void ItemRegistry::registerItem(SharedPtr<Item> itemReg) {
	Zenova::Hook::Call<void>(FSIG, itemReg);
}

void ItemRegistry::shutdown() {
	Zenova::Hook::Call<void>(FSIG, shutdown);
}