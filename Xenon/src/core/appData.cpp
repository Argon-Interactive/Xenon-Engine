#include "appData.h"
#include "logger.hpp"

namespace Core
{

	AppData& AppData::getInstance() {
		static AppData instance;
		return instance;
	}

	void AppData::init(uint32_t width, uint32_t height, const std::string& title) {
		XN_LOG_BR;
		XN_LOG_ENT("Application Systems initialization...");

		getWindow().create();
		getWindow().setWindowSize(width, height);
		getWindow().setTitle(title);

		XN_LOG_INF("Application systems initialized successfully");
		XN_LOG_BR;
	}

	void AppData::terminate() {
		XN_LOG_BR;
		XN_LOG_ENT("Application systems termination...");

		XN_LOG_INF("Application systems terminated successfully");
		XN_LOG_BR;
	}

	AppData::AppData()
		:m_window(512, 256, "elo ¿elo") {}

	Window& AppData::getWindow() {
		return getInstance().m_window;
	}

}