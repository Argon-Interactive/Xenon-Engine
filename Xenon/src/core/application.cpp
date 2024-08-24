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
	if(Input::getKeyPress(Input::Np1))
		AppData::getSceneManager().loadScene(1);
	if(Input::getKeyPress(Input::Np2))
		AppData::getSceneManager().unloadScene(1);
	if(Input::getKeyPress(Input::Np3))
		AppData::getSceneManager().loadSceneAsync(1);
	if(Input::getKeyPress(Input::Np4))
		AppData::getSceneManager().unloadSceneAsync(1);
	if(Input::getKeyPress(Input::Np5))
		AppData::getSceneManager().switchScene(1);
	if(Input::getKeyPress(Input::Np6))
		AppData::getSceneManager().switchSceneImmediate(2);
	if(Input::getKeyPress(Input::Np7))
		AppData::getSceneManager().switchSceneAsync(3);
	if(Input::getKeyPress(Input::Np8)) {
		AppData::getSceneManager().loadSceneAsync(10);
		AppData::getSceneManager().loadSceneAsync(11);
		AppData::getSceneManager().loadSceneAsync(12);
		AppData::getSceneManager().loadSceneAsync(13);
		AppData::getSceneManager().loadSceneAsync(14);
		AppData::getSceneManager().loadSceneAsync(15);
	}
	if(Input::getKeyPress(Input::Np9))
		AppData::getSceneManager().switchSceneAsync(0);
	if(Input::getKeyPress(Input::Np0))
		AppData::getSceneManager().purgeAsync();
	if(Input::getKeyPress(Input::Enter))
		AppData::getSceneManager().debug();
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
