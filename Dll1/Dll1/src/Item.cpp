#include "../mcpe/Item.h"
#include "../mcpe/UseAnimation.h"
#include "../mcpe/BlockPos.h"

Item::Item(const std::string&, int) {
	//call function at a certain address
}

Item::~Item() {

}

void Item::tearDown() {

}

int Item::getMaxUseDuration(const ItemInstance*) const {
	return 0;
}

int Item::getMaxUseDuration(const ItemStack*) const {
	return 0;

}

int Item::isExperimental(const ItemInstance*) const {
	return 0;

}

Item* Item::setMaxStackSize(unsigned char) {
	return 0;

}

Item* Item::setCategory(CreativeItemCategory) {
	return 0;

}

Item* Item::setStackedByData(bool) {
	return 0;

}

Item* Item::setMaxDamage(int) {
	return 0;

}

Item* Item::setHandEquipped() {
	return 0;

}

Item* Item::setUseAnimation(UseAnimation) {
	return 0;
}

Item* Item::setMaxUseDuration(int) { return 0; }
Item* Item::setRequiresWorldBuilder(bool) { return 0; }
Item* Item::setExplodable(bool) { return 0; }
Item* Item::setIsGlint(bool) { return 0; }
Item* Item::setShouldDespawn(bool) { return 0; }
int Item::getBlockShape() const { return 0; }
bool Item::canBeDepleted() const { return 0; }
bool Item::canDestroySpecial() const { return 0; }
int Item::getLevelDataForAuxValue(int) const { return 0; }
bool Item::isStackedByData() const { return 0; }
short Item::getMaxDamage() const { return 0; }
int Item::getAttackDamage() const { return 0; }
bool Item::isHandEquipped() const { return 0; }
bool Item::isArmor() const { return 0; }
bool Item::isDye() const { return 0; }
bool Item::isFertilizer(int) const { return 0; }
bool Item::isGlint(const ItemInstance&) const { return 0; }
bool Item::isGlint(const ItemStack&) const { return 0; }
bool Item::isThrowable() const { return 0; }
bool Item::isPattern() const { return 0; }
bool Item::showsDurabilityInCreative() const { return 0; }
bool Item::isWearableThroughLootTable(const ItemStack&) const { return 0; }
bool Item::canDestroyInCreative() const { return 0; }
bool Item::isDestructive(int) const { return 0; }
bool Item::isLiquidClipItem(int) const { return 0; }
bool Item::requiresInteract() const { return 0; }
void Item::appendFormattedHovertext(const ItemStack&, Level&, const std::string&, bool) const {}
void Item::appendFormattedHovertext(const ItemInstance&, Level&, const std::string&, bool) const {}
int Item::getEnchantSlot() const { return 0; }
int Item::getEnchantValue() const { return 0; }
int Item::getArmorValue() const { return 0; }
bool Item::isComplex() const { return 0; }
bool Item::isValidAuxValue() const { return 0; }
int Item::getDamageChance() const { return 0; }
bool Item::uniqueAuxValues() const { return 0; }
bool Item::isMultiColorTinted(const ItemStack&) const { return 0; }
Color Item::getColor(const ItemInstance&) const {}
Color Item::getColor(const ItemStack&) const {}
Color Item::getBaseColor(const ItemStack&) const {}
Color Item::getSecondaryColor(const ItemStack&) const {}
void Item::saveAdditionalData(const ItemInstance&, CompoundTag&) const {}
void Item::saveAdditionalData(const ItemStack&, CompoundTag&) const {}
void Item::readAdditionalData(ItemStack&, const CompoundTag&) const {}
void Item::readAdditionalData(ItemInstance&, const CompoundTag&) const {}
bool Item::isTintable() const { return 0; }
Item* Item::use(ItemStack&, Player&) const { return 0; }
bool Item::dispense(BlockSource&, Container&, int, const Vec3&, unsigned char) const { return 0; }
void Item::useTimeDepleted(ItemInstance&, Level*, Player*) const {}
void Item::useTimeDepleted(ItemStack&, Level*, Player*) const {}
void Item::releaseUsing(ItemInstance&, Player*, int) const {}
void Item::releaseUsing(ItemStack&, Player*, int) const {}
float Item::getDestroySpeed(const ItemStack&, const Block&) const { return 0; }
void Item::hurtEnemy(ItemInstance&, Mob*, Mob*) const {}
void Item::hurtEnemy(ItemStack&, Mob*, Mob*) const {}
bool Item::mineBlock(ItemInstance&, const Block&, int, int, int, Actor*) const { return 0; }
bool Item::mineBlock(ItemStack&, const Block&, int, int, int, Actor*) const { return 0; }
std::string Item::buildDescriptionId(const ItemInstance&) const { return 0; }
std::string Item::buildDescriptionId(const ItemStack&) const { return 0; }
std::string Item::buildEffectDescriptionName(const ItemInstance&) const { return 0; }
std::string Item::buildEffectDescriptionName(const ItemStack&) const { return 0; }
std::string Item::buildCategoryDescriptionName(const ItemInstance&) const { return 0; }
std::string Item::buildCategoryDescriptionName(const ItemStack&) const { return 0; }
void Item::readUserData(ItemInstance&, IDataInput&, ReadOnlyBinaryStream&) const {}
void Item::readUserData(ItemStack&, IDataInput&, ReadOnlyBinaryStream&) const {}
void Item::writeUserData(const ItemInstance&, IDataOutput&) const {}
void Item::writeUserData(const ItemStack&, IDataOutput&) const {}
unsigned char Item::getMaxStackSize(const ItemDescriptor&) const { return 0; }
void Item::inventoryTick(ItemStack&, Level&, Actor&, int, bool) const {}
void Item::refreshedInContainer(ItemStack&, Level&) const {}
void Item::refreshedInContainer(ItemInstance&, Level&) const {}
CooldownType Item::getCooldownType() const {}
CooldownType Item::getCooldownTime() const {}
void Item::fixupOnLoad(ItemInstance&) const {}
void Item::fixupOnLoad(ItemStack&) const {}
short Item::getDamageValue(const ItemStack&) const { return 0; }
short Item::getDamageValue(const ItemInstance&) const { return 0; }
void Item::setDamageValue(ItemStack&, short) const {}
void Item::setDamageValue(ItemInstance&, short) const {}
InHandUpdateType Item::getInHandUpdateType(const Player&, const ItemInstance&, const ItemInstance&, bool, bool) const {}
InHandUpdateType Item::getInHandUpdateType(const Player&, const ItemStack&, const ItemStack&, bool, bool) const {}
bool Item::isSameItem(const ItemInstance&, const ItemInstance&) const { return 0; }
bool Item::isSameItem(const ItemStack&, const ItemStack&) const { return 0; }
std::string Item::getInteractText(const Player&) const { return 0; }
int Item::getAnimationFrameFor(Mob*, bool, const ItemStack*, bool) const { return 0; }
bool Item::isEmissive(int) const { return 0; }
const TextureUVCoordinateSet& Item::getIcon(int, int, bool) const {}
int Item::getIconYOffset() const { return 0; }
Item* Item::setIcon(const std::string&, int) { return 0; }
Item* Item::setIcon(const TextureUVCoordinateSet&) { return 0; }
bool Item::canBeCharged() const { return 0; }
void Item::playSoundIncrementally(const ItemInstance&, Mob&) const {}
void Item::playSoundIncrementally(const ItemStack&, Mob&) const {}
std::string Item::getAuxValuesDescription() const { return 0; }
bool Item::_checkUseOnPermissions(Actor&, ItemInstance&, const unsigned char&, const BlockPos&) const { return 0; }
bool Item::_checkUseOnPermissions(Actor&, ItemStack&, const unsigned char&, const BlockPos&) const { return 0; }
int Item::_calculatePlacePos(ItemInstance&, Actor&, unsigned char&, BlockPos&) const { return 0; }
int Item::_calculatePlacePos(ItemStack&, Actor&, unsigned char&, BlockPos&) const { return 0; }
bool Item::_useOn(ItemInstance&, Actor&, BlockPos, unsigned char, float, float, float) const { return 0; }
bool Item::_useOn(ItemStack&, Actor&, BlockPos, unsigned char, float, float, float) const { return 0; }

void Item::addCreativeItem(Item*, short) {

}