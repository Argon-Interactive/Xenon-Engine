#include "input.hpp"
#include <stdexcept>
#include <unordered_map>
#include <glfw3.h>

#define UINT8(int) static_cast<uint8_t>(int)

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
	{GLFW_KEY_Q, Xenon::Input::Key::Q},
	{GLFW_KEY_R, Xenon::Input::Key::R},
	{GLFW_KEY_S, Xenon::Input::Key::S},
	{GLFW_KEY_T, Xenon::Input::Key::T},
	{GLFW_KEY_U, Xenon::Input::Key::U},
	{GLFW_KEY_V, Xenon::Input::Key::V},
	{GLFW_KEY_W, Xenon::Input::Key::W},
	{GLFW_KEY_X, Xenon::Input::Key::X},
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
	{GLFW_KEY_SPACE, Xenon::Input::Key::Space},
	{GLFW_KEY_ENTER, Xenon::Input::Key::Enter},
	{GLFW_KEY_LEFT_SHIFT, Xenon::Input::Key::Shift},
	{GLFW_KEY_RIGHT_SHIFT, Xenon::Input::Key::Right_Shift},
	{GLFW_KEY_LEFT_CONTROL, Xenon::Input::Key::Control},
	{GLFW_KEY_RIGHT_CONTROL, Xenon::Input::Key::Right_Control},
	{GLFW_KEY_LEFT_ALT, Xenon::Input::Key::Alt},
	{GLFW_KEY_RIGHT_ALT, Xenon::Input::Key::Alt_Gr},
	{GLFW_KEY_CAPS_LOCK, Xenon::Input::Key::Caps_Lock},
	{GLFW_KEY_TAB, Xenon::Input::Key::Tab},
	{GLFW_KEY_ESCAPE, Xenon::Input::Key::Esc},
	{GLFW_KEY_LEFT_SUPER, Xenon::Input::Key::Windows},
	{GLFW_KEY_LEFT_SUPER, Xenon::Input::Key::Command},
	{GLFW_KEY_LEFT_SUPER, Xenon::Input::Key::Super},
	{GLFW_KEY_LEFT_BRACKET, Xenon::Input::Key::Left_Braket},
	{GLFW_KEY_RIGHT_BRACKET, Xenon::Input::Key::Right_Braket},
	{GLFW_KEY_SEMICOLON, Xenon::Input::Key::Semicolon},
	{GLFW_KEY_APOSTROPHE, Xenon::Input::Key::Apostrophe},
	{GLFW_KEY_BACKSLASH, Xenon::Input::Key::Backslash},
	{GLFW_KEY_COMMA, Xenon::Input::Key::Comma},
	{GLFW_KEY_PERIOD, Xenon::Input::Key::Period},
	{GLFW_KEY_SLASH, Xenon::Input::Key::Slash},
	{GLFW_KEY_MINUS, Xenon::Input::Key::Dash},
	{GLFW_KEY_EQUAL, Xenon::Input::Key::Equals},
	{GLFW_KEY_GRAVE_ACCENT, Xenon::Input::Key::Back_Quote},
	{GLFW_KEY_UP, Xenon::Input::Key::Up},
	{GLFW_KEY_DOWN, Xenon::Input::Key::Down},
	{GLFW_KEY_LEFT, Xenon::Input::Key::Left},
	{GLFW_KEY_RIGHT, Xenon::Input::Key::Right},
	{GLFW_KEY_PRINT_SCREEN, Xenon::Input::Key::Print_Screen},
	{GLFW_KEY_INSERT, Xenon::Input::Key::Insert},
	{GLFW_KEY_DELETE, Xenon::Input::Key::Delete},
	{GLFW_KEY_HOME, Xenon::Input::Key::Home},
	{GLFW_KEY_END, Xenon::Input::Key::End},
	{GLFW_KEY_PAGE_UP, Xenon::Input::Key::PageUp},
	{GLFW_KEY_PAGE_DOWN, Xenon::Input::Key::PageDown},
	{GLFW_KEY_PAUSE, Xenon::Input::Key::Pause},
	{GLFW_KEY_KP_DIVIDE, Xenon::Input::Key::Np_Devide},
	{GLFW_KEY_KP_MULTIPLY, Xenon::Input::Key::Np_Multiply},
	{GLFW_KEY_KP_SUBTRACT, Xenon::Input::Key::Np_Subtract},
	{GLFW_KEY_KP_ADD, Xenon::Input::Key::Np_Add},
	{GLFW_KEY_KP_ENTER, Xenon::Input::Key::Np_Enter},
	{GLFW_KEY_KP_DECIMAL, Xenon::Input::Key::Np_Decimal},
	{GLFW_KEY_SCROLL_LOCK, Xenon::Input::Key::Scroll_Lock},
	{GLFW_KEY_NUM_LOCK, Xenon::Input::Key::Num_Lock},
	{GLFW_KEY_BACKSPACE, Xenon::Input::Key::Backspace},
	{GLFW_MOUSE_BUTTON_1, Xenon::Input::Key::Left_Mouse_Button},
	{GLFW_MOUSE_BUTTON_2, Xenon::Input::Key::Right_Mouse_Button},
	{GLFW_MOUSE_BUTTON_3, Xenon::Input::Key::Middle_Mouse_Button},
	{GLFW_MOUSE_BUTTON_4, Xenon::Input::Key::Mouse_Button_4},
	{GLFW_MOUSE_BUTTON_5, Xenon::Input::Key::Mouse_Button_5},
	{GLFW_MOUSE_BUTTON_6, Xenon::Input::Key::Mouse_Button_6},
	{GLFW_MOUSE_BUTTON_7, Xenon::Input::Key::Mouse_Button_7},
	{GLFW_MOUSE_BUTTON_8, Xenon::Input::Key::Mouse_Button_8}
};

