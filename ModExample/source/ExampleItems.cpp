#include "ExampleMod/ExampleItems.h"
#include "ExampleMod/ExampleItem.h"

#include "ZenovaCore.h"

#include "Minecraft/ItemRegistry.h"

ClientInstance* ExampleItems::instance;
WeakPtr<Item> ExampleItems::exampleItem;

void ExampleItems::init() {
	ExampleItems::exampleItem = ItemRegistry::registerItem<ExampleItem>("example", 4000 - 256);
	ExampleItems::exampleItem->setAllowOffhand(true);
}

void ExampleItems::initCreativeItems() {
	Item::addCreativeItem(ExampleItems::exampleItem.get(), 0);
}

void ExampleItems::loadResources() {
	ExampleItems::exampleItem->setIcon("example", 0);
}

__declspec(noinline) void ExampleItems::passInstance(ClientInstance* tmp) {
	ExampleItems::instance = tmp;
	DebugBreak();
}