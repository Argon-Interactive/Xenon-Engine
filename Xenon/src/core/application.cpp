#include "application.h"
#include "appData.h"
#include "logger.hpp"

namespace Xenon
{

	Application::Application() {
		Core::AppData::init();
		XN_LOG_INF("Application: created");
	}

	Application::~Application() {
		Core::AppData::terminate();
		XN_LOG_INF("Application: destroyed");
	}

	int Application::run() {
		while (!Core::AppData::getWindow().closeCallBack()) {
			// aplikacja dziaua
			Core::AppData::getWindow().FEP();
		}
		return 0;
	}

	void Application::pushEvent(const Event& event) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(event);
		m_cond.notify_one();
	}

	Event Application::popEvent() {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this] { return !m_queue.empty(); } );
		Event e = m_queue.front();
		m_queue.pop();
		return e;
	}

}
