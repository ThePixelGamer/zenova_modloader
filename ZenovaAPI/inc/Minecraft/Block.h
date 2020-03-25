#pragma once

#include "Zenova/Hook.h"

#include <cstdint>

#include "Ptr.h"

struct BlockID {
    uint_fast8_t id;
};

class BlockLegacy;
enum class BlockRenderLayer;

class Block {
public:
	//vtable 0x0 : 0
	void* unk; //0x8 : 8
	WeakPtr<BlockLegacy> legacyBlock; //0x10 : 16

	virtual ~Block();
	virtual BlockRenderLayer getRenderlayer() const;
};