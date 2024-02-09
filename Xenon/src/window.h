#pragma once 
#include<all.hpp>
#include<glad.h>
#include<glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

namespace xen
{
	class Window
	{
	public:
		Window(const Window&) = delete;
		Window(uint32_t width, uint32_t height, std::string title);
		~Window();
		void create();
		bool closeCallBack() const;
		void close() const;
		void setFullscreen(bool fullscreen);
		//Not fully working as indended WIP
		void setBorderless(bool borderless);
		void setVSync(bool vsync);
		void setWindowSize(uint32_t width, uint32_t height);
		void setWindowSize(glm::uvec2 dims);
		void setWindowPos(uint32_t x, uint32_t y);
		void setWindowPos(glm::uvec2 pos);
		void setSizeLimits(uint32_t minW, uint32_t minH, uint32_t maxW, uint32_t maxH);
		void setSizeLimits(glm::uvec2 minDims, glm::uvec2 maxDims);
		void setTitle(std::string title);
		void setIcon(std::string icon, std::string icon_small);
		void setIcon() const;
		void maximizeWindow(bool maximize) const;
		void setResizable(bool resizable) const;
		bool isBorderless() const;
		bool isFullscreen() const;
		std::string getTitle();
		glm::uvec2 getWindowSize();
		glm::ivec2 getWindowPos();
		glm::uvec2 getScreenResolution();
		bool getVSync() const;
		void FEP() const;
		GLFWwindow* passPointer() const;
	private:
		bool m_isVSync;
		bool m_isBorderless;
		GLFWwindow* m_ID;
		std::string m_title;
		GLFWmonitor* m_monitor;
		GLFWimage m_icon[2];
		int m_winPosX, m_winPosY;
		int m_winSizeW, m_winSizeH;

	};
}
