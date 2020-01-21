#pragma once

#include <memory>
#include <string>
#include <functional>

class LocalPlayer {
public:
};

class ClientInstance {
public:
	char filler[0x890];

	virtual int temp() { return NULL; }
	virtual void setupGameClientGame(std::function<void(bool)>&&, bool) {}
	virtual LocalPlayer* getLocalPlayer() { return (LocalPlayer*)NULL; }
	virtual LocalPlayer* getLocalPlayer() const { return (LocalPlayer*)NULL; }
};

#include "../inc/bedrock/Ptr.h"
class Item;

class ExampleItems {
	static WeakPtr<Item> exampleItem;
	static ClientInstance* instance;

public:
	static void init();
	static void initCreativeItems();
	static void loadResources();
	static void passInstance(ClientInstance*);
};