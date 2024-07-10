#ifndef _XENON_XENON_APPCONFIG_
#define _XENON_XENON_APPCONFIG_

#include <functional>

namespace Xenon {

class Scene;

struct AppConfig {
	std::function<void(void*)> configFunction;
	std::vector<std::function<void(Xenon::Scene*)>> buildFunctions;
};


}

#endif // !_XENON_XENON_APPCONFIG_
