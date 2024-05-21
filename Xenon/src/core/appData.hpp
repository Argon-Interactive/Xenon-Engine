#ifndef _XENON_APPDATA_H_
#define _XENON_APPDATA_H_

#include "core/input/input.hpp"
#include "event/event.hpp"
#include "window/window.hpp"
#include <memory>

namespace Core
{

	class AppData
	{

	public:
		AppData(uint32_t width, uint32_t height, const std::string& title);

		static void init(std::function<void(const Xenon::Event&)> eventDispatcher);
		static void terminate();

		static Core::Window& getWindow();

	private:

		static std::unique_ptr<AppData> s_appData;
		static bool s_exists;	// XD

		Core::Window m_window;
	};

}

#endif // !_XENON_APPDATA_H_
