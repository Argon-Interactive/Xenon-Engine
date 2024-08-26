#include "resourceManager.hpp"
#include "devTools/logger_core.hpp"


void Core::ResourceManager::ensureLoaded(const assetID* assetIDs, uint64_t assetsAmmount) {
	//TODO: 
}

void Core::ResourceManager::freeAssets(const assetID* assetIDs, uint64_t assetsAmmount) {
	//TODO: 
}

//TODO: 
uint8_t* Core::ResourceManager::getAssetData(assetID id) const { return m_assetsMetadata[id].getData(); }
uint64_t Core::ResourceManager::getAssetSize(assetID id) const { return m_assetsMetadata[id].getSize(); }

bool Core::ResourceManager::p_decryption([[maybe_unused]] uint8_t* data, [[maybe_unused]] uint64_t size) {
	return true;
}

bool Core::ResourceManager::p_versionCheck() { return true; }

