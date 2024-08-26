#ifndef _XENON_CORE_RESOURCESMANAGMENT_RESOURCEMANAGER_
#define _XENON_CORE_RESOURCESMANAGMENT_RESOURCEMANAGER_

#include "resource.hpp"

#include <filesystem>

namespace Core {
class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager &) = delete;
	ResourceManager(ResourceManager &&)  noexcept = delete;
	ResourceManager &operator=(const ResourceManager &) = delete;
	ResourceManager &operator=(ResourceManager &&)  noexcept = delete;

	void ensureLoaded(const ResourceID* IDs, size_t amount); 
	void free(const ResourceID* IDs, size_t amount);
	void ensureLoaded(const std::vector<ResourceID>& IDs); 
	void free(const std::vector<ResourceID>& IDs);


private:
	std::vector<Core::ResourceHandle> m_handles;
	std::vector<Core::ResourceMetadata> m_metadatas;
	std::filesystem::path m_rpPath;

	bool static p_versionCheck();
	bool static p_decryption(uint8_t* data, uint64_t dataSize);
};
}

#endif // !_XENON_CORE_RESOURCESMANAGMENT_RESOURCEMANAGER_
