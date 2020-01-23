#include "bedrock/Item.h"
#include "bedrock/UseAnimation.h"

#include "ZenovaCore.h"

Item::Item() {}

Item::Item(const std::string& name, short id) {
	((void (*)(Item*, const std::string&, int))SlideAddress(0x114CF40))(this, name, id);
}

Item& Item::setMaxStackSize(unsigned char stackSize) {
	return ((Item& (*)(Item*, unsigned char))SlideAddress(0x114D820))(this, stackSize);
}

Item& Item::setCategory(CreativeItemCategory category) { 
	return ((Item& (*)(Item*, CreativeItemCategory))SlideAddress(0x114F030))(this, category); 
}

Item& Item::setStackedByData(bool b) {
	return ((Item& (*)(Item*, bool))SlideAddress(0x1151AD0))(this, b); 
}

Item& Item::setMaxDamage(int i) {
	return ((Item& (*)(Item*, int))SlideAddress(0x1151AF0))(this, i);
}

Item& Item::setHandEquipped() {
	return ((Item& (*)(Item*))SlideAddress(0x114F210))(this); 
}

Item& Item::setUseAnimation(UseAnimation anim) {
	return ((Item& (*)(Item*, UseAnimation))SlideAddress(0x10FC530))(this, anim);
}

Item& Item::setMaxUseDuration(int i) {
	return ((Item& (*)(Item*, int))SlideAddress(0x10FC540))(this, i);
}

Item& Item::setRequiresWorldBuilder(bool b) {
	return ((Item& (*)(Item*, bool))SlideAddress(0x114D830))(this, b); 
}

Item& Item::setExplodable(bool b) {
	return ((Item& (*)(Item*, bool))SlideAddress(0x10FC550))(this, b); 
}

Item& Item::setIsGlint(bool b) {
	return ((Item& (*)(Item*, bool))SlideAddress(0x10FC570))(this, b); 
}

Item& Item::setShouldDespawn(bool b) {
	return ((Item& (*)(Item*, bool))SlideAddress(0x10FC590))(this, b); 
}

void Item::addCreativeItem(Item* item, short data) {
	((void (*)(Item*, short))SlideAddress(0x114C8E0))(item, data);
}

Item& Item::setAllowOffhand(bool boolean) {
	return ((Item& (*)(Item*, bool))SlideAddress(0x114F230))(this, boolean);
}

const TextureUVCoordinateSet& Item::getIcon(int x, int y, bool b) const { 
	return ((TextureUVCoordinateSet& (*)(int, int, bool))SlideAddress(0x0))(x, y, b); 
}

bool Item::_useOn(ItemInstance& item, Actor& actor, BlockPos blockpos, unsigned char uc, float f1, float f2, float f3) const { 
	return ((bool (*)(const Item*, ItemInstance&, Actor&, BlockPos, unsigned char, float, float, float))SlideAddress(0x114E770))(this, item, actor, blockpos, uc, f1, f2, f3); 
}

bool Item::_useOn(ItemStack& item, Actor& actor, BlockPos blockpos, unsigned char uc, float f1, float f2, float f3) const { 
	return ((bool (*)(const Item*, ItemStack&, Actor&, BlockPos, unsigned char, float, float, float))SlideAddress(0x114E770))(this, item, actor, blockpos, uc, f1, f2, f3); 
}

/*typedef void (*fn_addCreativeItem)(ItemInsatance const&);
void Item::addCreativeItem(ItemInsatance const&) {
	((fn_addCreativeItem)SlideAddress(0x114C990))(item, data);
}*/