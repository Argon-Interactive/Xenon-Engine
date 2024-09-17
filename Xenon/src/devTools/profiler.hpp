#ifndef _XENON_DEVTOOLS_PROFILER_
#define _XENON_DEVTOOLS_PROFILER_

#include "devTools/logger_core.hpp"
#include <chrono>
#include <map>

#define XN_PROFILE(func, category) XNTools::Profiler::getInstance().profile(category, [&](){ return func; })

namespace XNTools {

class Profiler {
public:
	~Profiler() = default;
	Profiler(Profiler &&) = delete;
	Profiler(const Profiler &) = delete;
	Profiler &operator=(Profiler &&) = delete;
	Profiler &operator=(const Profiler &) = delete;

	template<typename F, typename... Args>
	auto profile(std::string category, F func, Args&&... args) 
	-> typename std::enable_if<!std::is_void<decltype(func(std::forward<Args>(args)...))>::value, decltype(func(std::forward<Args>(args)...))>::type {
		auto start = std::chrono::high_resolution_clock::now();
		auto result = func(std::forward<Args>(args)...);
		auto end = std::chrono::high_resolution_clock::now();
		m_timesMap[category] = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
		return result;
	}

	template<typename F, typename... Args>
	auto profile(std::string category, F func, Args&&... args) 
	-> typename std::enable_if<std::is_void<decltype(func(std::forward<Args>(args)...))>::value, void>::type {
		auto start = std::chrono::high_resolution_clock::now();
		func(std::forward<Args>(args)...);
		auto end = std::chrono::high_resolution_clock::now();
		m_timesMap[category] = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
	}

	inline void log() {
		for(auto& entry : m_timesMap) {
			XN_LOG_DEB("{q} - {0}", entry.first, entry.second);
		}
	}

	[[nodiscard]] static inline Profiler& getInstance() {
		static Profiler prof;
		return prof;
	}
private:
	Profiler();
	std::map<std::string, uint64_t> m_timesMap;
};
}

#endif // !_XENON_DEVTOOLS_PROFILER_
