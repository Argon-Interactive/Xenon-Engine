#ifndef _XNENON_CORE_RESOURCESMANAGMENT_RESOURCE_
#define _XNENON_CORE_RESOURCESMANAGMENT_RESOURCE_

#include <cstdint>
#include <filesystem>
#include <future>
#include <ios>
#include <vector>
#include <memory_resource>
#include <functional>
#include <mutex>
#include <list>

namespace Core {

using ResourceID = uint64_t;

enum ResourceType : uint32_t {
	Scene, 
};

struct Resource {
	Resource(uint8_t* d, size_t s) : data(d), size(s) {}

	uint8_t* data;
	size_t size;
};

struct ResourceFlag {

	void setFlag(uint32_t flag) { m_flag = flag; }

	[[nodiscard]] bool isValid() { return p_getBit(1); }

	[[nodiscard]] bool isCompressed() { return p_getBit(2); }
	void setCompressed() { p_setBit(2); p_resetBit(1); }
	void resetCompressed() { p_resetBit(2); if (!isEncrypted()) p_setBit(1); }

	[[nodiscard]] bool isEncrypted() { return p_getBit(3); }
	void setEncrypted() { p_setBit(3); p_resetBit(1); }
	void resetEncrypted() { p_resetBit(3); if (!isCompressed()) p_setBit(1); }

private:
	/* flag bits
	* 1st - valid
	* 2nd - compressed
	* 3rd - encrypted
	*/
	uint32_t m_flag;

	[[nodiscard]] bool p_getBit(uint8_t bitmask) const { return static_cast<bool>(m_flag & (1u << bitmask)); }
	void p_setBit(uint8_t bitmask) { m_flag |= (1u << bitmask); }
	void p_resetBit(uint8_t bitmask) { m_flag &= ~(1u << bitmask); }
};

struct ResourceHandle {
	std::streampos offset;
	std::streamsize size{};
	ResourceType type{};
	uint32_t flag{};
};

class ResourceMetadata {
	static constexpr const char* s_FILE_NAME = "gamedata.rp";
public:
	explicit ResourceMetadata(std::pmr::memory_resource* memRes) : m_data(memRes) {}
	~ResourceMetadata();
	ResourceMetadata(ResourceMetadata&&) = delete;
	ResourceMetadata(const ResourceMetadata&) = delete;
	ResourceMetadata& operator=(ResourceMetadata&&) = delete;
	ResourceMetadata& operator=(const ResourceMetadata&) = delete;

	void load(const Core::ResourceHandle& handle, const std::filesystem::path& path, std::function<bool(std::pmr::vector<uint8_t>)>& decryptionFunc);
	void unload();

	[[nodiscard]] uint8_t* getRawData();
	[[nodiscard]] size_t getSize();

	static void sync();

private:
	static std::list<std::future<void>> sm_futures;
	static std::mutex sm_futuresMutex;

	ResourceType m_type{};
	ResourceFlag m_flag{};
	uint64_t m_referenceCounter{};
	std::mutex m_mutex;
	std::pmr::vector<uint8_t> m_data;

	void p_asyncLoad(const Core::ResourceHandle& handle, std::function<bool(std::pmr::vector<uint8_t>)> decryptionFunc);
	void p_asyncUnload();
};
}

#endif // !_XNENON_CORE_RESOURCESMANAGMENT_RESOURCE_
