#include "Minecraft/BlockLegacy.h"
#include "Minecraft/Ptr.h"

#include "Zenova/Hook.h"

BlockLegacy::BlockLegacy(const std::string& name, int id, const Material& mat) {
	Zenova::Hook::Call<void>(FSIG, this, name, id, mat);
}

BlockLegacy& BlockLegacy::setCategory(CreativeItemCategory category) {
	return Zenova::Hook::Call<BlockLegacy&>(FSIG, this, category);
}

WeakPtr<BlockLegacy> BlockLegacy::createWeakPtr() {
	return Zenova::Hook::Call<WeakPtr<BlockLegacy>>(FSIG, this);
}

const std::string& BlockLegacy::getRawNameId() {
	return Zenova::Hook::Call<const std::string&>(FSIG, this);
}

BlockLegacy::~BlockLegacy() {}
void BlockLegacy::tick(BlockSource &,BlockPos const&,Random &) {}
void BlockLegacy::getStateFromLegacyData(unsigned short) {}
void BlockLegacy::getCollisionShape(AABB &,Block const&,BlockSource &,BlockPos const&,Actor *) {}
void BlockLegacy::isObstructingChests(BlockSource &,BlockPos const&) {}
void BlockLegacy::randomlyModifyPosition(BlockPos const&,int &) {}
void BlockLegacy::randomlyModifyPosition(BlockPos const&) {}
void BlockLegacy::addAABBs(Block const&,BlockSource &,BlockPos const&,AABB const*,std::vector<AABB,std::allocator<AABB>> &) {}
void BlockLegacy::getAABB(BlockSource &,BlockPos const&,Block const&,AABB &,bool) {}
void BlockLegacy::addCollisionShapes(Block const&,BlockSource &,BlockPos const&,AABB const*,std::vector<AABB,std::allocator<AABB>> &,Actor *) {}
void BlockLegacy::getOutline(BlockSource &,BlockPos const&,AABB &) {}
void BlockLegacy::getLiquidClipVolume(BlockSource &,BlockPos const&,AABB &) {}
void BlockLegacy::onProjectileHit(BlockSource &,BlockPos const&,Actor const&) {}
void BlockLegacy::liquidCanFlowIntoFromDirection(unsigned char,std::function<Block const& (BlockPos const&)>,BlockPos const&) {}
void BlockLegacy::getLightEmission(Block const&) {}
void BlockLegacy::shouldRandomTick(void) {}
void BlockLegacy::hasVariableLighting(void) {}
void BlockLegacy::isStrippable(Block const&) {}
void BlockLegacy::getStrippedBlock(Block const&) {}
void BlockLegacy::canProvideSupport(Block const&,unsigned char,BlockSupportType) {}
void BlockLegacy::canConnect(Block const&,unsigned char,Block const&) {}
void BlockLegacy::getConnectedDirections(Block const&,BlockPos const&,BlockSource &,bool &,bool &,bool &,bool &) {}
void BlockLegacy::isCropBlock(void) {}
void BlockLegacy::isStemBlock(void) {}
void BlockLegacy::isContainerBlock(void) {}
void BlockLegacy::isCraftingBlock(void) {}
void BlockLegacy::isInteractiveBlock(void) {}
void BlockLegacy::isWaterBlocking(void) {}
void BlockLegacy::isHurtableBlock(void) {}
void BlockLegacy::isFenceBlock(void) {}
void BlockLegacy::isFenceGateBlock(void) {}
void BlockLegacy::isThinFenceBlock(void) {}
void BlockLegacy::isWallBlock(void) {}
void BlockLegacy::isStairBlock(void) {}
void BlockLegacy::isSlabBlock(void) {}
void BlockLegacy::isDoorBlock(void) {}
void BlockLegacy::isRailBlock(void) {}
void BlockLegacy::isButtonBlock(void) {}
void BlockLegacy::canHurtAndBreakItem(void) {}
void BlockLegacy::isSignalSource(void) {}
void BlockLegacy::canBeOriginalSurface(void) {}
void BlockLegacy::isValidAuxValue(int) {}
void BlockLegacy::canFillAtPos(BlockSource &,BlockPos const&,Block const&) {}
void BlockLegacy::sanitizeFillBlock(BlockSource &,BlockPos const&,Block const&) {}
void BlockLegacy::onFillBlock(BlockSource &,BlockPos const&,Block const&) {}
void BlockLegacy::getDirectSignal(BlockSource &,BlockPos const&,int) {}
void BlockLegacy::waterSpreadCausesSpawn(void) {}
void BlockLegacy::canContainLiquid(void) {}
void BlockLegacy::shouldConnectToRedstone(BlockSource &,BlockPos const&,int) {}
void BlockLegacy::handleRain(BlockSource &,BlockPos const&,float) {}
void BlockLegacy::canBeUsedInCommands(bool) {}
void BlockLegacy::getThickness(void) {}
void BlockLegacy::getFlexibility(BlockSource &,BlockPos const&) {}
void BlockLegacy::checkIsPathable(Actor &,BlockPos const&,BlockPos const&) {}
void BlockLegacy::shouldDispense(BlockSource &,Container &) {}
void BlockLegacy::dispense(BlockSource &,Container &,int,Vec3 const&,unsigned char) {}
void BlockLegacy::onPlace(BlockSource &,BlockPos const&) {}
void BlockLegacy::onRemove(BlockSource &,BlockPos const&) {}
void BlockLegacy::onExploded(BlockSource &,BlockPos const&,Actor *) {}
void BlockLegacy::onStepOn(Actor &,BlockPos const&) {}
void BlockLegacy::onFallOn(BlockSource &,BlockPos const&,Actor *,float) {}
void BlockLegacy::transformOnFall(BlockSource &,BlockPos const&,Actor *,float) {}
void BlockLegacy::onRedstoneUpdate(BlockSource &,BlockPos const&,int,bool) {}
void BlockLegacy::onMove(BlockSource &,BlockPos const&,BlockPos const&) {}
void BlockLegacy::detachesOnPistonMove(BlockSource &,BlockPos const&) {}
void BlockLegacy::movedByPiston(BlockSource &,BlockPos const&) {}
void BlockLegacy::setupRedstoneComponent(BlockSource &,BlockPos const&) {}
void BlockLegacy::getRedstoneProperty(BlockSource &,BlockPos const&) {}
void BlockLegacy::updateEntityAfterFallOn(Actor &) {}
void BlockLegacy::isBounceBlock(void) {}
void BlockLegacy::ignoreEntitiesOnPistonMove(Block const&) {}
void BlockLegacy::onFertilized(BlockSource &,BlockPos const&,Actor *,FertilizerType) {}
void BlockLegacy::mayConsumeFertilizer(BlockSource &) {}
void BlockLegacy::mayPick(void) {}
void BlockLegacy::mayPick(BlockSource &,Block const&,bool) {}
void BlockLegacy::mayPlace(BlockSource &,BlockPos const&,unsigned char) {}
void BlockLegacy::mayPlace(BlockSource &,BlockPos const&) {}
void BlockLegacy::mayPlaceOn(BlockSource &,BlockPos const&) {}
void BlockLegacy::tryToPlace(BlockSource &,BlockPos const&,Block const&,ActorBlockSyncMessage const*) {}
void BlockLegacy::breaksFallingBlocks(Block const&) {}
void BlockLegacy::destroy(BlockSource &,BlockPos const&,Block const&,Actor *) {}
void BlockLegacy::playerWillDestroy(Player &,BlockPos const&,Block const&) {}
void BlockLegacy::getIgnoresDestroyPermissions(Actor &,BlockPos const&) {}
void BlockLegacy::neighborChanged(BlockSource &,BlockPos const&,BlockPos const&) {}
void BlockLegacy::getSecondPart(BlockSource &,BlockPos const&,BlockPos&) {}
void BlockLegacy::getResourceCount(Random &,Block const&,int) {}
void BlockLegacy::getResourceItem(Random &,Block const&,int) {}
void BlockLegacy::asItemInstance(BlockSource &,BlockPos const&,Block const&) {}
void BlockLegacy::spawnResources(BlockSource &,BlockPos const&,Block const&,float,int) {}
void BlockLegacy::spawnBurnResources(BlockSource &,float,float,float) {}
void BlockLegacy::getExplosionResistance(Actor *) {}
void BlockLegacy::clip(BlockSource &,BlockPos const&,Vec3 const&,Vec3 const&,bool) {}
void BlockLegacy::use(Player &,BlockPos const&) {}
void BlockLegacy::getPlacementBlock(Actor &,BlockPos const&,unsigned char,Vec3 const&,int) {}
void BlockLegacy::calcVariant(BlockSource &,BlockPos const&) {}
void BlockLegacy::isAttachedTo(BlockSource &,BlockPos const&,BlockPos&) {}
void BlockLegacy::attack(Player *,BlockPos const&) {}
void BlockLegacy::handleEntityInside(BlockSource &,BlockPos const&,Actor *,Vec3 &) {}
void BlockLegacy::entityInside(BlockSource &,BlockPos const&,Actor &) {}
void BlockLegacy::playerDestroy(Player &,BlockPos const&,Block const&) {}
void BlockLegacy::canSurvive(BlockSource &,BlockPos const&) {}
void BlockLegacy::getExperienceDrop(Random &) {}
void BlockLegacy::canBeBuiltOver(BlockSource &,BlockPos const&) {}
void BlockLegacy::triggerEvent(BlockSource &,BlockPos const&,int,int) {}
void BlockLegacy::getMobToSpawn(SpawnConditions const&,BlockSource &) {}
void BlockLegacy::getMapColor(BlockSource &,BlockPos const&) {}
void BlockLegacy::getMapColor(void) {}
void BlockLegacy::shouldStopFalling(Actor &) {}
void BlockLegacy::pushesUpFallingBlocks(void) {}
void BlockLegacy::calcGroundFriction(Mob &,BlockPos const&) {}
void BlockLegacy::canHaveExtraData(void) {}
void BlockLegacy::hasComparatorSignal(void) {}
void BlockLegacy::getComparatorSignal(BlockSource &,BlockPos const&,Block const&,unsigned char) {}
void BlockLegacy::canSlide(BlockSource &,BlockPos const&) {}
void BlockLegacy::getIconYOffset(void) {}
void BlockLegacy::getRenderLayer(void) {}
void BlockLegacy::buildDescriptionId(Block const&) {}
void BlockLegacy::isAuxValueRelevantForPicking(void) {}
void BlockLegacy::getColor(Block const&) {}
void BlockLegacy::getColorAtPos(BlockSource &,BlockPos const&) {}
void BlockLegacy::getColor(BlockSource &,BlockPos const&,Block const&) {}
void BlockLegacy::getColorForParticle(BlockSource &,BlockPos const&,Block const&) {}
void BlockLegacy::isSeasonTinted(Block const&,BlockSource &,BlockPos const&) {}
void BlockLegacy::onGraphicsModeChanged(bool,bool,bool,bool) {}
void BlockLegacy::getRenderLayer(Block const&,BlockSource &,BlockPos const&) {}
void BlockLegacy::getExtraRenderLayers(void) {}
void BlockLegacy::getShadeBrightness(void) {}
void BlockLegacy::getVisualShapeInWorld(Block const&,BlockSource &,BlockPos const&,AABB &,bool) {}
void BlockLegacy::getVisualShape(Block const&,AABB &,bool) {}
void BlockLegacy::telemetryVariant(BlockSource &,BlockPos const&) {}
void BlockLegacy::getVariant(Block const&) {}
void BlockLegacy::canSpawnOn(void) {}
void BlockLegacy::getMappedFace(unsigned char,Block const&) {}
void BlockLegacy::getFaceFlip(unsigned char,Block const&) {}
void BlockLegacy::animateTick(BlockSource &,BlockPos const&,Random &) {}
void BlockLegacy::init(void) {}
void BlockLegacy::setLightBlock(Brightness) {}
void BlockLegacy::setLightEmission(float) {}
void BlockLegacy::setExplodeable(float) {}
void BlockLegacy::setDestroyTime(float) {}
void BlockLegacy::setFriction(float) {}
void BlockLegacy::addProperty(BlockProperty) {}
void BlockLegacy::addState(ItemState const&) {}
void BlockLegacy::addState(ItemState const&,unsigned int) {}
void BlockLegacy::resetBitsUsed(void) {}
void BlockLegacy::setAllowsRunes(bool) {}
void BlockLegacy::setMapColor(Color const&) {}
void BlockLegacy::canBeSilkTouched(void) {}
void BlockLegacy::getSilkTouchItemInstance(Block const&) {}
void BlockLegacy::setVisualShape(AABB const&) {}
void BlockLegacy::setVisualShape(Vec3 const&,Vec3 const&) {}
void BlockLegacy::tryLegacyUpgrade(unsigned short) {}
void BlockLegacy::dealsContactDamage(Actor const&,Block const&,bool) {}