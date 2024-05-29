#include <functional>
#include<glad.h> //this must be included before window.h
#include<stb_image.h>
#include<numeric>
#include <utility>
#include"window.hpp"
#include"devTools/logger/logger_core.hpp"
#include "glfw3.h"

Core::Window::Window(uint32_t width, uint32_t height, std::string title)
	:m_ID(nullptr), m_isVSync(true), m_isBorderless(false), m_title(std::move(title)), m_monitor(glfwGetPrimaryMonitor())
{
	glfwWindowHint(GLFW_SAMPLES, 4);
	m_ID = glfwCreateWindow(static_cast<int32_t>(width), static_cast<int32_t>(height), m_title.c_str(), nullptr, nullptr);
	if (m_ID == nullptr)
	{
		XN_LOG_ERR("Error with creation of a window named \"{0}\".", m_title);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_ID);
	// vsync on by default!
	glfwSwapInterval(1);
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		XN_LOG_ERR("Failed to initialize GLAD.");
		exit(EXIT_FAILURE);
	}
	glfwSetInputMode(m_ID, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetInputMode(m_ID, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	using Core::Event;
	glfwSetFramebufferSizeCallback(m_ID, [](GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		auto *fun = reinterpret_cast<std::function<void(const Event&)>*>(glfwGetWindowUserPointer(window));
		//int0 = width int1 = height
		const Event e(Event::Type::WINDOW_RESIZE, w, h);
		(*fun)(e);
	});

	glfwSetWindowUserPointer(m_ID, &m_eventDispatcher);

	glfwSetWindowCloseCallback(m_ID, [](GLFWwindow* window) {
		auto *fun = reinterpret_cast<std::function<void(const Event&)>*>(glfwGetWindowUserPointer(window));
		const Event e(Event::Type::WINDOW_CLOSE);
		(*fun)(e);
	});

	glfwSetKeyCallback(m_ID, [](GLFWwindow* window, int key,[[maybe_unused]] int scancode, int action,[[maybe_unused]] int mods) {
		auto *fun = reinterpret_cast<std::function<void(const Event&)>*>(glfwGetWindowUserPointer(window));
		if(action == GLFW_REPEAT) return;
		const Event e(static_cast<Event::Type>(action), static_cast<uint64_t>(key));
		(*fun)(e);
	});

	glfwSetMouseButtonCallback(m_ID, [](GLFWwindow* window, int button, int action,[[maybe_unused]] int mods) {
		auto *fun = reinterpret_cast<std::function<void(const Event&)>*>(glfwGetWindowUserPointer(window));
		const Event e(static_cast<Event::Type>(action), static_cast<uint64_t>(button));
		(*fun)(e);
	});

	glfwSetCursorPosCallback(m_ID, [](GLFWwindow* window, double xpos, double ypos){
		auto *fun = reinterpret_cast<std::function<void(const Event&)>*>(glfwGetWindowUserPointer(window));
		const Event e(Event::Type::MOUSE_MOVED, static_cast<float>(xpos), static_cast<float>(ypos));
		(*fun)(e);
	});
}

Core::Window::~Window()
{ glfwDestroyWindow(m_ID); }

bool Core::Window::closeCallBack() const
{
	if (m_ID == nullptr) return false;
	return glfwWindowShouldClose(m_ID) != 0;
}

void Core::Window::close() const
{ if (m_ID == nullptr) { return; } glfwSetWindowShouldClose(m_ID, GLFW_TRUE); }

void Core::Window::setEventDispatcher(std::function<void(const Core::Event&)> dispatch) {
	m_eventDispatcher = dispatch;
}

void Core::Window::setFullscreen(bool fullscreen)
{
	if (isFullscreen() == fullscreen) return;
	int posx = 0; int posy = 0; int sizew = 0; int sizeh = 0;
	if (fullscreen)
	{
		glfwGetWindowPos(m_ID, &posx, &posy);
		glfwGetWindowSize(m_ID, &sizew, &sizeh);
		const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
		if (m_isVSync) {
			glfwSetWindowMonitor(m_ID, m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		} else {
			glfwSetWindowMonitor(m_ID, m_monitor, 0, 0, mode->width, mode->height, 0);
		}
	}
	else
	{ glfwSetWindowMonitor(m_ID, m_monitor, posx, posy, sizew, sizeh, 0); }
}

void Core::Window::setBorderless(bool borderless)
{
	if (isBorderless() == borderless) return;
	if (borderless)
	{ glfwSetWindowAttrib(m_ID, GLFW_DECORATED, GLFW_FALSE); maximizeWindow(true); }
	else
	{ maximizeWindow(false); glfwSetWindowAttrib(m_ID, GLFW_DECORATED, GLFW_TRUE); }
	m_isBorderless = borderless;
}

void Core::Window::setVSync(bool vsync)
{ 
	glfwSwapInterval(static_cast<int>(vsync)); 
	if (isFullscreen()) {
		const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
		glfwSetWindowMonitor(m_ID, m_monitor, 0, 0, mode->width, mode->height, vsync ? mode->refreshRate : 0);
	}
}

void Core::Window::setWindowSize(uint32_t width, uint32_t height)
{ glfwSetWindowSize(m_ID, static_cast<int32_t>(width), static_cast<int32_t>(height)); }

void Core::Window::setWindowSize(std::pair<uint32_t, uint32_t> dims)
{ glfwSetWindowSize(m_ID, static_cast<int32_t>(dims.first), static_cast<int32_t>(dims.second)); }

void Core::Window::setWindowPos(uint32_t x, uint32_t y)
{ glfwSetWindowPos(m_ID, static_cast<int>(x), static_cast<int>(y)); }

void Core::Window::setWindowPos(std::pair<uint32_t, uint32_t> pos)
{ glfwSetWindowPos(m_ID, static_cast<int>(pos.first), static_cast<int>(pos.second)); }

void Core::Window::setSizeLimits(uint32_t minW, uint32_t minH, uint32_t maxW, uint32_t maxH)
{ glfwSetWindowSizeLimits(m_ID, static_cast<int>(minW), static_cast<int>(minH), static_cast<int>(maxW), static_cast<int>(maxH)); }

void Core::Window::setSizeLimits(std::pair<uint32_t, uint32_t> minDims, std::pair<uint32_t, uint32_t> maxDims)
{ glfwSetWindowSizeLimits(m_ID, static_cast<int>(minDims.first), static_cast<int>(minDims.second), static_cast<int>(maxDims.first), static_cast<int>(maxDims.second)); }

void Core::Window::setTitle(std::string title)
{ glfwSetWindowTitle(m_ID, title.c_str()); m_title = title; }

void Core::Window::FEP() const
{
	glfwSwapBuffers(m_ID);
	glfwPollEvents();
	glClearColor(.3f, .3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Core::Window::setIcon(std::string icon, std::string icon_small)
{
	stbi_set_flip_vertically_on_load(0);
	GLFWimage iconImage[2]; //NOLINT - stupid warning
	iconImage[0].pixels = stbi_load(icon.c_str(), &iconImage[0].width, &iconImage[0].height, nullptr, 4);
	iconImage[1].pixels = stbi_load(icon_small.c_str(), &iconImage[1].width, &iconImage[1].height, nullptr, 4);

	if (iconImage[0].pixels == nullptr || iconImage[1].pixels == nullptr)
	{ XN_LOG_ERR("Failed to load window icon."); return; }

	glfwSetWindowIcon(m_ID, 2, iconImage); //NOLINT - stupid warning
	stbi_image_free(iconImage[0].pixels);
	stbi_image_free(iconImage[1].pixels);
	stbi_set_flip_vertically_on_load(1);
}

void Core::Window::setIcon(std::string icon)
{
	stbi_set_flip_vertically_on_load(0);
	GLFWimage iconImage;
	iconImage.pixels = stbi_load(icon.c_str(), &iconImage.width, &iconImage.height, nullptr, 4);

	if (iconImage.pixels == nullptr)
	{ XN_LOG_ERR("Failed to load window icon."); return; }

	glfwSetWindowIcon(m_ID, 1, &iconImage);
	stbi_image_free(iconImage.pixels);
	stbi_set_flip_vertically_on_load(1);
}

void Core::Window::setIcon() const
{ glfwSetWindowIcon(m_ID, 0, nullptr); }

void Core::Window::maximizeWindow(bool maximize) const
{
	if (maximize) glfwMaximizeWindow(m_ID);
	else glfwRestoreWindow(m_ID);
}

void Core::Window::setResizable(bool resizable) const
{ glfwSetWindowAttrib(m_ID, GLFW_RESIZABLE, static_cast<int>(resizable)); }

bool Core::Window::isBorderless() const
{ return m_isBorderless; }

bool Core::Window::isFullscreen() const
{ return (m_ID == nullptr) ? false : glfwGetWindowMonitor(m_ID) != nullptr; }

bool Core::Window::isFocused() const 
{ return glfwGetWindowAttrib(m_ID, GLFW_FOCUSED) == GLFW_TRUE; }

std::string Core::Window::getTitle()
{ return m_title; }

std::pair<uint32_t, uint32_t> Core::Window::getWindowSize()
{
	int w = 0; int h = 0;
	glfwGetWindowSize(m_ID, &w, &h);
	return { static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
}

std::pair<int, int> Core::Window::getWindowPos()
{
	int x = 0; int y = 0;
	glfwGetWindowSize(m_ID, &x, &y);
	return { x, y };
}

std::pair<uint32_t, uint32_t> Core::Window::getScreenResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
	return { static_cast<uint32_t>(mode->width), static_cast<uint32_t>(mode->height) };
}

std::pair<uint32_t, uint32_t> Core::Window::getScreenAspectRatio()
{
	auto Wsize = getWindowSize();
	const unsigned int gcd = std::gcd(Wsize.first, Wsize.second);
	return { Wsize.first / gcd, Wsize.second / gcd };
}

bool Core::Window::getVSync() const
{ return m_isVSync; }

GLFWwindow* Core::Window::passPointer() const
{ return m_ID; }

