#include "profiler.hpp"
#include "devTools/logger_core.hpp"

void XNTools::Profiler::log() {
	for(auto& entry : m_timesMap) {
		XN_LOG_DEB("{q} - {0}", entry.first, entry.second);
	}
}

[[nodiscard]] XNTools::Profiler& XNTools::Profiler::getInstance() {
	static Profiler prof;
	return prof;
}
