#pragma once

#include "ExampleItem.h"

class ExampleItemManager {
	static ExampleItem* exampleItem;

public:
	static void init();
	static void initCreativeItems();
	static void loadResources();
};