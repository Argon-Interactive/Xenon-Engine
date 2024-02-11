#include "appData.h"

#include "all.hpp"


namespace core
{

	AppData& AppData::getInstance() {
		static AppData instance;
		return instance;
	}

	void AppData::init(uint32_t width, uint32_t height, const std::string& title) {
		ALL_ENT("===============================================================================");
		ALL_ENT("Application Systems initialization...");

		getWindow().create();
		getWindow().setWindowSize(width, height);
		getWindow().setTitle(title);

		ALL_INF("Application systems initialized successfully");
		ALL_ENT("===============================================================================");
	}

	void AppData::terminate() {
		ALL_ENT("===============================================================================");
		ALL_ENT("Application systems termination...");

		ALL_INF("Application systems terminated successfully");
		ALL_ENT("===============================================================================");
	}

	AppData::AppData()
		:m_window(512, 256, "elo ¿elo") {}

	xen::Window& AppData::getWindow() {
		return getInstance().m_window;
	}

}