#ifndef _XENON_CORE_ASSETS_ASSETSMANAGER_
#define _XENON_CORE_ASSETS_ASSETSMANAGER_

#include "asset.hpp"
#include <fstream>

namespace Core {

class AssetsManager {
public:
	AssetsManager();
	~AssetsManager() = default;
	AssetsManager(const AssetsManager &) = delete;
	AssetsManager(AssetsManager &&)  noexcept = delete;
  	AssetsManager &operator=(const AssetsManager &) = delete;
  	AssetsManager &operator=(AssetsManager &&)  noexcept = delete;

	void init();
	void terminate();
	[[nodiscard]] uint8_t* getAssetData(assetID id) const;
	void ensureLoaded(const assetID* assetIDs, uint64_t assetsAmmount);
	void freeAssets(const assetID* assetIDs, uint64_t assetsAmmount);

private:
	AssetHandle* m_assetHandles;
	AssetMetadata* m_assetsMetadata;
	std::ifstream m_file;
};
}

#endif // !_XENON_CORE_ASSETS_ASSETSMANAGER_