Xenon::Input::Input() {
	throw std::runtime_error("Xenon::Input is a static class and there shouldn't exist an instance of it");
}

bool Xenon::Input::getKeyPress(Key key) {
	const uint8_t res = s_keyStateMap.at(key) & 1u;
	s_keyStateMap.at(key) &= UINT8(~1u);
	return res != 0;
}

bool Xenon::Input::getKeyRelese(Key key) {
	const uint8_t res = s_keyStateMap.at(key) & 2u;
	s_keyStateMap.at(key) &= UINT8(~2u);
	return res != 0;
}

bool Xenon::Input::getKeyHold(Key key) {
	return (s_keyStateMap.at(key) & 4u) != 0;
}

void Xenon::Input::enableCursor() { glfwSetInputMode(static_cast<GLFWwindow*>(s_window), GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
void Xenon::Input::disableCursor() { glfwSetInputMode(static_cast<GLFWwindow*>(s_window), GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

std::pair<float, float> Xenon::Input::getMouseScreenPosition() {
	return {s_xMousePosition, s_yMousePosition};
}

void Xenon::Input::init(void* window) {
	static bool singletonCheck = false;
	if(singletonCheck) throw std::runtime_error("Xenon::Input was initialized twice");
	singletonCheck = true;
	s_keyStateMap.fill(0);
	s_window = window;
}

void Xenon::Input::resetStickyKeys() {
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

void Xenon::Input::proccesEvents(Xenon::Input::Action act, int GLFWKeyCode) {
	auto inx = static_cast<size_t>(c_GLFWKeyToXenonKey.at(GLFWKeyCode));
	auto actInt = act;
	s_keyStateMap.at(inx) |= UINT8(actInt + 1);
	s_keyStateMap.at(inx) &= UINT8(~UINT8(4));
	s_keyStateMap.at(inx) |= UINT8(UINT8(static_cast<int>(actInt == 0u)) << 2u); 
}

void Xenon::Input::proccesEvents(float xpos, float ypos) { 
	s_xMousePosition = xpos; 
	s_yMousePosition = ypos; 
}



