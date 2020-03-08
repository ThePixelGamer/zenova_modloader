#pragma once

#include <memory>
#include <string>
#include <functional>

template <class T>
struct WeakPtr;
class BlockLegacy;

class ExampleBlockTypes {
public:
	static WeakPtr<BlockLegacy> exampleBlockType;

	static void registerBlocks();
	static void initCreativeItems();
	static void loadResources();
};