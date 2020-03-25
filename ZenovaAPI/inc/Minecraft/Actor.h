#pragma once

class BlockSource;

class Actor {
public:
	//size 0x11D0
	char filler[0xCF8];
	BlockSource* blocksource;
};