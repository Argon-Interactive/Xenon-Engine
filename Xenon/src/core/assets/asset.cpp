#include "asset.hpp"
#include "devTools/logger_core.hpp"
#include <fstream>

bool Core::AssetMetadata::load(std::ifstream& file, const Core::AssetHandle& assetHandle) {
	if(!file.is_open()) {XN_LOG_ERR("Trying to read from an AssetPack file which isn't open."); return false;}
	file.seekg(assetHandle.offset);
	if(!file) {XN_LOG_ERR("Seeking the correct offset in AssetPack failed."); return false;}
	m_data = new uint8_t[static_cast<size_t>(assetHandle.size)];
	file.read(reinterpret_cast<char*>(m_data), assetHandle.size);
	if(!file) {XN_LOG_ERR("Reading the data from AssetPack failed."); return false;}
	m_counter++;
	return true;
}
void Core::AssetMetadata::unload() {if(--m_counter == 0) {delete[] m_data; m_data = nullptr;}}
uint8_t* Core::AssetMetadata::getData() const { return m_data; }
