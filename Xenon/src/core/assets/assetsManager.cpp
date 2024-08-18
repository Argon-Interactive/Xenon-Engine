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
	m_file.read(reinterpret_cast<char*>(&apm), 16);
	if(std::strcmp(apm.header, "XEAP") != 0) {XN_LOG_ERR("The Asset Pack is not valid"); return;}
	//NOTE: If the format ever changes a version check should be specified here
	m_assetHandles = new AssetHandle[apm.assetsAmmount];
	m_assetsMetadata = new AssetMetadata[apm.assetsAmmount];
	m_file.seekg(16);
	m_file.read(reinterpret_cast<char*>(m_assetHandles), static_cast<std::streamsize>(apm.assetsAmmount * sizeof(AssetHandle)));
}
void Core::AssetsManager::terminate() {
	m_file.close();
	delete[] m_assetHandles;
	delete[] m_assetsMetadata;
}
void Core::AssetsManager::ensureLoaded(const assetID* assetIDs, uint64_t assetsAmmount) {
	for(uint64_t i = 0; i < assetsAmmount; ++i) {
		assetID id = assetIDs[i];
		m_assetsMetadata[id].load(m_file, m_assetHandles[id]);
	}
}
void Core::AssetsManager::freeAssets(const assetID* assetIDs, uint64_t assetsAmmount) {
	for(uint64_t i = 0; i < assetsAmmount; ++i) {
		assetID id = assetIDs[i];
		m_assetsMetadata[id].unload();
	}
}
uint8_t* Core::AssetsManager::getAssetData(assetID id) const { return m_assetsMetadata[id].getData(); }
