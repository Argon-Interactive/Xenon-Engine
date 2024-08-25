#include "assetsManager.hpp"
#include "devTools/logger_core.hpp"

#ifdef _WIN32
	#include<windows.h>
#elif __linux__
	
#else
	#error Only Linux and Windows are supported.
#endif


void Core::ResourceManager::init() {
	m_file.open("assets.ap", std::ios::binary);
	if(!m_file.is_open()) {XN_LOG_ERR("Failed to open AssetPack"); return;}
	struct AssetPackMetadata {
		char header[4];
		int64_t version;
		uint32_t assetsAmmount;
	};
	AssetPackMetadata apm{};
	m_file.read(reinterpret_cast<char*>(&apm), sizeof(AssetPackMetadata));
	if(apm.header[0] != 'X' || apm.header[1] != 'e' || apm.header[2] != 'A' || apm.header[3] != 'P') {XN_LOG_ERR("The Asset Pack is not valid"); return;}
	if(!p_versionCheck()) {XN_LOG_ERR("The Asset Pack is version is no longer supported"); return;}
	m_assetHandles = std::make_unique<AssetHandle[]>(apm.assetsAmmount);
	m_assetsMetadata = std::make_unique<AssetMetadata[]>(apm.assetsAmmount);
	m_file.seekg(sizeof(AssetPackMetadata));
	m_file.read(reinterpret_cast<char*>(m_assetHandles.get()), static_cast<std::streamsize>(apm.assetsAmmount * sizeof(AssetHandle)));
}

void Core::ResourceManager::terminate() {
	m_file.close();
}

void Core::ResourceManager::ensureLoaded(const assetID* assetIDs, uint64_t assetsAmmount) {
	for(uint64_t i = 0; i < assetsAmmount; ++i) {
		const assetID id = assetIDs[i];
		m_assetsMetadata[id].load(m_assetHandles[id], p_decryption);
	}
}

void Core::ResourceManager::freeAssets(const assetID* assetIDs, uint64_t assetsAmmount) {
	for(uint64_t i = 0; i < assetsAmmount; ++i) {
		const assetID id = assetIDs[i];
		m_assetsMetadata[id].unload();
	}
}

std::ifstream* Core::ResourceManager::getFile() { return &m_file; }

uint8_t* Core::ResourceManager::getAssetData(assetID id) const { return m_assetsMetadata[id].getData(); }
uint64_t Core::ResourceManager::getAssetSize(assetID id) const { return m_assetsMetadata[id].getSize(); }

bool Core::ResourceManager::p_decryption([[maybe_unused]] uint8_t* data, [[maybe_unused]] uint64_t size) {
	return true;
}

bool Core::ResourceManager::p_versionCheck() { return true; }

