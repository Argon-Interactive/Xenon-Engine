#ifndef _XENON_APPDATA_H_
#define _XENON_APPDATA_H_

#include "window.h"

namespace Core
{

	class AppData
	{

	public:

		static void init(uint32_t width, uint32_t height, const std::string& title);
		static void terminate();

		static Core::Window& getWindow();

	private:
		AppData();

		static AppData& getInstance();

		Core::Window m_window;
	};

}

#endif // !_XENON_APPDATA_H_