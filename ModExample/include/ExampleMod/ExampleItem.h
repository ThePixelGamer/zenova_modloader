#pragma once

#include <string>

#include "Minecraft/Item.h"
#include "Zenova/Structure.h"

class ExampleItem : public Item, public Zenova::Structure {
public:
	ExampleItem(const std::string&, short);
	virtual bool _useOn(ItemStack&, Actor&, BlockPos, unsigned char, float, float, float) const;

	void placeHouse(BlockSource*, const BlockPos& pos) const;
};