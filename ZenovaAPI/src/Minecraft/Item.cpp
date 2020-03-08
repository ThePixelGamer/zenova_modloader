#include "Minecraft/Item.h"
#include "Minecraft/UseAnimation.h"

Item::Item(ZenovaConstructor) {}

Item::Item(const std::string& name, short id) {
	Zenova::Hook::Call<void>(FSIG, this, name, id);
}

Item& Item::setMaxStackSize(unsigned char stackSize) {
	return Zenova::Hook::Call<Item&>(FSIG, this, stackSize);
}

Item& Item::setCategory(CreativeItemCategory category) { 
	return Zenova::Hook::Call<Item&>(FSIG, this, category);
}

Item& Item::setStackedByData(bool b) {
	return Zenova::Hook::Call<Item&>(FSIG, this, b);
}

Item& Item::setMaxDamage(int i) {
	return Zenova::Hook::Call<Item&>(FSIG, this, i);
}

Item& Item::setHandEquipped() {
	return Zenova::Hook::Call<Item&>(FSIG, this);
}

Item& Item::setUseAnimation(UseAnimation anim) {
	return Zenova::Hook::Call<Item&>(FSIG, this, anim);
}

Item& Item::setMaxUseDuration(int i) {
	return Zenova::Hook::Call<Item&>(FSIG, this, i);
}

Item& Item::setRequiresWorldBuilder(bool b) {
	return Zenova::Hook::Call<Item&>(FSIG, this, b);
}

Item& Item::setExplodable(bool b) {
	return Zenova::Hook::Call<Item&>(FSIG, this, b);
}

Item& Item::setIsGlint(bool b) {
	return Zenova::Hook::Call<Item&>(FSIG, this, b);
}

Item& Item::setShouldDespawn(bool b) {
	return Zenova::Hook::Call<Item&>(FSIG, this, b);
}

Item& Item::setAllowOffhand(bool b) {
	return Zenova::Hook::Call<Item&>(FSIG, this, b);
}

const TextureUVCoordinateSet& Item::getIcon(int x, int y, bool b) const { 
	return Zenova::Hook::Call<const TextureUVCoordinateSet&>(FSIG, this, x, y, b);
}

bool Item::_useOn(ItemInstance& item, Actor& actor, BlockPos blockpos, unsigned char uc, float f1, float f2, float f3) const { 
	return Zenova::Hook::Call<bool>(FSIG, this, std::ref(item), std::ref(actor), blockpos, uc, f1, f2, f3);
}

bool Item::_useOn(ItemStack& item, Actor& actor, BlockPos blockpos, unsigned char uc, float f1, float f2, float f3) const { 
	return Zenova::Hook::Call<bool>(FSIG, this, std::ref(item), std::ref(actor), blockpos, uc, f1, f2, f3);
}

void Item::addCreativeItem(Item* item, short data) {
	return Zenova::Hook::Call<void>(FSIG, item, data);
}

/*typedef void (*fn_addCreativeItem)(ItemInsatance const&);
void Item::addCreativeItem(ItemInsatance const&) {
	((fn_addCreativeItem)SlideAddress(0x114C990))(item, data);
}*/