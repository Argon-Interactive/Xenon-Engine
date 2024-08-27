#ifndef _XENON_XENON_SCENE_
#define _XENON_XENON_SCENE_

#include "api.h"
#include "Xenon/entity.hpp"

#include <cstdint>

namespace Core { class Scene; }

namespace Xenon {

class XAPI Scene {
public:
	explicit Scene(Core::Scene* ptr);

	Entity createEntity();

	uint64_t getBuildIndex();

private:
	Core::Scene* m_impl;

};

}

#endif // _XENON_XENON_SCENE_
