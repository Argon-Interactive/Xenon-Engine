#include "asset.hpp"
#include "devTools/logger_core.hpp"
#include "core/appData.hpp"
#include <fstream>

bool Core::AssetMetadata::load(const Core::AssetHandle& assetHandle, std::function<bool(uint8_t*, uint64_t)> decryptionFunc) {
	if(m_counter == 0) {
	}
	m_counter++;
	return true;
}

void Core::AssetMetadata::unload() {if(--m_counter == 0) {m_data.reset(); m_data = nullptr;}}

uint8_t* Core::AssetMetadata::getData() const { return m_data.get(); }
uint64_t Core::AssetMetadata::getSize() const { return m_size; }
