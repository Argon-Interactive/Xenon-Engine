#ifndef _XENON_APPLICATION_
#define _XENON_APPLICATION_

#include "api.h"
#include "core/input/input.hpp"
#include "event/event.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

extern int main(int argc, char** argv);

namespace Xenon
{
	class XAPI Application
	{
	public:
		Application();
		virtual ~Application();
		Application(const Application&) = delete;
		Application(Application&&) noexcept = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		void pushEvent(const Core::Event& event) noexcept;

	private:
		int run();
		void handleEvents();
		void render();
		void update();

		bool m_running = true;

		Core::Event popEvent() noexcept;
		bool emptyEventQueue() const noexcept;

		std::queue<Core::Event> m_eventQueue;
		mutable std::mutex m_mutex;
		std::condition_variable m_cond;

		friend int ::main(int argc, char** argv);
	};

}

#endif // !_XENON_APPLICATION_
