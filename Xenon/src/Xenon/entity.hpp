#ifndef _XENON_XENON_ENTITY_
#define _XENON_XENON_ENTITY_

#include "api.h"

#include <cstdint>

namespace Xenon {

class XAPI Entity {
public:
	Entity() = default;

	//addComponent();
	
	//removeComponent();
	
	//getComponent();
	
private:
	uint64_t m_uuid;

	friend class Scene;
};



}

#endif // !_XENON_XENON_ENTITY_
