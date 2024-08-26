#include "resourceManager.hpp"
#include "devTools/logger_core.hpp"

Core::ResourceManager::ResourceManager(std::filesystem::path path) : m_rpPath(std::move(path)) {

}

Core::ResourceManager::~ResourceManager() {
	sync();
}

void Core::ResourceManager::load(const ResourceID* IDs, size_t amount) {
	std::list<std::future<void>> list;
	for(size_t i = 0; i < amount; ++i) { 
		const ResourceID id = IDs[i];
		m_metadatas[id].load(m_handles[id], list, p_decryption);
	}
	for (auto& future : list) {
		future.wait();
	}
}

void Core::ResourceManager::loadAsync(const ResourceID* IDs, size_t amount) {
	for(size_t i = 0; i < amount; ++i) { 
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		const ResourceID id = IDs[i];
		m_metadatas[id].load(m_handles[id], m_futureList, p_decryption);
	}
}

void Core::ResourceManager::free(const ResourceID* IDs, size_t amount) {
	for(size_t i = 0; i < amount; ++i) { 
		const ResourceID id = IDs[i];
		m_metadatas[id].unload();
	}
}

void Core::ResourceManager::load(const std::vector<ResourceID>& IDs) {
	std::list<std::future<void>> list;
	for (auto id : IDs) {
		m_metadatas[id].load(m_handles[id], list, p_decryption);
	}
	for (auto& future : list) {
		future.wait();
	}
}

void Core::ResourceManager::loadAsync(const std::vector<ResourceID>& IDs) {
	for (auto id : IDs) {
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_metadatas[id].load(m_handles[id], m_futureList, p_decryption);
	}
}

void Core::ResourceManager::free(const std::vector<ResourceID>& IDs) {
	for (auto id : IDs) {
		m_metadatas[id].unload();
	}
}

void Core::ResourceManager::load(const ResourceID ID) {
	std::list<std::future<void>> list;
	m_metadatas[ID].load(m_handles[ID], list, p_decryption);
	//I know this is stupid but oh well, one heap allocation that could be on the stack will not be a bottleneck when loading assets
	for (auto& future : list) {
		future.wait();
	}
}

void Core::ResourceManager::loadAsync(const ResourceID ID) {
	const std::lock_guard<std::mutex> lock(m_futuresMutex);
	m_metadatas[ID].load(m_handles[ID], m_futureList, p_decryption);
}


void Core::ResourceManager::free(const ResourceID ID) {
	m_metadatas[ID].unload();
}

void Core::ResourceManager::sync() {
	const std::lock_guard<std::mutex> lock(m_futuresMutex);
	for (auto& future : m_futureList) {
		future.wait();
	}
}

Core::ResourceManager::Resource Core::ResourceManager::getResource(ResourceID ID) {
	ResourceMetadata& metadata = m_metadatas[ID];
	if (metadata.getFlag().isValid()) {
		return { metadata.getRawData(), metadata.getSize(), true };
	}
	return { nullptr, 0, false };
}

bool Core::ResourceManager::p_versionCheck([[maybe_unused]] int64_t version) {
	return true;
}

bool Core::ResourceManager::p_decryption([[maybe_unused]] std::pmr::vector<uint8_t>& data) {
	return true;
}
