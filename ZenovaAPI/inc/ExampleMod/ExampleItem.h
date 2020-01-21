#pragma once

#include <string>

#include "../inc/bedrock/Item.h"

class ExampleItem : public Item {
public:
	ExampleItem(const std::string&, short);
	virtual bool _useOn(ItemStack&, Actor&, BlockPos, unsigned char, float, float, float) const;

	void placeHouse(BlockSource*, const BlockPos& pos) const;
};