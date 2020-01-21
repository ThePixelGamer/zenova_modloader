#include "ExampleMod/ExampleBlockTypes.h"
#include "ExampleMod/ExampleBlock.h"

#include "ZenovaCore.h"

#include "ExampleMod/ExampleBlock.h"
#include "bedrock/Ptr.h"
#include "bedrock/BlockTypeRegistry.h"

#include <cstdint>

WeakPtr<BlockLegacy> ExampleBlockTypes::exampleBlockType;

namespace {
	void addLegacyBlockIdMapping(BlockLegacy* block, uintmax_t id) { //0x12A4A50
		//call original
	}

	void addLegacyBlockIdMappings() {
		addLegacyBlockIdMapping(ExampleBlockTypes::exampleBlockType.get(), 477);
	}
}

void ExampleBlockTypes::registerBlocks() {
	ExampleBlockTypes::exampleBlockType = BlockTypeRegistry::registerBlock<ExampleBlock>("example", 477, Material::getMaterial(MaterialType::DIRT)).createWeakPtr();
	ExampleBlockTypes::exampleBlockType->setDestroyTime(0.0f);
	ExampleBlockTypes::exampleBlockType->setExplodeable(0.0f);

	addLegacyBlockIdMappings();
}

//void ExampleBlockTypes::initCreativeItems() {
//	Item::addCreativeItem(ExampleBlockTypes::exampleBlockType->get(), 0);
//}
//
//void ExampleBlockTypes::loadResources() {
//	(*ExampleBlockTypes::exampleBlockType)->setIcon("example", 0);
//}