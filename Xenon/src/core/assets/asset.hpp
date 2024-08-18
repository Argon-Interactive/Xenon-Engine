#ifndef _XENON_CORE_ASSETS_ASSET_
#define _XENON_CORE_ASSETS_ASSET_

#include <cstdint>
#include <ios>
namespace Core {
using assetID = uint32_t;

struct AssetHandle {
	AssetHandle() = default;
	~AssetHandle() = default;
	AssetHandle(AssetHandle &&) = default;
	AssetHandle(const AssetHandle &) = default;
	AssetHandle &operator=(AssetHandle &&) = default;
	AssetHandle &operator=(const AssetHandle &) = default;
private:
	std::streampos offset;
	std::streamsize size{};
	uint64_t flag{}; //yeah this is a massive flag but bcoz of padding it may as well be
friend struct AssetMetadata;
};

struct AssetMetadata {
	AssetMetadata() = default;
	~AssetMetadata() = default;
	AssetMetadata(AssetMetadata &&) = default;
	AssetMetadata(const AssetMetadata &) = delete;
	AssetMetadata &operator=(AssetMetadata &&) = default;
	AssetMetadata &operator=(const AssetMetadata &) = delete;

	bool load(std::ifstream& file, const AssetHandle& assetHandle);
	void unload();
	[[nodiscard]] uint8_t* getData() const;
private:
	uint8_t* m_data{};
	uint64_t m_flag{};
	uint64_t m_counter{};
};
}

#endif // !_XENON_CORE_ASSETS_ASSET_
