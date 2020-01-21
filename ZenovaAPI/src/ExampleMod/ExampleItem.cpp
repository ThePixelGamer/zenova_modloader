﻿#include "ExampleMod/ExampleItem.h"
#include "ZenovaCore.h"

#include <iostream>
#include <string>
#include <vector>

#include "bedrock/Actor.h"
#include "bedrock/Block.h"
#include "bedrock/BlockLegacy.h"
#include "bedrock/BlockSource.h"

#include "bedrock/Ptr.h"

ExampleItem::ExampleItem(const std::string& name, short id) : Item(name, id) {
	//Replace our vtable with calls to the original vtable, but keep the ones we've overriden
	CompareAndReplace<Item>(*(uintptr_t**)this, (uintptr_t*)SlideAddress(0x244A108));
	setCategory(CreativeItemCategory::TOOLS);
	setMaxStackSize(16);
}

void ExampleItem::placeHouse(BlockSource* source, const BlockPos& pos) const {
	auto set = [&](WeakPtr<BlockLegacy> blockPtr, BlockPos blockpos, unsigned char update) {
		//WeakPtr<BlockLegacy> blockPtr = ((WeakPtr<BlockLegacy>(*)(const std::string&))SlideAddress(0x12A0300))(name); //BlockTypeRegistry::lookupByName
		return ((bool (*)(BlockSource*, int, int, int, const Block&, int))SlideAddress(0x13338F0))(source, blockpos.x, blockpos.y, blockpos.z, *blockPtr->block, update); //1 updateable, 2 let renderer know a block's changed, needs more research
	};

	auto get = [&](const std::vector<WeakPtr<BlockLegacy>>& blockPtr, const BlockPos& blockpos) {
		for(int i = 0; i < blockPtr.size(); i++) {
			if((blockPtr[i]->blockID == (((const Block & (*)(const BlockSource*, const BlockPos&))SlideAddress(0x1333650))(source, blockpos)).legacyBlock->blockID))
				return true;
		}
		return false;
	};
	
	WeakPtr<BlockLegacy> gold = ((WeakPtr<BlockLegacy>(*)(const std::string&))SlideAddress(0x12A0300))("gold_block");
	WeakPtr<BlockLegacy> air = ((WeakPtr<BlockLegacy>(*)(const std::string&))SlideAddress(0x12A0300))("air");
	WeakPtr<BlockLegacy> bedrock = ((WeakPtr<BlockLegacy>(*)(const std::string&))SlideAddress(0x12A0300))("bedrock");
	
	unsigned char update = 0;
	for(int x = pos.x - 3, endx = pos.x + 3; x <= endx; x++) {
		for(int y = pos.y - 3, endy = pos.y + 3; y <= endy; y++) {
			for(int z = pos.z - 3, endz = pos.z + 3; z <= endz; z++) {
				BlockPos current {x, y, z};
				if(!get(std::vector<WeakPtr<BlockLegacy>>{air, gold, bedrock}, current)) {
					if((x == pos.x - 3) || (x == endx) || (y == pos.y - 3) || (y == endy) || (z == pos.z - 3) || (z == endz)) {
						update = 2;
					}
					set(gold, current, update);
					update = 0;
				}
			}
		}
	}
}

bool ExampleItem::_useOn(ItemStack& item, Actor& actor, BlockPos blockpos, unsigned char side, float click_x, float click_y, float click_z) const { //click xyz is where on the block the use was clicked
	short id = (((const Block & (*)(const BlockSource*, const BlockPos&))SlideAddress(0x1333650))(actor.blocksource, blockpos)).legacyBlock.get()->blockID;
	if((id != 7) && (id != 41)) { //can also use the SlideAddress function in "auto set =" to use the block names just remove the ".get()->blockID" above
		placeHouse(actor.blocksource, blockpos);
		return true;
	}

	return false;
	//return Item::_useOn(item, actor, blockpos, side, click_x, click_y, click_z);
}