#include "ExampleItemManager.h"
#include "ExampleItem.h"

ExampleItem* ExampleItemManager::exampleItem;

void ExampleItemManager::init() {
	//exampleItem = registerItemShared<ExampleItem>("exampleItem", 439 - 256); // registerItemShared callFunc(0x1160A00‬);
	exampleItem->setCategory(CreativeItemCategory::TOOLS);
}

void ExampleItemManager::initCreativeItems() {
	Item::addCreativeItem(exampleItem, 0); //addCreativeItem callFunc(0x114C7A0)
}

void ExampleItemManager::loadResources() {}