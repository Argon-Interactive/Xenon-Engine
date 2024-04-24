#ifndef _XENON_ENTRYPOINT_H_
#define _XENON_ENTRYPOINT_H_

#include "application.h"
#include <memory>

Xenon::Application* Xenon::create();

int main(int argc, char** argv) {
	std::unique_ptr<Xenon::Application> game(Xenon::create());
	int res = game->run();
	return res;
}

#endif // !_XENON_ENTRYPOINT_H_
