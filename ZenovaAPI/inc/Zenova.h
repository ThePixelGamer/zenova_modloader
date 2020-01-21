#pragma once

#ifdef ZENOVA_EXPORTS
#define ZENOVA_API __declspec(dllexport)
#else
#define ZENOVA_API __declspec(dllimport)
#endif

#include <vector>
#include <map>
#include <string>

struct Version {
	uint8_t major, minor, patch;
};

class ZenovaMod {
	std::string name, description;
	Version version;

protected:
	ZenovaMod(std::string name, Version version, std::string description) : name(name), version(version), description(description) {}
	
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Stop() = 0;
};

struct Symbol {
	Symbol* prev = nullptr;
	Symbol* next = nullptr;
};

class Zenova {
	//std::vector<ZenovaModAPI> apis;
	std::map<const char*, Symbol> syms;

public:
	//ZenovaModAPI& FindAPI(const char* name);
	bool CreateHook(void* address, void* funcJump, void** funcTrampoline);
	bool CreateHook(std::string funcName, void* funcJump, void** funcTrampoline);
};