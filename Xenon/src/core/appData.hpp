#ifndef _XENON_APPDATA_
#define _XENON_APPDATA_

#include <memory>
#include "window.hpp"

namespace Core
{

	class AppData
	{

	public:
		AppData(uint32_t width, uint32_t height, const std::string& title);

		static void init();
		static void terminate();

		static Core::Window& getWindow();

	private:

		static std::unique_ptr<AppData> s_appData;
		static bool s_exists;	// XD

		Core::Window m_window;
	};

}

#endif // !_XENON_APPDATA_
