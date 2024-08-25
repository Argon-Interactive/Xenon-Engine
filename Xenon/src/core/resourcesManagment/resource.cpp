#include "resource.hpp"

#include "devTools/logger_core.hpp"

#include <fstream>

std::list<std::future<void>> Core::ResourceMetadata::sm_futures;
std::mutex Core::ResourceMetadata::sm_futuresMutex;

Core::ResourceMetadata::~ResourceMetadata() {
	p_asyncUnload();
}
void Core::ResourceMetadata::load(const Core::ResourceHandle& handle, std::function<bool(std::pmr::vector<uint8_t>)> decryptionFunc) {
	if (m_referenceCounter++ == 0) p_asyncLoad(handle, decryptionFunc);
}

void Core::ResourceMetadata::unload() {
	if (--m_referenceCounter == 0) p_asyncUnload();
}

uint8_t* Core::ResourceMetadata::getRawData() {
	const std::lock_guard<std::mutex> lock(m_mutex);
	return m_data.data();
}

size_t Core::ResourceMetadata::getSize() {
	const std::lock_guard<std::mutex> lock(m_mutex);
	return m_data.size();
}

void Core::ResourceMetadata::sync() {
	for (auto& future : sm_futures) {
		future.wait();
	}
}

void Core::ResourceMetadata::p_asyncLoad(const Core::ResourceHandle& handle, std::function<bool(std::pmr::vector<uint8_t>)> decryptionFunc) {
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		m_type = handle.type;
		m_flag.setFlag(handle.flag);
	}
	std::future<void> future = std::async(std::launch::async, [this, handle, decryptionFunc]() {
		std::ifstream file(s_FILE_NAME, std::ios::binary);
		if(!file.is_open()) {XN_LOG_ERR("Resourcepack failed to open."); return;}
		file.seekg(handle.offset);
		if(!file) {XN_LOG_ERR("Seeking the correct offset in Resourcepack failed."); return;}
		
		const std::lock_guard<std::mutex> lock(m_mutex); 
		m_data.reserve(static_cast<size_t>(handle.size));
		file.read(reinterpret_cast<char*>(m_data.data()), handle.size);
		if(!file) {XN_LOG_ERR("Reading the data from Resourcepack failed."); return;}
		if (m_flag.isEncrypted()) {
			if(!decryptionFunc(m_data)) {XN_LOG_ERR("Decryption of an asset failed"); return;}
			m_flag.resetEncrypted();
		}
	});
	const std::lock_guard<std::mutex> lock(sm_futuresMutex);
	sm_futures.push_back(std::move(future));
}

void Core::ResourceMetadata::p_asyncUnload() {
	//I know this isn't async but i don't think it needs to be (for now at least)
	const std::lock_guard<std::mutex> lock(m_mutex);
	m_data.clear();
	m_data.shrink_to_fit();
	m_flag.setFlag(0);
}
