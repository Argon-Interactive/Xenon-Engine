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
	m_metadatas = std::vector<ResourceMetadata>(static_cast<size_t>(rph.resourcesAmount));
	for (auto& rm : m_metadatas) {
		rm.changePMR(m_memoryResource); //This is nessesary (It will work but badly)
	}
	file.read(reinterpret_cast<char*>(m_handles.data()), static_cast<std::streamsize>(rph.resourcesAmount * sizeof(ResourceHandle)));
	file.close();
}

Core::ResourceManager::~ResourceManager() {
	sync();
}
//TODO: This should make sure to use maximum of 256 threads at one time
void Core::ResourceManager::load(const ResourceID* IDs, size_t amount) {
	for(size_t i = 0; i < amount; ++i) { 
		const ResourceID id = IDs[i];
		auto opt = m_metadatas[id].load(m_handles[id], p_decryption);
		if (opt.has_value()) {
			m_futuresMap[id] = std::move(opt.value());
		}
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
		auto opt = m_metadatas[id].load(m_handles[id], p_decryption);
		if (opt.has_value()) {
			m_futuresMap[id] = std::move(opt.value());
		}
	}
}

void Core::ResourceManager::free(const std::vector<ResourceID>& IDs) {
	for (auto id : IDs) {
		m_metadatas[id].unload();
	}
}

void Core::ResourceManager::load(const ResourceID ID) {
	auto opt = m_metadatas[ID].load(m_handles[ID], p_decryption);
	if (opt.has_value()) {
		m_futuresMap[ID] = std::move(opt.value());
	}
}

void Core::ResourceManager::free(const ResourceID ID) {
	m_metadatas[ID].unload();
}

void Core::ResourceManager::sync() {
	const std::lock_guard<std::mutex> lock(m_futuresMutex);
	for (auto& future : m_futuresMap) {
		future.second.wait();
	}
	m_futuresMap.clear();
}

std::optional<Core::ResourceManager::Resource> Core::ResourceManager::getResource(ResourceID ID) {
	using namespace std::chrono_literals;
	ResourceMetadata& metadata = m_metadatas[ID];
	auto result = m_futuresMap.at(ID).wait_for(0s);
	switch (result) {
	case std::future_status::ready:
		XN_LOG_DEB("ready");
		break;
	case std::future_status::deferred:
		XN_LOG_DEB("deferred");
		break;
	case std::future_status::timeout:
		XN_LOG_DEB("timeout");
		break;
	}
	if (result == std::future_status::ready) {
		m_futuresMap.erase(ID);
		return Resource(metadata.getRawData(), metadata.getSize());
	}
	return std::nullopt;
}

Core::ResourceManager::Resource Core::ResourceManager::getSyncedResource(ResourceID ID) {
	ResourceMetadata& metadata = m_metadatas[ID];
	m_futuresMap.at(ID).wait();
	m_futuresMap.erase(ID);
	return { metadata.getRawData(), metadata.getSize() };
}

bool Core::ResourceManager::p_versionCheck([[maybe_unused]] int64_t version) {
	return true;
}

bool Core::ResourceManager::p_decryption([[maybe_unused]] std::pmr::vector<uint8_t>& data) {
	return true;
}
