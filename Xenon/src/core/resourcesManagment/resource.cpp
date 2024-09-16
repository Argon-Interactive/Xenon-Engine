#include "resource.hpp"

#include "devTools/logger_core.hpp"
#include "core/appData.hpp"
#include "async.hpp"

#include <fstream>

//TODO: Create error handeling here
/////////////////////////////////////////////////////////////
// Flag
/////////////////////////////////////////////////////////////

[[nodiscard]] bool Core::ResourceFlag::isValid() { 
	const std::lock_guard<std::mutex> lock(m_mutex);
	return p_getBit(0);
} 

[[nodiscard]] bool Core::ResourceFlag::isCompressed() {
	const std::lock_guard<std::mutex> lock(m_mutex);
	return p_getBit(1);
}

void Core::ResourceFlag::setCompressed() { 
	const std::lock_guard<std::mutex> lock(m_mutex);
	p_setBit(1); p_resetBit(0); 
}

void Core::ResourceFlag::resetCompressed() { 
	const std::lock_guard<std::mutex> lock(m_mutex);
	p_resetBit(1); if (!isEncrypted()) p_setBit(0); 
}

[[nodiscard]] bool Core::ResourceFlag::isEncrypted() { 
	const std::lock_guard<std::mutex> lock(m_mutex);
	return p_getBit(2); 
}

void Core::ResourceFlag::setEncrypted() { 
	const std::lock_guard<std::mutex> lock(m_mutex);
	p_setBit(2); p_resetBit(0); 
}

void Core::ResourceFlag::resetEncrypted() { 
	const std::lock_guard<std::mutex> lock(m_mutex);
	p_resetBit(2); if (!isCompressed()) p_setBit(0); 
}

/////////////////////////////////////////////////////////////
// Metadata
/////////////////////////////////////////////////////////////

Core::ResourceMetadata::~ResourceMetadata() {
	const std::lock_guard<std::mutex> lock(m_mutex);
	m_data.clear();
	m_data.shrink_to_fit();
	m_flag.setFlag(0);
}

void Core::ResourceMetadata::changePMR(std::pmr::memory_resource* newMemRes) {
	std::pmr::vector<uint8_t> newData{ std::move(m_data), newMemRes };
	m_data = newData;
}

std::optional<std::future<void>> Core::ResourceMetadata::load(const Core::ResourceHandle& handle, std::function<bool(std::pmr::vector<uint8_t>&)> decryptionFunc) {
	bool doNeedToLoad = false;
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		doNeedToLoad = m_referenceCounter++ == 0;
	}
	if (doNeedToLoad) return p_asyncLoad(handle, decryptionFunc);
	return std::nullopt;
}

void Core::ResourceMetadata::unload() {
	//TODO: This should throw an exception if i is called when reference counter is 0
	const std::lock_guard<std::mutex> lock(m_mutex);
	if (--m_referenceCounter == 0) {
		m_data.clear();
		m_data.shrink_to_fit();
		m_flag.setFlag(0);
	}
}

uint8_t* Core::ResourceMetadata::getRawData() {
	const std::lock_guard<std::mutex> lock(m_mutex);
	return m_data.data();
}

size_t Core::ResourceMetadata::getSize() {
	const std::lock_guard<std::mutex> lock(m_mutex);
	return m_data.size();
}

Core::ResourceFlag* Core::ResourceMetadata::getFlag() {
	return &m_flag;
}



std::future<void> Core::ResourceMetadata::p_asyncLoad(const Core::ResourceHandle& handle, std::function<bool(std::pmr::vector<uint8_t>&)> decryptionFunc) {
	const auto* path = Core::AppData::getResourceManager().getFilepathPtr();
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		m_type = handle.type;
	}
	m_flag.setFlag(1); //TEMP: 
	return al::async(al::launch::pooled, [this, handle, path, decryptionFunc]() {
		std::ifstream file(*path, std::ios::binary);
		if(!file.is_open()) {XN_LOG_ERR("Resourcepack failed to open."); return;}
		file.seekg(handle.offset);
		if(!file) {XN_LOG_ERR("Seeking the correct offset in Resourcepack failed."); return;}
		
		const std::lock_guard<std::mutex> lock(m_mutex); 
		m_data.resize(static_cast<size_t>(handle.size));
		file.read(reinterpret_cast<char*>(m_data.data()), handle.size);
		if(!file) {XN_LOG_ERR("Reading the data from Resourcepack failed."); return;}
		
		if (m_flag.isEncrypted()) {
			if(!decryptionFunc(m_data)) {XN_LOG_ERR("Decryption of an asset failed"); return;}
		}
		m_flag.resetEncrypted();
		m_flag.resetCompressed(); //TODO: Create a decopression interface
		file.close();
	});
}

