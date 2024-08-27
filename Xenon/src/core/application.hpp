#ifndef _XENON_CORE_APPLICATION_
#define _XENON_CORE_APPLICATION_

#include "Xenon/appConfig.hpp"
#include "event.hpp"

#include <queue>
#include <mutex>
#include <condition_variable>

namespace Xenon { class Application; }

namespace Core {

class Application {
public:
	explicit Application(const Xenon::AppConfig& config);
	~Application();
	Application(const Application&) = delete;
	Application(Application&&) noexcept = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void pushEvent(const Core::Event& event);

private:
	int run();
	void handleEvents();
	void update(double deltaT);
	void fixedUpdate();
	void render();

	bool m_running = true;

	std::queue<Core::Event> m_eventQueue;
	mutable std::mutex m_mutex;
	std::condition_variable m_cond;

	friend class Xenon::Application;
};

}

#endif // !_XENON_CORE_APPLICATION_
