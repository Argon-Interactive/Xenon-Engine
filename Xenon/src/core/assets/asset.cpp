#include "asset.hpp"
#include "devTools/logger_core.hpp"
#include "core/appData.hpp"
#include <fstream>

bool Core::AssetMetadata::load(const Core::AssetHandle& assetHandle, std::function<bool(uint8_t*, uint64_t)> decryptionFunc) {
	if(m_counter == 0) {
		auto* file = AppData::getAssetManager().getFile();
		if(!file->is_open()) {XN_LOG_ERR("Trying to read from an AssetPack file which isn't open."); return false;}
		m_size = static_cast<uint64_t>(assetHandle.size);
		file->seekg(assetHandle.offset);
		if(!(*file)) {XN_LOG_ERR("Seeking the correct offset in AssetPack failed."); return false;}
		m_data = std::make_unique<uint8_t[]>(m_size);
		file->read(reinterpret_cast<char*>(m_data.get()), assetHandle.size);
		if(!(*file)) {XN_LOG_ERR("Reading the data from AssetPack failed."); return false;}
		if(!decryptionFunc(m_data.get(), m_size)) {XN_LOG_ERR("Decryption of an asset failed"); return false;}
	}
	m_counter++;
	return true;
}

void Core::AssetMetadata::unload() {if(--m_counter == 0) {m_data.reset(); m_data = nullptr;}}

uint8_t* Core::AssetMetadata::getData() const { return m_data.get(); }
uint64_t Core::AssetMetadata::getSize() const { return m_size; }
