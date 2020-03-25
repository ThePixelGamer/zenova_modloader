#pragma once

#include "Zenova/Hook.h"

enum class MaterialType {
	DIRT = 1,
	WOOD,
	STONE,
	IRON,
	WATER,
	LAVA,
	WOOL,
	DECORATION = 8,
	PLANT,
	SAND = 14,
	TORCH,
	PORTAL = 25,
	CIRCUIT = 28,
	LAMP = 30,
	OIL
};

class Material {
public:
	char filler[0x1C]; //0x8 : 8

	static const Material& getMaterial(MaterialType type) {
		return Zenova::Hook::Call<const Material&>(FSIG, type);
	}
};