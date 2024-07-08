#include "systemTest.hpp"
#include "ComponentManager.hpp"
#include "devTools/logger_core.hpp"

void Core::Systems::systemTest([[maybe_unused]] ComponentManager *cmgr,[[maybe_unused]] testMetadata& metadata, int &comp) {
	XN_LOG_TRC(comp);
}
