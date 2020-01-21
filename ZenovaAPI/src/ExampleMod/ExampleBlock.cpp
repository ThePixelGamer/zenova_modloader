#include "ExampleMod/ExampleBlock.h"
#include "ZenovaCore.h"

#include <iostream>

ExampleBlock::ExampleBlock(const std::string& name, int id, const Material& mat) : BlockLegacy(name, id, mat) {
	//Replace our vtable with calls to the original vtable, but keep the ones we've overriden
	//CompareAndReplace<BlockLegacy>(*(uintptr_t**)this, (uintptr_t*)SlideAddress(0x2462138));
	setCategory(CreativeItemCategory::ITEMS);
}