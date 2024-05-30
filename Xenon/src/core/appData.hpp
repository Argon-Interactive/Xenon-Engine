#ifndef _XENON_CORE_APPDATA_H_
#define _XENON_CORE_APPDATA_H_

#include "event.hpp"
#include "window/window.hpp"
#include <memory>

namespace Core {

class AppData {
private:
	struct ConstructorToken {};

public:
	AppData(ConstructorToken t, uint32_t width, uint32_t height, const std::string& title);

	static void init(std::function<void(const Event&)> eventDispatcher);
	static void terminate();

	[[nodiscard]] static Core::Window& getWindow();

private:

	inline static std::unique_ptr<AppData> s_appData;
	inline static bool s_exists;	// XD

	Core::Window m_window;
};

}

#endif // !_XENON_CORE_APPDATA_H_
