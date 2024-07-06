#include "application.hpp"
#include "core/application.hpp"

namespace Xenon {

Application::Application() :m_impl(nullptr) {}

Application::~Application() {
	delete m_impl;
}

void Application::create() {
	m_impl = new Core::Application(m_configFunction, m_buildFunctions); // NOLINT yeah i fucking know
}

void Application::addConfiguration(std::function<void(void*)> configFunction) {
	m_configFunction = configFunction;
}

void Application::addScenes(std::vector<std::function<void(Scene*)>> buildFunctions) {
	m_buildFunctions = buildFunctions;
}

int Application::run() {
	return m_impl->run();
}

}
