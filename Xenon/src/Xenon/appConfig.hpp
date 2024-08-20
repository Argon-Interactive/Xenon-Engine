#ifndef _XENON_XENON_APPCONFIG_
#define _XENON_XENON_APPCONFIG_

#include "api.h"

#include <cstdint>

namespace Xenon {

struct XAPI AppConfig {
	uint32_t defaultWindowWidth;
	uint32_t defaultWindowHeight;
	const char* defaultWindowName;
};


}

#endif // !_XENON_XENON_APPCONFIG_
