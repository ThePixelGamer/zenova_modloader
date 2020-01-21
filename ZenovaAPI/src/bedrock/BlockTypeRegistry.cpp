#include "bedrock/BlockTypeRegistry.h"

#include "ZenovaCore.h"

std::unordered_map<std::string, SharedPtr<BlockLegacy>>* BlockTypeRegistry::mBlockTypeRegistryP = (std::unordered_map<std::string, SharedPtr<BlockLegacy>>*)SlideAddress(0x12778F0);