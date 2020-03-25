#pragma once

#include "Zenova/Hook.h"

#include <string>

class ItemInstance;
class ItemStack;
class Level;
class CompoundTag;
enum class Color;
class Player;
class BlockSource;
class Container;
class Vec3;
class Block;
class Mob;
class Actor;
class IDataInput;
class ReadOnlyBinaryStream;
class IDataOutput;
class ItemDescriptor;
enum class CooldownType;
enum class InHandUpdateType;
class TextureUVCoordinateSet;

#include "CreativeItemCategory.h"
#include "UseAnimation.h"

#include "BlockPos.h"

enum class ResourceFileSystem {
	InUserPackage,
	InAppPackage,
	RawPath,
	InSettingDir,
	InExternalDir,
	InServerPackage,
	InDataDir,
	InUserDir,
	InWorldDir
};

class TextureUVCoordinateSet {
public:

	float u1, v1, u2, v2;
	unsigned short w, h;
	struct {
		std::string path;
		ResourceFileSystem fileSystem;
		std::string other;
	} location;
};

class Item {
private:
	char filler[0x148]; 

public:
	Item(ZenovaConstructor); //for Zenova's vtable modifier
	Item(const std::string&, short);

	virtual ~Item() {}
	virtual void tearDown() {}
	virtual int getMaxUseDuration(const ItemInstance*) const { return NULL; }
	virtual int getMaxUseDuration(const ItemStack*) const { return NULL; }
	virtual int isExperimental(const ItemInstance*) const { return NULL; }
	virtual Item& setMaxStackSize(unsigned char);
	virtual Item& setCategory(CreativeItemCategory);
	virtual Item& setStackedByData(bool);
	virtual Item& setMaxDamage(int);
	virtual Item& setHandEquipped();
	virtual Item& setUseAnimation(UseAnimation);
	virtual Item& setMaxUseDuration(int);
	virtual Item& setRequiresWorldBuilder(bool);
	virtual Item& setExplodable(bool);
	virtual Item& setIsGlint(bool);
	virtual Item& setShouldDespawn(bool);
	virtual int getBlockShape() const { return NULL; }
	virtual bool canBeDepleted() const { return NULL; }
	virtual bool canDestroySpecial() const { return NULL; }
	virtual int getLevelDataForAuxValue(int) const { return NULL; }
	virtual bool isStackedByData() const { return NULL; }
	virtual short getMaxDamage() const { return NULL; }
	virtual int getAttackDamage() const { return NULL; }
	virtual bool isHandEquipped() const { return NULL; }
	virtual bool isArmor() const { return NULL; }
	virtual bool isDye() const { return NULL; }
	virtual bool isFertilizer(int) const { return NULL; }
	virtual bool isGlint(const ItemInstance&) const { return NULL; }
	virtual bool isGlint(const ItemStack&) const { return NULL; }
	virtual bool isThrowable() const { return NULL; }
	virtual bool isPattern() const { return NULL; }
	virtual bool showsDurabilityInCreative() const { return NULL; }
	virtual bool isWearableThroughLootTable(const ItemStack&) const { return NULL; }
	virtual bool canDestroyInCreative() const { return NULL; }
	virtual bool isDestructive(int) const { return NULL; }
	virtual bool isLiquidClipItem(int) const { return NULL; }
	virtual bool requiresInteract() const { return NULL; }
	virtual void appendFormattedHovertext(const ItemStack&, Level&, const std::string&, bool) const {}
	virtual void appendFormattedHovertext(const ItemInstance&, Level&, const std::string&, bool) const {}
	virtual bool isValidRepairItem(const ItemStack&, const ItemStack&) const { return NULL; }
	virtual int getEnchantSlot() const { return NULL; }
	virtual int getEnchantValue() const { return NULL; }
	virtual int getArmorValue() const { return NULL; }
	virtual bool isComplex() const { return NULL; }
	virtual bool isValidAuxValue() const { return NULL; }
	virtual int getDamageChance() const { return NULL; }
	virtual bool uniqueAuxValues() const { return NULL; }
	virtual bool isMultiColorTinted(const ItemStack&) const { return NULL; }
	virtual Color getColor(const ItemInstance&) const { return (Color)NULL; }
	virtual Color getColor(const ItemStack&) const { return (Color)NULL; }
	virtual Color getBaseColor(const ItemStack&) const { return (Color)NULL; }
	virtual Color getSecondaryColor(const ItemStack&) const { return (Color)NULL; }
	virtual void saveAdditionalData(const ItemInstance&, CompoundTag&) const {}
	virtual void saveAdditionalData(const ItemStack&, CompoundTag&) const {}
	virtual void readAdditionalData(ItemStack&, const CompoundTag&) const {}
	virtual void readAdditionalData(ItemInstance&, const CompoundTag&) const {}
	virtual bool isTintable() const { return NULL; }
	virtual Item* use(ItemStack&, Player&) const { return (Item*)NULL; }
	virtual bool dispense(BlockSource&, Container&, int, const Vec3&, unsigned char) const { return (Item*)NULL; }
	virtual void useTimeDepleted(ItemInstance&, Level*, Player*) const {}
	virtual void useTimeDepleted(ItemStack&, Level*, Player*) const {}
	virtual void releaseUsing(ItemInstance&, Player*, int) const {}
	virtual void releaseUsing(ItemStack&, Player*, int) const {}
	virtual float getDestroySpeed(const ItemStack&, const Block&) const { return NULL; }
	virtual void hurtEnemy(ItemInstance&, Mob*, Mob*) const {}
	virtual void hurtEnemy(ItemStack&, Mob*, Mob*) const {}
	virtual bool mineBlock(ItemInstance&, const Block&, int, int, int, Actor*) const { return NULL; }
	virtual bool mineBlock(ItemStack&, const Block&, int, int, int, Actor*) const { return NULL; }
	virtual std::string buildDescriptionId(const ItemInstance&) const { return std::string(); }
	virtual std::string buildDescriptionId(const ItemStack&) const { return std::string(); }
	virtual std::string buildEffectDescriptionName(const ItemInstance&) const { return std::string(); }
	virtual std::string buildEffectDescriptionName(const ItemStack&) const { return std::string(); }
	virtual std::string buildCategoryDescriptionName(const ItemInstance&) const { return std::string(); }
	virtual std::string buildCategoryDescriptionName(const ItemStack&) const { return std::string(); }
	virtual void readUserData(ItemInstance&, IDataInput&, ReadOnlyBinaryStream&) const {}
	virtual void readUserData(ItemStack&, IDataInput&, ReadOnlyBinaryStream&) const {}
	virtual void writeUserData(const ItemInstance&, IDataOutput&) const {}
	virtual void writeUserData(const ItemStack&, IDataOutput&) const {}
	virtual unsigned char getMaxStackSize(const ItemDescriptor&) const { return NULL; }
	virtual void inventoryTick(ItemStack&, Level&, Actor&, int, bool) const {}
	virtual void refreshedInContainer(ItemStack&, Level&) const {}
	virtual void refreshedInContainer(ItemInstance&, Level&) const {}
	virtual CooldownType getCooldownType() const { return (CooldownType)NULL; }
	virtual CooldownType getCooldownTime() const { return (CooldownType)NULL; }
	virtual void fixupOnLoad(ItemInstance&) const {}
	virtual void fixupOnLoad(ItemStack&) const {}
	virtual short getDamageValue(const ItemStack&) const { return NULL; }
	virtual short getDamageValue(const ItemInstance&) const { return NULL; }
	virtual void setDamageValue(ItemStack&, short) const {}
	virtual void setDamageValue(ItemInstance&, short) const {}
	virtual InHandUpdateType getInHandUpdateType(const Player&, const ItemInstance&, const ItemInstance&, bool, bool) const { return (InHandUpdateType)NULL; }
	virtual InHandUpdateType getInHandUpdateType(const Player&, const ItemStack&, const ItemStack&, bool, bool) const { return (InHandUpdateType)NULL; }
	virtual bool isSameItem(const ItemInstance&, const ItemInstance&) const { return NULL; }
	virtual bool isSameItem(const ItemStack&, const ItemStack&) const { return NULL; }
	virtual std::string getInteractText(const Player&) const { return std::string(); }
	virtual int getAnimationFrameFor(Mob*, bool, const ItemStack*, bool) const { return NULL; }
	virtual bool isEmissive(int) const { return NULL; }
	virtual const TextureUVCoordinateSet& getIcon(int, int, bool) const;
	virtual int getIconYOffset() const { return NULL; }
	virtual Item& setIcon(const std::string&, int) { return *(Item*)NULL; }
	virtual Item& setIcon(const TextureUVCoordinateSet&) { return *(Item*)NULL; }
	virtual bool canBeCharged() const { return NULL; }
	virtual void playSoundIncrementally(const ItemInstance&, Mob&) const {}
	virtual void playSoundIncrementally(const ItemStack&, Mob&) const {}
	virtual std::string getAuxValuesDescription() const { return std::string(); }
	virtual bool _checkUseOnPermissions(Actor&, ItemInstance&, const unsigned char&, const BlockPos&) const { return NULL; }
	virtual bool _checkUseOnPermissions(Actor&, ItemStack&, const unsigned char&, const BlockPos&) const { return NULL; }
	virtual int _calculatePlacePos(ItemInstance&, Actor&, unsigned char&, BlockPos&) const { return NULL; }
	virtual int _calculatePlacePos(ItemStack&, Actor&, unsigned char&, BlockPos&) const { return NULL; }
	virtual bool _useOn(ItemInstance&, Actor&, BlockPos, unsigned char, float, float, float) const;
	virtual bool _useOn(ItemStack&, Actor&, BlockPos, unsigned char, float, float, float) const;
	
	Item& setAllowOffhand(bool);

	static void addCreativeItem(Item*, short);
};