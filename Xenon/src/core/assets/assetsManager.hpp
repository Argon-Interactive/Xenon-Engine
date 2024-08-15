#ifndef _XENON_CORE_ASSETS_ASSETSMANAGER_
#define _XENON_CORE_ASSETS_ASSETSMANAGER_

#include "asset.hpp"
#include <fstream>
#include <vector>

namespace Core {

class AssetsManager {
public:
	AssetsManager();
	~AssetsManager();
	AssetsManager(const AssetsManager &) = delete;
	AssetsManager(AssetsManager &&)  noexcept = delete;
  	AssetsManager &operator=(const AssetsManager &) = delete;
  	AssetsManager &operator=(AssetsManager &&)  noexcept = delete;

	void init();
	void terminate();
	[[nodiscard]] char* getAssetData() const;
	void ensureLoaded(const std::vector<Asset::AssetID>& assetIDs);
	void ensureLoaded(const Asset::AssetID* assetIDs, uint64_t assetsAmmount);
	void freeAssets(const std::vector<Asset::AssetID>& assetIDs);
	void freeAssets(const Asset::AssetID* assetIDs, uint64_t assetsAmmount);

private:
	Asset* m_assets;
	std::ifstream m_file;
};
}

#endif // !_XENON_CORE_ASSETS_ASSETSMANAGER_
