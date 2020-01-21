#pragma once

#include <string>

#include "../inc/bedrock/BlockLegacy.h"

class ExampleBlock : public BlockLegacy {
public:
	ExampleBlock(const std::string&, int, const Material&);
};