#include "assetsManager.hpp"
#include "devTools/logger_core.hpp"
#include <cstring>

void Core::AssetsManager::init() {
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

void Core::AssetsManager::terminate() {
	m_file.close();
}

void Core::AssetsManager::ensureLoaded(const assetID* assetIDs, uint64_t assetsAmmount) {
	for(uint64_t i = 0; i < assetsAmmount; ++i) {
		assetID id = assetIDs[i];
		m_assetsMetadata[id].load(m_assetHandles[id], p_decryption);
	}
}

void Core::AssetsManager::freeAssets(const assetID* assetIDs, uint64_t assetsAmmount) {
	for(uint64_t i = 0; i < assetsAmmount; ++i) {
		assetID id = assetIDs[i];
		m_assetsMetadata[id].unload();
	}
}

std::ifstream* Core::AssetsManager::getFile() { return &m_file; }

uint8_t* Core::AssetsManager::getAssetData(assetID id) const { return m_assetsMetadata[id].getData(); }
uint64_t Core::AssetsManager::getAssetSize(assetID id) const { return m_assetsMetadata[id].getSize(); }

bool Core::AssetsManager::p_decryption(uint8_t* data [[maybe_unused]], uint64_t size [[maybe_unused]]) {
	return true;
}

bool Core::AssetsManager::p_versionCheck() { return true; }

