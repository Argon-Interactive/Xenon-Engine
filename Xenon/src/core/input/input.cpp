#include "input.hpp"
#include <stdexcept>
#include <unordered_map>
#include <glfw3.h>

#define UINT8(int) static_cast<uint8_t>(int)

namespace Core {

const static std::unordered_map<int, Input::Key> c_GLFWKeyToXenonKey = {
	{GLFW_KEY_A, Input::Key::A},
	{GLFW_KEY_B, Input::Key::B},
	{GLFW_KEY_C, Input::Key::C},
	{GLFW_KEY_D, Input::Key::D},
	{GLFW_KEY_E, Input::Key::E},
	{GLFW_KEY_F, Input::Key::F},
	{GLFW_KEY_G, Input::Key::G},
	{GLFW_KEY_H, Input::Key::H},
	{GLFW_KEY_I, Input::Key::I},
	{GLFW_KEY_J, Input::Key::J},
	{GLFW_KEY_K, Input::Key::K},
	{GLFW_KEY_L, Input::Key::L},
	{GLFW_KEY_M, Input::Key::M},
	{GLFW_KEY_N, Input::Key::N},
	{GLFW_KEY_O, Input::Key::O},
	{GLFW_KEY_P, Input::Key::P},
	{GLFW_KEY_Q, Input::Key::Q},
	{GLFW_KEY_R, Input::Key::R},
	{GLFW_KEY_S, Input::Key::S},
	{GLFW_KEY_T, Input::Key::T},
	{GLFW_KEY_U, Input::Key::U},
	{GLFW_KEY_V, Input::Key::V},
	{GLFW_KEY_W, Input::Key::W},
	{GLFW_KEY_X, Input::Key::X},
	{GLFW_KEY_Y, Input::Key::Y},
	{GLFW_KEY_Z, Input::Key::Z},
	{GLFW_KEY_0, Input::Key::Nr0},
	{GLFW_KEY_1, Input::Key::Nr1},
	{GLFW_KEY_2, Input::Key::Nr2},
	{GLFW_KEY_3, Input::Key::Nr3},
	{GLFW_KEY_4, Input::Key::Nr4},
	{GLFW_KEY_5, Input::Key::Nr5},
	{GLFW_KEY_6, Input::Key::Nr6},
	{GLFW_KEY_7, Input::Key::Nr7},
	{GLFW_KEY_8, Input::Key::Nr8},
	{GLFW_KEY_9, Input::Key::Nr9},
	{GLFW_KEY_KP_0, Input::Key::Np0},
	{GLFW_KEY_KP_1, Input::Key::Np1},
	{GLFW_KEY_KP_2, Input::Key::Np2},
	{GLFW_KEY_KP_3, Input::Key::Np3},
	{GLFW_KEY_KP_4, Input::Key::Np4},
	{GLFW_KEY_KP_5, Input::Key::Np5},
	{GLFW_KEY_KP_6, Input::Key::Np6},
	{GLFW_KEY_KP_7, Input::Key::Np7},
	{GLFW_KEY_KP_8, Input::Key::Np8},
	{GLFW_KEY_KP_9, Input::Key::Np9},
	{GLFW_KEY_F1, Input::Key::F1},
	{GLFW_KEY_F2, Input::Key::F2},
	{GLFW_KEY_F3, Input::Key::F3},
	{GLFW_KEY_F4, Input::Key::F4},
	{GLFW_KEY_F5, Input::Key::F5},
	{GLFW_KEY_F6, Input::Key::F6},
	{GLFW_KEY_F7, Input::Key::F7},
	{GLFW_KEY_F8, Input::Key::F8},
	{GLFW_KEY_F9, Input::Key::F9},
	{GLFW_KEY_F10, Input::Key::F10},
	{GLFW_KEY_F11, Input::Key::F11},
	{GLFW_KEY_F12, Input::Key::F12},
	{GLFW_KEY_F13, Input::Key::F13},
	{GLFW_KEY_F14, Input::Key::F14},
	{GLFW_KEY_F15, Input::Key::F15},
	{GLFW_KEY_F16, Input::Key::F16},
	{GLFW_KEY_F17, Input::Key::F17},
	{GLFW_KEY_F18, Input::Key::F18},
	{GLFW_KEY_F19, Input::Key::F19},
	{GLFW_KEY_F20, Input::Key::F20},
	{GLFW_KEY_F21, Input::Key::F21},
	{GLFW_KEY_F22, Input::Key::F22},
	{GLFW_KEY_F23, Input::Key::F23},
	{GLFW_KEY_F24, Input::Key::F24},
	{GLFW_KEY_F25, Input::Key::F25},
	{GLFW_KEY_SPACE, Input::Key::Space},
	{GLFW_KEY_ENTER, Input::Key::Enter},
	{GLFW_KEY_LEFT_SHIFT, Input::Key::Shift},
	{GLFW_KEY_RIGHT_SHIFT, Input::Key::Right_Shift},
	{GLFW_KEY_LEFT_CONTROL, Input::Key::Control},
	{GLFW_KEY_RIGHT_CONTROL, Input::Key::Right_Control},
	{GLFW_KEY_LEFT_ALT, Input::Key::Alt},
	{GLFW_KEY_RIGHT_ALT, Input::Key::Alt_Gr},
	{GLFW_KEY_CAPS_LOCK, Input::Key::Caps_Lock},
	{GLFW_KEY_TAB, Input::Key::Tab},
	{GLFW_KEY_ESCAPE, Input::Key::Esc},
	{GLFW_KEY_LEFT_SUPER, Input::Key::Windows},
	{GLFW_KEY_LEFT_SUPER, Input::Key::Command},
	{GLFW_KEY_LEFT_SUPER, Input::Key::Super},
	{GLFW_KEY_LEFT_BRACKET, Input::Key::Left_Braket},
	{GLFW_KEY_RIGHT_BRACKET, Input::Key::Right_Braket},
	{GLFW_KEY_SEMICOLON, Input::Key::Semicolon},
	{GLFW_KEY_APOSTROPHE, Input::Key::Apostrophe},
	{GLFW_KEY_BACKSLASH, Input::Key::Backslash},
	{GLFW_KEY_COMMA, Input::Key::Comma},
	{GLFW_KEY_PERIOD, Input::Key::Period},
	{GLFW_KEY_SLASH, Input::Key::Slash},
	{GLFW_KEY_MINUS, Input::Key::Dash},
	{GLFW_KEY_EQUAL, Input::Key::Equals},
	{GLFW_KEY_GRAVE_ACCENT, Input::Key::Back_Quote},
	{GLFW_KEY_UP, Input::Key::Up},
	{GLFW_KEY_DOWN, Input::Key::Down},
	{GLFW_KEY_LEFT, Input::Key::Left},
	{GLFW_KEY_RIGHT, Input::Key::Right},
	{GLFW_KEY_PRINT_SCREEN, Input::Key::Print_Screen},
	{GLFW_KEY_INSERT, Input::Key::Insert},
	{GLFW_KEY_DELETE, Input::Key::Delete},
	{GLFW_KEY_HOME, Input::Key::Home},
	{GLFW_KEY_END, Input::Key::End},
	{GLFW_KEY_PAGE_UP, Input::Key::PageUp},
	{GLFW_KEY_PAGE_DOWN, Input::Key::PageDown},
	{GLFW_KEY_PAUSE, Input::Key::Pause},
	{GLFW_KEY_KP_DIVIDE, Input::Key::Np_Devide},
	{GLFW_KEY_KP_MULTIPLY, Input::Key::Np_Multiply},
	{GLFW_KEY_KP_SUBTRACT, Input::Key::Np_Subtract},
	{GLFW_KEY_KP_ADD, Input::Key::Np_Add},
	{GLFW_KEY_KP_ENTER, Input::Key::Np_Enter},
	{GLFW_KEY_KP_DECIMAL, Input::Key::Np_Decimal},
	{GLFW_KEY_SCROLL_LOCK, Input::Key::Scroll_Lock},
	{GLFW_KEY_NUM_LOCK, Input::Key::Num_Lock},
	{GLFW_KEY_BACKSPACE, Input::Key::Backspace},
	{GLFW_MOUSE_BUTTON_1, Input::Key::Left_Mouse_Button},
	{GLFW_MOUSE_BUTTON_2, Input::Key::Right_Mouse_Button},
	{GLFW_MOUSE_BUTTON_3, Input::Key::Middle_Mouse_Button},
	{GLFW_MOUSE_BUTTON_4, Input::Key::Mouse_Button_4},
	{GLFW_MOUSE_BUTTON_5, Input::Key::Mouse_Button_5},
	{GLFW_MOUSE_BUTTON_6, Input::Key::Mouse_Button_6},
	{GLFW_MOUSE_BUTTON_7, Input::Key::Mouse_Button_7},
	{GLFW_MOUSE_BUTTON_8, Input::Key::Mouse_Button_8}
};

Input::Input() {
	throw std::runtime_error("Input is a static class and there shouldn't exist an instance of it");
}

bool Input::getKeyPress(Key key) {
	const uint8_t res = s_keyStateMap.at(key) & 1u;
	s_keyStateMap.at(key) &= UINT8(~1u);
	return res != 0;
}

bool Input::getKeyRelese(Key key) {
	const uint8_t res = s_keyStateMap.at(key) & 2u;
	s_keyStateMap.at(key) &= UINT8(~2u);
	return res != 0;
}

bool Input::getKeyHold(Key key) {
	return (s_keyStateMap.at(key) & 4u) != 0;
}

void Input::enableCursor() { glfwSetInputMode(static_cast<GLFWwindow*>(s_window), GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
void Input::disableCursor() { glfwSetInputMode(static_cast<GLFWwindow*>(s_window), GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

std::pair<float, float> Input::getMouseScreenPosition() {
	return {s_xMousePosition, s_yMousePosition};
}

void Input::init(void* window) {
	static bool singletonCheck = false;
	if(singletonCheck) throw std::runtime_error("Input was initialized twice");
	singletonCheck = true;
	s_keyStateMap.fill(0);
	s_window = window;
}

void Input::resetStickyKeys() {
	for(auto& val : s_keyStateMap) {
		const uint8_t resetPressFlag = val & 8u;
		const uint8_t resetReleseFlag = val & 16u;
		val &= UINT8(~UINT8(resetPressFlag >> 3u));
		val &= UINT8(~UINT8(resetReleseFlag >> 3u));
		val &= UINT8(~24u);
		val |= UINT8(UINT8(val & 1u) << 3u);
		val |= UINT8(UINT8(val & 2u) << 3u);
	}	
	//I know this look like shit but there is no way to do this bcos clangtidy is stupid and annoying
}

void Input::proccesEvents(Input::Action act, int GLFWKeyCode) {
	auto inx = static_cast<size_t>(c_GLFWKeyToXenonKey.at(GLFWKeyCode));
	auto actInt = act;
	s_keyStateMap.at(inx) |= UINT8(actInt + 1);
	s_keyStateMap.at(inx) &= UINT8(~UINT8(4));
	s_keyStateMap.at(inx) |= UINT8(UINT8(static_cast<int>(actInt == 0u)) << 2u); 
}

void Input::proccesEvents(float xpos, float ypos) { 
	s_xMousePosition = xpos; 
	s_yMousePosition = ypos; 
}

}
