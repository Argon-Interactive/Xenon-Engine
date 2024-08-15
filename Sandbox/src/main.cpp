#define XENON_DEBUG //This macro makes Xenon run the application in debug mode

#include "xenon.h"

// void config([[maybe_unused]]void* something) {
// 	XN_LOG_BR(XNTools::Logger::logMode::info);
// 	XN_LOG_DEB("Helo, I am under the water");
// }

const Xenon::AppConfig config{
	800, 600, "Sandbox" 
};

const Xenon::AppConfig& getConfig() {
	return config;
}
