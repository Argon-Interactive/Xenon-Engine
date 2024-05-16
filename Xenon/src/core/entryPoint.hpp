#ifndef _XENON_ENTRYPOINT_
#define _XENON_ENTRYPOINT_

#include "application.hpp"
#include "devTools/logger/logger_core.hpp"
#include <memory>

Xenon::Application* Xenon::create();

int main(int argc, char** argv) {
	XN_LOG_TO_FILE("Xenon_log");
	Xenon::Logger::getInstance().setFilePath("testing");
	std::unique_ptr<Xenon::Application> game(Xenon::create());
	int res = game->run();
	return res;
}

#endif // !_XENON_ENTRYPOINT_
