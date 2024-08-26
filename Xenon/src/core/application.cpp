#include "application.hpp"
#include "appData.hpp"
#include "devTools/logger_core.hpp"
#include "input/input.hpp"
#include <exception>

namespace Core {

Application::Application(const Xenon::AppConfig& config) {
	XN_LOG_TO_FILE("Xenon-log");
	Core::AppData::init([this](auto && a) { pushEvent(std::forward<decltype(a)>(a)); }, config); // what the fuck?
	XN_LOG_TRC("Application: created");
	//TEST: we need to write testing infrastructure
	AppData::getResourceManager().loadAsync({ 0, 1, 2 });
	std::string str;
	auto res0 = AppData::getResourceManager().getResource(0);
	auto res1 = AppData::getResourceManager().getResource(1);
	auto res2 = AppData::getResourceManager().getResource(2);
	XN_LOG_DEB(res0.data);
	XN_LOG_DEB(*(reinterpret_cast<int*>(res1.data)));
	XN_LOG_DEB(*(reinterpret_cast<float*>(res2.data)));
}

Application::~Application() {
	try {
		Core::AppData::terminate();
		XN_LOG_TRC("Application: destroyed");
	} catch (std::exception& e) {
		try {
			std::cout << "something went terribly fucking wrong: " << e.what();
		} catch (std::exception& e2) { 
			exit(-1); // This is never gonna happen basically
		}
	}
}

int Application::run() {
	try {
		while (m_running) {
			update(1);
			fixedUpdate();
			render();
			handleEvents();
			Core::AppData::getWindow().FEP();
		}
	} catch (std::exception& e) {
		try {
			XN_LOG_ERR("Exception in main application loop: {0}", e.what());
			return 1;
		} catch (std::exception&) {
			return 2;
		}
	}
	return 0;
}

void Application::render() {

}

void Application::update([[maybe_unused]] double deltaT) {

}

void Application::fixedUpdate() {

}

void Application::handleEvents() {
	while(!emptyEventQueue()) {
		const Core::Event e = popEvent();
		switch (e.getType()) {
			case Core::Event::Type::WINDOW_CLOSE:
				XN_LOG_INF("Window close");
				m_running = false;
				break;
			case Core::Event::Type::WINDOW_RESIZE:
				XN_LOG_INF("Window resize: (width = {0}, height = {0})", e.getArg().argInt.first, e.getArg().argInt.second);
				break;
			case Core::Event::Type::KEY_PRESSED:
				Input::proccesEvents(Input::Action::Press, static_cast<int>(e.getArg().argUllong));
				break;
			case Core::Event::Type::KEY_RELESED:
				Input::proccesEvents(Input::Action::Relese, static_cast<int>(e.getArg().argUllong));
				break;
			case Core::Event::Type::MOUSE_MOVED:
				Input::proccesEvents(e.getArg().argFloat.first, e.getArg().argFloat.second);
				break;
			default:
				XN_LOG_ERR("Unknown event: " + e.getName());
		}
	}
	Input::resetStickyKeys();
}

void Application::pushEvent(const Core::Event& event) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	m_eventQueue.push(event);
	m_cond.notify_one();
}

Core::Event Application::popEvent() noexcept {
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cond.wait(lock, [this] { return !m_eventQueue.empty(); } );
	Core::Event e = m_eventQueue.front();
	m_eventQueue.pop();
	return e;
}

bool Application::emptyEventQueue() const noexcept {
	const std::lock_guard<std::mutex> lock(m_mutex);
	return m_eventQueue.empty();
}

}
