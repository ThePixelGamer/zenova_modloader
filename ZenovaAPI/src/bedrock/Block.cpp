#include "../inc/bedrock/Block.h"
#include "ZenovaCore.h"

Block::~Block() {
	((void (*)(Block*))SlideAddress(0x123F350))(this);
}

BlockRenderLayer Block::getRenderlayer() const {
	return ((BlockRenderLayer (*)(const Block*))SlideAddress(0x123F350))(this);
}