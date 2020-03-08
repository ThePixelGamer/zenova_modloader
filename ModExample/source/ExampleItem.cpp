#include "ExampleMod/ExampleItem.h"
#include "Zenova/Hook.h"

#include <iostream>
#include <string>
#include <vector>

#include "Minecraft/Actor.h"
#include "Minecraft/Block.h"
#include "Minecraft/BlockLegacy.h"
#include "Minecraft/BlockSource.h"

#include "Minecraft/Ptr.h"

ExampleItem::ExampleItem(const std::string& name, short id) : Item(name, id), Zenova::Structure("vItem") {
	//Replace our vtable with calls to the original vtable, but keep the ones we've overriden
	Zenova::Hook::RepairVtable<Item>(reinterpret_cast<uintptr_t*>(this), reinterpret_cast<uintptr_t*>(Zenova::Hook::Symbols["vItem"]));
	setCategory(CreativeItemCategory::TOOLS);
	setMaxStackSize(16);
}

//fix this by moving all the SlideAddress calls into their proper classes
void ExampleItem::placeHouse(BlockSource* source, const BlockPos& pos) const {
	auto set = [&](WeakPtr<BlockLegacy> blockPtr, BlockPos blockpos, unsigned char update) {
		//WeakPtr<BlockLegacy> blockPtr = ((WeakPtr<BlockLegacy>(*)(const std::string&))SlideAddress(0x12A0300))(name); //BlockTypeRegistry::lookupByName
		return Zenova::Hook::Call<bool>(Zenova::Hook::SlideAddress(0x13338F0), source, blockpos.x, blockpos.y, blockpos.z, *blockPtr->block, update); //1 updateable, 2 let renderer know a block's changed, needs more research
	};

	auto get = [&](const std::vector<WeakPtr<BlockLegacy>>& blockPtr, const BlockPos& blockpos) {
		for(size_t i = 0; i < blockPtr.size(); i++) {
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
					else {
						update = 0;
					}

					set(gold, current, update);
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