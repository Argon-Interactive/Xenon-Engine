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

enum class ResourceType : uint32_t {
	Scene, 
};

struct ResourceFlag {

	void inline setFlag(uint32_t flag) { m_flag = flag; }

	[[nodiscard]] bool isValid();

	[[nodiscard]] bool isCompressed();
	void setCompressed();
	void resetCompressed();

	[[nodiscard]] bool isEncrypted();
	void setEncrypted();
	void resetEncrypted();

private:
	/* flag bits
	* 0 - valid
	* 1 - compressed
	* 2 - encrypted
	*/
	uint32_t m_flag;
	std::mutex m_mutex;

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
public:
	ResourceMetadata() = default;
	explicit ResourceMetadata(std::pmr::memory_resource* memRes) : m_data(memRes) {}
	~ResourceMetadata();
	ResourceMetadata(ResourceMetadata&&) = delete;
	ResourceMetadata(const ResourceMetadata&) = delete;
	ResourceMetadata& operator=(ResourceMetadata&&) = delete;
	ResourceMetadata& operator=(const ResourceMetadata&) = delete;

	void changePMR(std::pmr::memory_resource* newMemRes);

	std::optional<std::future<void>> load(const Core::ResourceHandle& handle, std::function<bool(std::pmr::vector<uint8_t>&)> decryptionFunc);
	void unload();

	[[nodiscard]] uint8_t* getRawData();
	[[nodiscard]] size_t getSize();
	[[nodiscard]] ResourceFlag* getFlag();

private:
	ResourceType m_type{};
	ResourceFlag m_flag{};
	uint64_t m_referenceCounter{};
	std::mutex m_mutex;
	std::pmr::vector<uint8_t> m_data;

	std::future<void> p_asyncLoad(const Core::ResourceHandle& handle, std::function<bool(std::pmr::vector<uint8_t>&)> decryptionFunc);
};
}

#endif // !_XNENON_CORE_RESOURCESMANAGMENT_RESOURCE_
