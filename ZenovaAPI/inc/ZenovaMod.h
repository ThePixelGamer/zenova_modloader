#pragma once

#include <vector>

struct ZenovaMod {
	const char* m_name;
	const char* m_description;
	std::vector<struct ZenovaModAPI*> m_apis;

	ZenovaMod(const char* name, const char* description) : m_name(name), m_description(description) {

	}

	~ZenovaMod() {
		printf("%s unloaded.", m_name);
	}

	void RemoveFromAPIs() {
		/*for(auto api: m_apis) {
			api->RemoveMod(this);
		}*/
	}
};

struct ZenovaModAPI {
	const char* m_name;
	const char* m_description;
	std::vector<ZenovaMod*> m_mods;
	
	ZenovaModAPI(const char* name, const char* description) : m_name(name), m_description(description) {

	}

	~ZenovaModAPI() {
		for(auto mods : m_mods) {
			mods->RemoveFromAPIs();
		}
		printf("%s unloaded.", m_name);
	}

	void SetupHooks() {}

	void AddMod(ZenovaMod* mod) {
		auto pos = std::find(m_mods.begin(), m_mods.end(), mod);
		if(pos != m_mods.end()) printf("%s already in %s\n", mod->m_name, m_name);
		else m_mods.push_back(mod);
	}

	void RemoveMod(ZenovaMod* mod) {
		auto pos = std::find(m_mods.begin(), m_mods.end(), mod);
		if(pos == m_mods.end()) printf("%s not found in %s\n", mod->m_name, m_name);
		else m_mods.erase(pos);
	}
};