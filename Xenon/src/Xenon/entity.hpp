#ifndef _XENON_XENON_ENTITY_
#define _XENON_XENON_ENTITY_

#include "api.h"

#include <cstdint>

namespace Xenon {

class XAPI Entity {
public:
	explicit Entity(uint64_t uuid);

	//addComponent();
	
	//removeComponent();
	
	//getComponent();
	
private:
	uint64_t m_uuid;

	bool m_enabled = true;
	bool m_exists = true;

};



}

#endif // !_XENON_XENON_ENTITY_
