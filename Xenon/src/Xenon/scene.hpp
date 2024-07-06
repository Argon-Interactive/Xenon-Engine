#ifndef _XENON_XENON_SCENE_
#define _XENON_XENON_SCENE_

#include "api.h"

#include <cstdint>

namespace Core { class Scene; }

namespace Xenon {

// class SceneManager;

class XAPI Scene {
public:
	explicit Scene(Core::Scene* ptr);

	void unload();

	uint64_t createEntity();
	void deleteEntity(uint64_t uuid);

	void setBuildIndex(int64_t index);
	int64_t getBuildIndex();

private:
	Core::Scene* m_impl;

};

}

#endif // _XENON_XENON_SCENE_
