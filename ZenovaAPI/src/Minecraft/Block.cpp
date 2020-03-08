#include "Minecraft/Block.h"

Block::~Block() {
	Zenova::Hook::Call<void>(FSIG, this);
}

BlockRenderLayer Block::getRenderlayer() const {
	return Zenova::Hook::Call<BlockRenderLayer>(FSIG, this);
}