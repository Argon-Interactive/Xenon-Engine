#include "application.hpp"
#include "appData.hpp"
#include "devTools/logger/logger_core.hpp"
#include <functional>

namespace Xenon
{

	Application::Application() {
		XN_LOG_TO_FILE("Xenon-log");
		Core::AppData::init(std::bind(&Application::pushEvent, this, std::placeholders::_1));
		XN_LOG_TRC("Application: created");
	}

	Application::~Application() {
		Core::AppData::terminate();
		XN_LOG_TRC("Application: destroyed");
	}

	int Application::run() {
		while (m_running) {
			// aplikacja dziaua
			handleEvents();
			Core::AppData::getWindow().FEP();
		}
		return 0;
	}

	void Application::handleEvents() {
		while(!emptyEventQueue()) {
			Event e = popEvent();
			switch (e.getType()) {
				case Event::Type::WINDOW_CLOSE:
					m_running = false;
				break;
				default:
					XN_LOG_ERR("Unknown event: " + e.getName());
			}
		}
	}

	void Application::pushEvent(const Event& event) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_eventQueue.push(event);
		m_cond.notify_one();
	}

	Event Application::popEvent() {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this] { return !m_eventQueue.empty(); } );
		Event e = m_eventQueue.front();
		m_eventQueue.pop();
		return e;
	}

	bool Application::emptyEventQueue() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_eventQueue.empty();
    }

}
