#ifndef _XENON_XENON_APPCONFIG_
#define _XENON_XENON_APPCONFIG_

#include "api.h"

#include <cstddef>
#include <cstdint>

namespace Xenon {

class Scene;

using BuildFunction = void (*)(Xenon::Scene *);

struct XAPI BuildFunctions {
	const BuildFunction* functions;
	size_t size;

	inline BuildFunction operator[](size_t index) const {
		return functions[index];
	}
};

struct XAPI AppConfig {
	uint32_t defaultWindowWidth;
	uint32_t defaultWindowHeight;
	const char* defaultWindowName;
	BuildFunctions buildFunctions;
};


}

#endif // !_XENON_XENON_APPCONFIG_
