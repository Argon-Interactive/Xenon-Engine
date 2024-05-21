#include "input.hpp"
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include "glfw3.h"
#include "devTools/logger/logger_core.hpp"

/*

	enum Key {
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, W, Y, Z,
		Nr1, Nr2, Nr3, Nr4, Nr5, Nr6, Nr7, Nr8, Nr9, Nr0,
		Np1, Np2, Np3, Np4, Np5, Np6, Np7, Np8, Np9, Np0,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
		Space, Enter, Shift, Control, Alt, Caps_Lock, Tab, Esc, Fn, Windows, Alt_Gr, Right_Control, Right_Shift,
		Left_Braket, Right_Braket, Semicolon, Apostrophe, Backslash, Comma, Period, Slash, Dash, Equals, Back_Quote,
		Up, Down, Left, Right,
		Print_Screen, Insert, Delete, Home, End, PageUp, PageDown, Pause,
		Np_Devide, Np_Multiply, Np_Minus, Np_Add, Np_Enter, 
	};

*/

const static std::unordered_map<int, Xenon::Input::Key> c_GLFWKeyToXenonKey = {
	{GLFW_KEY_A, Xenon::Input::Key::A},
	{GLFW_KEY_B, Xenon::Input::Key::B},
	{GLFW_KEY_C, Xenon::Input::Key::C},
	{GLFW_KEY_D, Xenon::Input::Key::D},
	{GLFW_KEY_E, Xenon::Input::Key::E},
	{GLFW_KEY_F, Xenon::Input::Key::F},
	{GLFW_KEY_G, Xenon::Input::Key::G},
	{GLFW_KEY_H, Xenon::Input::Key::H},
	{GLFW_KEY_I, Xenon::Input::Key::I},
	{GLFW_KEY_J, Xenon::Input::Key::J},
	{GLFW_KEY_K, Xenon::Input::Key::K},
	{GLFW_KEY_L, Xenon::Input::Key::L},
	{GLFW_KEY_M, Xenon::Input::Key::M},
	{GLFW_KEY_N, Xenon::Input::Key::N},
	{GLFW_KEY_O, Xenon::Input::Key::O},
	{GLFW_KEY_P, Xenon::Input::Key::P},
	{GLFW_KEY_R, Xenon::Input::Key::R},
	{GLFW_KEY_S, Xenon::Input::Key::S},
	{GLFW_KEY_T, Xenon::Input::Key::T},
	{GLFW_KEY_U, Xenon::Input::Key::U},
	{GLFW_KEY_W, Xenon::Input::Key::W},
	{GLFW_KEY_Y, Xenon::Input::Key::Y},
	{GLFW_KEY_Z, Xenon::Input::Key::Z},
	{GLFW_KEY_0, Xenon::Input::Key::Nr0},
	{GLFW_KEY_1, Xenon::Input::Key::Nr1},
	{GLFW_KEY_2, Xenon::Input::Key::Nr2},
	{GLFW_KEY_3, Xenon::Input::Key::Nr3},
	{GLFW_KEY_4, Xenon::Input::Key::Nr4},
	{GLFW_KEY_5, Xenon::Input::Key::Nr5},
	{GLFW_KEY_6, Xenon::Input::Key::Nr6},
	{GLFW_KEY_7, Xenon::Input::Key::Nr7},
	{GLFW_KEY_8, Xenon::Input::Key::Nr8},
	{GLFW_KEY_9, Xenon::Input::Key::Nr9},
	{GLFW_KEY_KP_0, Xenon::Input::Key::Np0},
	{GLFW_KEY_KP_1, Xenon::Input::Key::Np1},
	{GLFW_KEY_KP_2, Xenon::Input::Key::Np2},
	{GLFW_KEY_KP_3, Xenon::Input::Key::Np3},
	{GLFW_KEY_KP_4, Xenon::Input::Key::Np4},
	{GLFW_KEY_KP_5, Xenon::Input::Key::Np5},
	{GLFW_KEY_KP_6, Xenon::Input::Key::Np6},
	{GLFW_KEY_KP_7, Xenon::Input::Key::Np7},
	{GLFW_KEY_KP_8, Xenon::Input::Key::Np8},
	{GLFW_KEY_KP_9, Xenon::Input::Key::Np9},
	{GLFW_KEY_F1, Xenon::Input::Key::F1},
	{GLFW_KEY_F2, Xenon::Input::Key::F2},
	{GLFW_KEY_F3, Xenon::Input::Key::F3},
	{GLFW_KEY_F4, Xenon::Input::Key::F4},
	{GLFW_KEY_F5, Xenon::Input::Key::F5},
	{GLFW_KEY_F6, Xenon::Input::Key::F6},
	{GLFW_KEY_F7, Xenon::Input::Key::F7},
	{GLFW_KEY_F8, Xenon::Input::Key::F8},
	{GLFW_KEY_F9, Xenon::Input::Key::F9},
	{GLFW_KEY_F10, Xenon::Input::Key::F10},
	{GLFW_KEY_F11, Xenon::Input::Key::F11},
	{GLFW_KEY_F12, Xenon::Input::Key::F12},
	{GLFW_KEY_F13, Xenon::Input::Key::F13},
	{GLFW_KEY_F14, Xenon::Input::Key::F14},
	{GLFW_KEY_F15, Xenon::Input::Key::F15},
	{GLFW_KEY_F16, Xenon::Input::Key::F16},
	{GLFW_KEY_F17, Xenon::Input::Key::F17},
	{GLFW_KEY_F18, Xenon::Input::Key::F18},
	{GLFW_KEY_F19, Xenon::Input::Key::F19},
	{GLFW_KEY_F20, Xenon::Input::Key::F20},
	{GLFW_KEY_F21, Xenon::Input::Key::F21},
	{GLFW_KEY_F22, Xenon::Input::Key::F22},
	{GLFW_KEY_F23, Xenon::Input::Key::F23},
	{GLFW_KEY_F24, Xenon::Input::Key::F24},
	{GLFW_KEY_F25, Xenon::Input::Key::F25},


};








bool Xenon::Input::s_singletonCheck = false;

Xenon::Input::Input() {
	if(s_singletonCheck) throw std::runtime_error("Xenon::Input is a singleton and was created twice");
	s_singletonCheck = true;
	std::fill_n(m_pressedMap, 121, false);
	std::fill_n(m_relesedMap, 121, false);
	std::fill_n(m_heldMap, 121, false);
}

