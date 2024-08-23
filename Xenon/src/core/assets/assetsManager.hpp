#ifndef _XENON_CORE_ASSETS_ASSETSMANAGER_
#define _XENON_CORE_ASSETS_ASSETSMANAGER_

#include "asset.hpp"
#include <fstream>

namespace Core {
class Scene;

class AssetsManager {
public:
	AssetsManager() = default;
	~AssetsManager() = default;
	AssetsManager(const AssetsManager &) = delete;
	AssetsManager(AssetsManager &&)  noexcept = delete;
  	AssetsManager &operator=(const AssetsManager &) = delete;
  	AssetsManager &operator=(AssetsManager &&)  noexcept = delete;

	void init();
	void terminate();
	[[nodiscard]] uint8_t* getAssetData(assetID id) const;
	[[nodiscard]] uint64_t getAssetSize(assetID id) const;
	void loadScene(Scene* scene, assetID sceneID);
	void unloadScene(assetID sceneID);
	void ensureLoaded(const assetID* assetIDs, uint64_t assetsAmmount); //This functions are for all assets excluding scenes
	void freeAssets(const assetID* assetIDs, uint64_t assetsAmmount);
	[[nodiscard]] std::ifstream* getFile();

private:
	std::unique_ptr<AssetHandle[]> m_assetHandles;
	std::unique_ptr<AssetMetadata[]> m_assetsMetadata;
	std::ifstream m_file;

	bool static p_versionCheck();
	bool static p_decryption(uint8_t* data, uint64_t dataSize);
};
}

#endif // !_XENON_CORE_ASSETS_ASSETSMANAGER_
