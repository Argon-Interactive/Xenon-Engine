#include "resourceManager.hpp"
#include "devTools/logger_core.hpp"

#include <fstream>
#include <cassert>

Core::ResourceManager::ResourceManager(std::filesystem::path path) : m_rpPath(path) {
	std::ifstream file(m_rpPath, std::ios::binary);
	//TODO: This should be an elegant exception and not "this"
	if(!file.is_open()) {XN_LOG_ERR("{q} cannot be opened.", m_rpPath.string()); exit(1); }
	struct ResourcePackHeader {
		char header[4];
		int64_t version;
		uint64_t resourcesAmount;
	};
	ResourcePackHeader rph{};
	file.read(reinterpret_cast<char*>(&rph), sizeof(ResourcePackHeader));
	if(rph.header[0] != 'X' || rph.header[1] != 'e' || rph.header[2] != 'R' || rph.header[3] != 'P') {XN_LOG_ERR("The Resource Pack {q} is not valid", m_rpPath.string()); exit(1);}
	if(!p_versionCheck(rph.version)) {XN_LOG_ERR("The Resource Packs {q} version is no longer supported", m_rpPath.string()); exit(1);}
	m_handles.resize(static_cast<size_t>(rph.resourcesAmount));
	m_rawData = operator new[](static_cast<size_t>(rph.resourcesAmount) * sizeof(ResourceMetadata));
	m_metadatas = static_cast<Core::ResourceMetadata*>(m_rawData);
	for (uint64_t i = 0; i < rph.resourcesAmount; ++i) {
		new (&m_metadatas[i]) ResourceMetadata(m_memoryResource);
	}
	file.read(reinterpret_cast<char*>(m_handles.data()), static_cast<std::streamsize>(rph.resourcesAmount * sizeof(ResourceHandle)));
	file.close();
}

Core::ResourceManager::~ResourceManager() {
	sync();
	delete[] m_rawData;
}
//TODO: This should make sure to use maximum of 256 threads at one time
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
	//TODO: This should wait untill this is loaded.
	ResourceMetadata& metadata = m_metadatas[ID];
	if (metadata.getFlag()->isValid()) {
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
