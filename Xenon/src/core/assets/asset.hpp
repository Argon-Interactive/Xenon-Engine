#ifndef _XENON_CORE_ASSETS_ASSET_
#define _XENON_CORE_ASSETS_ASSET_

#include <cstdint>
#include <ios>
namespace Core {
struct Asset {
	using AssetID = uint64_t;

	Asset();
	~Asset();
	Asset(Asset &&) = default;
	Asset(const Asset &) = default;
	Asset &operator=(Asset &&) = default;
	Asset &operator=(const Asset &) = default;

	void load();
	void unload();
	
	AssetID ID;
	char* data;
	std::streamoff offset;
	std::size_t size;
	bool isloaded;
};
}

#endif // !_XENON_CORE_ASSETS_ASSET_
