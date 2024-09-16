#ifndef _XENON_XENON_ENTRYPOINT_
#define _XENON_XENON_ENTRYPOINT_

#include "appConfig.hpp"
#include "application.hpp"
#include "logger.hpp"

const Xenon::AppConfig& getConfig();

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	XNTools::Logger::getInstance().setName("[CLIENT]");
	Xenon::Application game(getConfig());
	int res = game.run();
	return res;
}

#endif // !_XENON_XENON_ENTRYPOINT_
