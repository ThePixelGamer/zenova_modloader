#pragma once

#include "Zenova/Hook.h"

#include "Material.h"
#include "CreativeItemCategory.h"

#include <string>
#include <functional>

class Block;
class BlockPos;
class BlockSource;
class Random;
class AABB;
class Actor;
class Container;
class Vec3;
class ActorBlockSyncMessage;
class Player;
class SpawnConditions;
class Mob;
class ItemState;
enum class BlockSupportType;
enum class FertilizerType;
enum class Color;
enum class Brightness;
enum class BlockProperty;

template<typename T>
struct WeakPtr;

class BlockLegacy {
public:
	//vtable 0x0 : 0
	char filler1[0xFC];
	short blockID; //0x104 : 260
	char filler[0xCEA]; //0x106 : 262
	const Block* block; //0xDF0 : 3568
	
	BlockLegacy(ZenovaConstructor) {}
	BlockLegacy() {}
	BlockLegacy(const std::string&, int, const Material&);

	BlockLegacy& setCategory(CreativeItemCategory);
	WeakPtr<BlockLegacy> createWeakPtr();
	const std::string& getRawNameId();

	virtual ~BlockLegacy();
	virtual void tick(BlockSource &,BlockPos const&,Random &);
	virtual void getStateFromLegacyData(unsigned short);
	virtual void getCollisionShape(AABB &,Block const&,BlockSource &,BlockPos const&,Actor *);
	virtual void isObstructingChests(BlockSource &,BlockPos const&);
	virtual void randomlyModifyPosition(BlockPos const&,int &);
	virtual void randomlyModifyPosition(BlockPos const&);
	virtual void addAABBs(Block const&,BlockSource &,BlockPos const&,AABB const*,std::vector<AABB,std::allocator<AABB>> &);
	virtual void getAABB(BlockSource &,BlockPos const&,Block const&,AABB &,bool);
	virtual void addCollisionShapes(Block const&,BlockSource &,BlockPos const&,AABB const*,std::vector<AABB,std::allocator<AABB>> &,Actor *);
	virtual void getOutline(BlockSource &,BlockPos const&,AABB &);
	virtual void getLiquidClipVolume(BlockSource &,BlockPos const&,AABB &);
	virtual void onProjectileHit(BlockSource &,BlockPos const&,Actor const&);
	virtual void liquidCanFlowIntoFromDirection(unsigned char,std::function<Block const& (BlockPos const&)>,BlockPos const&);
	virtual void getLightEmission(Block const&);
	virtual void shouldRandomTick(void);
	virtual void hasVariableLighting(void);
	virtual void isStrippable(Block const&);
	virtual void getStrippedBlock(Block const&);
	virtual void canProvideSupport(Block const&,unsigned char,BlockSupportType);
	virtual void canConnect(Block const&,unsigned char,Block const&);
	virtual void getConnectedDirections(Block const&,BlockPos const&,BlockSource &,bool &,bool &,bool &,bool &);
	virtual void isCropBlock(void);
	virtual void isStemBlock(void);
	virtual void isContainerBlock(void);
	virtual void isCraftingBlock(void);
	virtual void isInteractiveBlock(void);
	virtual void isWaterBlocking(void);
	virtual void isHurtableBlock(void);
	virtual void isFenceBlock(void);
	virtual void isFenceGateBlock(void);
	virtual void isThinFenceBlock(void);
	virtual void isWallBlock(void);
	virtual void isStairBlock(void);
	virtual void isSlabBlock(void);
	virtual void isDoorBlock(void);
	virtual void isRailBlock(void);
	virtual void isButtonBlock(void);
	virtual void canHurtAndBreakItem(void);
	virtual void isSignalSource(void);
	virtual void canBeOriginalSurface(void);
	virtual void isValidAuxValue(int);
	virtual void canFillAtPos(BlockSource &,BlockPos const&,Block const&);
	virtual void sanitizeFillBlock(BlockSource &,BlockPos const&,Block const&);
	virtual void onFillBlock(BlockSource &,BlockPos const&,Block const&);
	virtual void getDirectSignal(BlockSource &,BlockPos const&,int);
	virtual void waterSpreadCausesSpawn(void);
	virtual void canContainLiquid(void);
	virtual void shouldConnectToRedstone(BlockSource &,BlockPos const&,int);
	virtual void handleRain(BlockSource &,BlockPos const&,float);
	virtual void canBeUsedInCommands(bool);
	virtual void getThickness(void);
	virtual void getFlexibility(BlockSource &,BlockPos const&);
	virtual void checkIsPathable(Actor &,BlockPos const&,BlockPos const&);
	virtual void shouldDispense(BlockSource &,Container &);
	virtual void dispense(BlockSource &,Container &,int,Vec3 const&,unsigned char);
	virtual void onPlace(BlockSource &,BlockPos const&);
	virtual void onRemove(BlockSource &,BlockPos const&);
	virtual void onExploded(BlockSource &,BlockPos const&,Actor *);
	virtual void onStepOn(Actor &,BlockPos const&);
	virtual void onFallOn(BlockSource &,BlockPos const&,Actor *,float);
	virtual void transformOnFall(BlockSource &,BlockPos const&,Actor *,float);
	virtual void onRedstoneUpdate(BlockSource &,BlockPos const&,int,bool);
	virtual void onMove(BlockSource &,BlockPos const&,BlockPos const&);
	virtual void detachesOnPistonMove(BlockSource &,BlockPos const&);
	virtual void movedByPiston(BlockSource &,BlockPos const&);
	virtual void setupRedstoneComponent(BlockSource &,BlockPos const&);
	virtual void getRedstoneProperty(BlockSource &,BlockPos const&);
	virtual void updateEntityAfterFallOn(Actor &);
	virtual void isBounceBlock(void);
	virtual void ignoreEntitiesOnPistonMove(Block const&);
	virtual void onFertilized(BlockSource &,BlockPos const&,Actor *,FertilizerType);
	virtual void mayConsumeFertilizer(BlockSource &);
	virtual void mayPick(void);
	virtual void mayPick(BlockSource &,Block const&,bool);
	virtual void mayPlace(BlockSource &,BlockPos const&,unsigned char);
	virtual void mayPlace(BlockSource &,BlockPos const&);
	virtual void mayPlaceOn(BlockSource &,BlockPos const&);
	virtual void tryToPlace(BlockSource &,BlockPos const&,Block const&,ActorBlockSyncMessage const*);
	virtual void breaksFallingBlocks(Block const&);
	virtual void destroy(BlockSource &,BlockPos const&,Block const&,Actor *);
	virtual void playerWillDestroy(Player &,BlockPos const&,Block const&);
	virtual void getIgnoresDestroyPermissions(Actor &,BlockPos const&);
	virtual void neighborChanged(BlockSource &,BlockPos const&,BlockPos const&);
	virtual void getSecondPart(BlockSource &,BlockPos const&,BlockPos&);
	virtual void getResourceCount(Random &,Block const&,int);
	virtual void getResourceItem(Random &,Block const&,int);
	virtual void asItemInstance(BlockSource &,BlockPos const&,Block const&);
	virtual void spawnResources(BlockSource &,BlockPos const&,Block const&,float,int);
	virtual void spawnBurnResources(BlockSource &,float,float,float);
	virtual void getExplosionResistance(Actor *);
	virtual void clip(BlockSource &,BlockPos const&,Vec3 const&,Vec3 const&,bool);
	virtual void use(Player &,BlockPos const&);
	virtual void getPlacementBlock(Actor &,BlockPos const&,unsigned char,Vec3 const&,int);
	virtual void calcVariant(BlockSource &,BlockPos const&);
	virtual void isAttachedTo(BlockSource &,BlockPos const&,BlockPos&);
	virtual void attack(Player *,BlockPos const&);
	virtual void handleEntityInside(BlockSource &,BlockPos const&,Actor *,Vec3 &);
	virtual void entityInside(BlockSource &,BlockPos const&,Actor &);
	virtual void playerDestroy(Player &,BlockPos const&,Block const&);
	virtual void canSurvive(BlockSource &,BlockPos const&);
	virtual void getExperienceDrop(Random &);
	virtual void canBeBuiltOver(BlockSource &,BlockPos const&);
	virtual void triggerEvent(BlockSource &,BlockPos const&,int,int);
	virtual void getMobToSpawn(SpawnConditions const&,BlockSource &);
	virtual void getMapColor(BlockSource &,BlockPos const&);
	virtual void getMapColor(void);
	virtual void shouldStopFalling(Actor &);
	virtual void pushesUpFallingBlocks(void);
	virtual void calcGroundFriction(Mob &,BlockPos const&);
	virtual void canHaveExtraData(void);
	virtual void hasComparatorSignal(void);
	virtual void getComparatorSignal(BlockSource &,BlockPos const&,Block const&,unsigned char);
	virtual void canSlide(BlockSource &,BlockPos const&);
	virtual void getIconYOffset(void);
	virtual void getRenderLayer(void);
	virtual void buildDescriptionId(Block const&);
	virtual void isAuxValueRelevantForPicking(void);
	virtual void getColor(Block const&);
	virtual void getColorAtPos(BlockSource &,BlockPos const&);
	virtual void getColor(BlockSource &,BlockPos const&,Block const&);
	virtual void getColorForParticle(BlockSource &,BlockPos const&,Block const&);
	virtual void isSeasonTinted(Block const&,BlockSource &,BlockPos const&);
	virtual void onGraphicsModeChanged(bool,bool,bool,bool);
	virtual void getRenderLayer(Block const&,BlockSource &,BlockPos const&);
	virtual void getExtraRenderLayers(void);
	virtual void getShadeBrightness(void);
	virtual void getVisualShapeInWorld(Block const&,BlockSource &,BlockPos const&,AABB &,bool);
	virtual void getVisualShape(Block const&,AABB &,bool);
	virtual void telemetryVariant(BlockSource &,BlockPos const&);
	virtual void getVariant(Block const&);
	virtual void canSpawnOn(void);
	virtual void getMappedFace(unsigned char,Block const&);
	virtual void getFaceFlip(unsigned char,Block const&);
	virtual void animateTick(BlockSource &,BlockPos const&,Random &);
	virtual void init(void);
	virtual void setLightBlock(Brightness);
	virtual void setLightEmission(float);
	virtual void setExplodeable(float);
	virtual void setDestroyTime(float);
	virtual void setFriction(float);
	virtual void addProperty(BlockProperty);
	virtual void addState(ItemState const&);
	virtual void addState(ItemState const&,unsigned int);
	virtual void resetBitsUsed(void);
	virtual void setAllowsRunes(bool);
	virtual void setMapColor(Color const&);
	virtual void canBeSilkTouched(void);
	virtual void getSilkTouchItemInstance(Block const&);
	virtual void setVisualShape(AABB const&);
	virtual void setVisualShape(Vec3 const&,Vec3 const&);
	virtual void tryLegacyUpgrade(unsigned short);
	virtual void dealsContactDamage(Actor const&,Block const&,bool);
};