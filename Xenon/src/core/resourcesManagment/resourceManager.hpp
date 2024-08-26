#ifndef _XENON_CORE_RESOURCESMANAGMENT_RESOURCEMANAGER_
#define _XENON_CORE_RESOURCESMANAGMENT_RESOURCEMANAGER_

#include "resource.hpp"

#include <memory_resource>
#include <filesystem>

namespace Core {
class ResourceManager {
public:
	ResourceManager(std::filesystem::path path);
	~ResourceManager();
	ResourceManager(const ResourceManager &) = delete;
	ResourceManager(ResourceManager &&)  noexcept = delete;
	ResourceManager &operator=(const ResourceManager &) = delete;
	ResourceManager &operator=(ResourceManager &&)  noexcept = delete;

	struct Resource {
	private:
		inline Resource(uint8_t* d, size_t s, bool l) : data(d), size(s), isLoaded(l) {}
	public:
		uint8_t* data;
		size_t size;
		bool isLoaded;
		friend class Core::ResourceManager;
	};


	void load(const ResourceID* IDs, size_t amount); 
	void loadAsync(const ResourceID* IDs, size_t amount);
	void free(const ResourceID* IDs, size_t amount);
	void load(const std::vector<ResourceID>& IDs); 
	void loadAsync(const std::vector<ResourceID>& IDs); 
	void free(const std::vector<ResourceID>& IDs);
	void load(const ResourceID ID); 
	void loadAsync(const ResourceID ID); 
	void free(const ResourceID ID);

	void sync();

	[[nodiscard]] inline const std::filesystem::path* getFilepathPtr() const { return &m_rpPath; }
	[[nodiscard]] Resource getResource(ResourceID ID);

private:
	std::vector<Core::ResourceHandle> m_handles;
	std::vector<Core::ResourceMetadata> m_metadatas;
	std::list<std::future<void>> m_futureList;
	const std::filesystem::path m_rpPath;
	std::mutex m_futuresMutex;
	//temp
	std::pmr::memory_resource* m_memoryResource = std::pmr::get_default_resource();

	bool static p_versionCheck(int64_t version);
	bool static p_decryption(std::pmr::vector<uint8_t>& data);
};
}

#endif // !_XENON_CORE_RESOURCESMANAGMENT_RESOURCEMANAGER_
