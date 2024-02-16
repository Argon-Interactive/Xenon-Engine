#pragma once 
#include"../devTools/logger.hpp"
#include<glfw3.h>
#include<glm/glm.hpp>

namespace core
{
	class Window
	{
	public:
		Window(const Window&) = delete;
		Window(uint32_t width, uint32_t height, std::string title);
		~Window();
		bool closeCallBack() const;
		void close() const;
		void setFullscreen(bool fullscreen);
		//Not fully working as indended WIP
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
		bool isBorderless() const;
		bool isFullscreen() const;
		std::string getTitle();
		std::pair<uint32_t, uint32_t> getWindowSize();
		std::pair<int, int> getWindowPos();
		std::pair<uint32_t, uint32_t> getScreenResolution();
		std::pair<uint32_t, uint32_t> getScreenAspectRatio();
		bool getVSync() const;
		void FEP() const;
		GLFWwindow* passPointer() const;
	private:
		bool m_isVSync;
		bool m_isBorderless;
		GLFWwindow* m_ID;
		std::string m_title;
		GLFWmonitor* m_monitor;
	};
}
