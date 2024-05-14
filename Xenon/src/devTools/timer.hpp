#ifndef _XENON_DEVTOOLS_TIMER_
#define _XENON_DEVTOOLS_TIMER_

#include<chrono>

#ifdef XENON
	#if defined __DEBUG__ || __RELEASE__
		#define XN_TIMER(name) anl::Timer name
		#define XN_TIMER_START(name) name.start()
		#define XN_TIMER_GET(name, Units) name.get<Units>()
	#else 
		#define XN_TIMER(name)
		#define XN_TIMER_START(name)
		#define XN_TIMER_GET(name, Units) 0
	#endif
#endif

#define XN_MILLISECOND std::chrono::microseconds
#define XN_SECOND std::chrono::seconds
#define XN_MINUTE std::chrono::minutes
#define XN_HOUR std::chrono::hours

namespace Xenon {
	class Timer
	{
	public:
		Timer() :m_start(std::chrono::high_resolution_clock::now()){}
		~Timer() = default;
		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;
		template<typename Units = typename std::chrono::high_resolution_clock::duration>
		[[nodiscard]] float get()
		{ return static_cast<float>(std::chrono::duration_cast<Units>(std::chrono::high_resolution_clock::now() - m_start).count()); }
		void start() { m_start = std::chrono::high_resolution_clock::now(); }
	private:
		std::chrono::high_resolution_clock::time_point m_start;
	};
}

#endif // !_XENON_DEVTOOLS_TIMER_
