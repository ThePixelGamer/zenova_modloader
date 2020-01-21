#include "bedrock/ItemRegistry.h"

#include "ExampleMod/ExampleItem.h"

#include "ZenovaCore.h"

void ItemRegistry::registerItem(SharedPtr<Item> itemReg) {
	((void (*)(SharedPtr<Item>))SlideAddress(0x115D620))(itemReg);
}

void ItemRegistry::shutdown() {
	((void (*)())SlideAddress(0x115D620))();
}