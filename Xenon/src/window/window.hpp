#ifndef _XENON_WINDOW_
#define _XENON_WINDOW_

#include"core/event/event.hpp"
#include<glfw3.h>
#include<functional>
#include<string>

namespace Core
{
	class Window
	{
	public:
		Window(Window &&) = delete;
		~Window();
		Window &operator=(const Window &) = delete;
		Window &operator=(Window &&) = delete;
		Window(const Window &) = delete;
		Window(uint32_t width, uint32_t height, std::string title);
		[[nodiscard]] bool closeCallBack() const;
		void close() const;
		void setEventDispatcher(std::function<void(const Core::Event &)> dispatch);
		void setFullscreen(bool fullscreen);
		// Not fully working as indended WIP
		void setBorderless(bool borderless);
		void setVSync(bool vsync);
		void setWindowSize(uint32_t width, uint32_t height);
		void setWindowSize(std::pair<uint32_t, uint32_t> dims);
		void setWindowPos(uint32_t x, uint32_t y);
		void setWindowPos(std::pair<uint32_t, uint32_t> pos);
		void setSizeLimits(uint32_t minW, uint32_t minH, uint32_t maxW, uint32_t maxH);
		void setSizeLimits(std::pair<uint32_t, uint32_t> minDims, std::pair<uint32_t, uint32_t> maxDims);
		void setTitle(std::string title);
		void setIcon(std::string icon, std::string icon_small);
		void setIcon(std::string icon);
		void setIcon() const;
		void maximizeWindow(bool maximize) const;
		void setResizable(bool resizable) const;
		[[nodiscard]] bool isBorderless() const;
		[[nodiscard]] bool isFullscreen() const;
		[[nodiscard]] bool isFocused() const;
		std::string getTitle();
		std::pair<uint32_t, uint32_t> getWindowSize();
		std::pair<int, int> getWindowPos();
		std::pair<uint32_t, uint32_t> getScreenResolution();
		std::pair<uint32_t, uint32_t> getScreenAspectRatio();
		[[nodiscard]] bool getVSync() const;
		void FEP() const;
		[[nodiscard]] GLFWwindow *passPointer() const;
	private:
		GLFWwindow* m_ID;
		bool m_isVSync;
		bool m_isBorderless;
		std::string m_title;
		GLFWmonitor* m_monitor;
		std::function<void(const Core::Event&)> m_eventDispatcher;
	};
}

#endif // !_XENON_WINDOW_
