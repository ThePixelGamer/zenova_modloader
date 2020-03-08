#pragma once

#include <string>

#include "../inc/Minecraft/BlockLegacy.h"

class ExampleBlock : public BlockLegacy {
public:
	ExampleBlock(const std::string&, int, const Material&);
};