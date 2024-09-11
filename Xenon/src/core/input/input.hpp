#ifndef CORE_INPUT_HPP
#define CORE_INPUT_HPP

#include <array>
#include <cstdint>

#define XN_KEY_A Core::Input::Key::A
#define XN_KEY_B Core::Input::Key::B
#define XN_KEY_C Core::Input::Key::C
#define XN_KEY_D Core::Input::Key::D
#define XN_KEY_E Core::Input::Key::E
#define XN_KEY_F Core::Input::Key::F
#define XN_KEY_G Core::Input::Key::G
#define XN_KEY_H Core::Input::Key::H
#define XN_KEY_I Core::Input::Key::I
#define XN_KEY_J Core::Input::Key::J
#define XN_KEY_K Core::Input::Key::K
#define XN_KEY_L Core::Input::Key::L
#define XN_KEY_M Core::Input::Key::M
#define XN_KEY_N Core::Input::Key::N
#define XN_KEY_O Core::Input::Key::O
#define XN_KEY_P Core::Input::Key::P
#define XN_KEY_Q Core::Input::Key::Q
#define XN_KEY_R Core::Input::Key::R
#define XN_KEY_S Core::Input::Key::S
#define XN_KEY_T Core::Input::Key::T
#define XN_KEY_U Core::Input::Key::U
#define XN_KEY_V Core::Input::Key::V
#define XN_KEY_W Core::Input::Key::W
#define XN_KEY_X Core::Input::Key::X
#define XN_KEY_Y Core::Input::Key::Y
#define XN_KEY_Z Core::Input::Key::Z
#define XN_KEY_NP0 Core::Input::Key::Np0
#define XN_KEY_NP1 Core::Input::Key::Np1
#define XN_KEY_NP2 Core::Input::Key::Np2
#define XN_KEY_NP3 Core::Input::Key::Np3
#define XN_KEY_NP4 Core::Input::Key::Np4
#define XN_KEY_NP5 Core::Input::Key::Np5
#define XN_KEY_NP6 Core::Input::Key::Np6
#define XN_KEY_NP7 Core::Input::Key::Np7
#define XN_KEY_NP8 Core::Input::Key::Np8
#define XN_KEY_NP9 Core::Input::Key::Np9
#define XN_KEY_0 Core::Input::Key::Nr0
#define XN_KEY_1 Core::Input::Key::Nr1
#define XN_KEY_2 Core::Input::Key::Nr2
#define XN_KEY_3 Core::Input::Key::Nr3
#define XN_KEY_4 Core::Input::Key::Nr4
#define XN_KEY_5 Core::Input::Key::Nr5
#define XN_KEY_6 Core::Input::Key::Nr6
#define XN_KEY_7 Core::Input::Key::Nr7
#define XN_KEY_8 Core::Input::Key::Nr8
#define XN_KEY_9 Core::Input::Key::Nr9
#define XN_KEY_ENTER Core::Input::Key::Enter
#define XN_KEY_SHIFT Core::Input::Key::Shift
#define XN_KEY_CONTROL Core::Input::Key::Control
#define XN_KEY_ALT Core::Input::Key::Alt
#define XN_KEY_CAPS_LOCK Core::Input::Key::Caps_Lock
#define XN_KEY_TAB Core::Input::Key::Tab
#define XN_KEY_ESC Core::Input::Key::Esc
#define XN_KEY_WINDOWS Core::Input::Key::Windows
#define XN_KEY_COMMAND Core::Input::Key::Command
#define XN_KEY_SUPER Core::Input::Key::Super
#define XN_KEY_ALT_GR Core::Input::Key::Alt_Gr
#define XN_KEY_RIGHT_CONTROL Core::Input::Key::Right_Control
#define XN_KEY_RIGHT_SHIFT Core::Input::Key::Right_Shift
#define XN_KEY_SPACE Core::Input::Key::Space
#define XN_KEY_APOSTROPHE Core::Input::Key::Apostrophe
#define XN_KEY_UP Core::Input::Key::Up
#define XN_KEY_DOWN Core::Input::Key::Down
#define XN_KEY_LEFT Core::Input::Key::Left
#define XN_KEY_RIGHT Core::Input::Key::Right
#define XN_KEY_F1 Core::Input::Key::F1
#define XN_KEY_F2 Core::Input::Key::F2
#define XN_KEY_F3 Core::Input::Key::F3
#define XN_KEY_F4 Core::Input::Key::F4
#define XN_KEY_F5 Core::Input::Key::F5
#define XN_KEY_F6 Core::Input::Key::F6
#define XN_KEY_F7 Core::Input::Key::F7
#define XN_KEY_F8 Core::Input::Key::F8
#define XN_KEY_F9 Core::Input::Key::F9
#define XN_KEY_F10 Core::Input::Key::F10
#define XN_KEY_F11 Core::Input::Key::F11
#define XN_KEY_F12 Core::Input::Key::F12
#define XN_KEY_F13 Core::Input::Key::F13
#define XN_KEY_F14 Core::Input::Key::F14
#define XN_KEY_F15 Core::Input::Key::F15
#define XN_KEY_F16 Core::Input::Key::F16
#define XN_KEY_F17 Core::Input::Key::F17
#define XN_KEY_F18 Core::Input::Key::F18
#define XN_KEY_F19 Core::Input::Key::F19
#define XN_KEY_F20 Core::Input::Key::F20
#define XN_KEY_F21 Core::Input::Key::F21
#define XN_KEY_F22 Core::Input::Key::F22
#define XN_KEY_F23 Core::Input::Key::F23
#define XN_KEY_F24 Core::Input::Key::F24
#define XN_KEY_F25 Core::Input::Key::F25
#define XN_KEY_INSERT Core::Input::Key::Insert
#define XN_KEY_DELETE Core::Input::Key::Delete
#define XN_KEY_HOME Core::Input::Key::Home
#define XN_KEY_END Core::Input::Key::End
#define XN_KEY_COMMA Core::Input::Key::Comma
#define XN_KEY_DASH Core::Input::Key::Dash
#define XN_KEY_PERIOD Core::Input::Key::Period
#define XN_KEY_SLASH Core::Input::Key::Slash
#define XN_KEY_SEMICOLON Core::Input::Key::Semicolon
#define XN_KEY_EQUALS Core::Input::Key::Equals
#define XN_KEY_PAGE_UP Core::Input::Key::PageUp
#define XN_KEY_PAGE_DOWN Core::Input::Key::PageDown
#define XN_KEY_PAUSE Core::Input::Key::Pause
#define XN_KEY_LEFT_BRAKET Core::Input::Key::Left_Braket
#define XN_KEY_RIGHT_BRAKET Core::Input::Key::Right_Braket
#define XN_KEY_BACKSLASH Core::Input::Key::Backslash
#define XN_KEY_NP_DEVIDE Core::Input::Key::Np_Devide
#define XN_KEY_NP_MULTIPLY Core::Input::Key::Np_Devide
#define XN_KEY_NP_SUBTRACT Core::Input::Key::Np_Subtract
#define XN_KEY_NP_ADD Core::Input::Key::Np_Add
#define XN_KEY_NP_ENTER Core::Input::Key::Np_Enter
#define XN_KEY_Np_DECIMAL Core::Input::Key::Np_Decimal
#define XN_KEY_BACK_QUOTE Core::Input::Key::Back_Quote
#define XN_KEY_PRINT_SCREEN Core::Input::Key::Print_Screen
#define XN_KEY_SCROLL_LOCK Core::Input::Key::Scroll_Lock
#define XN_KEY_NUM_LOCK Core::Input::Key::Num_Lock
#define XN_KEY_BACKSPACE Core::Input::Key::Backspace
#define XN_KEY_LMB Core::Input::Key::Left_Mouse_Button
#define XN_KEY_RMB Core::Input::Key::Right_Mouse_Button
#define XN_KEY_MMB Core::Input::Key::Middle_Mouse_Button
#define XN_KEY_MB1 Core::Input::Key::Left_Mouse_Button
#define XN_KEY_MB2 Core::Input::Key::Right_Mouse_Button
#define XN_KEY_MB3 Core::Input::Key::Middle_Mouse_Button
#define XN_KEY_MB4 Core::Input::Key::Mouse_Button_4
#define XN_KEY_MB5 Core::Input::Key::Mouse_Button_5
#define XN_KEY_MB6 Core::Input::Key::Mouse_Button_6
#define XN_KEY_MB7 Core::Input::Key::Mouse_Button_7
#define XN_KEY_MB8 Core::Input::Key::Mouse_Button_8

namespace Core {

class Input {
public:
	~Input() = default;
	Input(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(const Input&&) = delete;

	/* DO NOT TOUCH. IT WAS METICULOUSLY CRAFTED BY HAND */
	enum Key {
		Np1, Np2, Np3, Np4, Np5, Np6, Np7, Np8, Np9, Np0,
		Enter, Shift, Control, Alt, Caps_Lock, Tab, Esc, Windows, Command, Super, Alt_Gr, Right_Control, Right_Shift,
		Up, Down, Left, Right,
		F1, F2, F3, F4, F5, Space, F7, F8, F9, F10, F11, F12, Apostrophe,
		Insert, Delete, Home, End,
		Comma, Dash, Period, Slash, Nr0, Nr1, Nr2, Nr3, Nr4, Nr5, Nr6, Nr7, Nr8, Nr9, F6, Semicolon,
		F13, Equals, PageUp, PageDown, Pause, 
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, Left_Braket, Backslash, Right_Braket,
		Np_Devide, Np_Multiply, Np_Subtract, Np_Add, Np_Enter,
		Back_Quote, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, Print_Screen, Np_Decimal, Scroll_Lock, Num_Lock, Backspace, 
		Left_Mouse_Button, Right_Mouse_Button, Middle_Mouse_Button, Mouse_Button_4, Mouse_Button_5, Mouse_Button_6, Mouse_Button_7, Mouse_Button_8
	};
	// These keys are not implemented bcoz i dont think we will need them
	// world 1
	// world 2
	// right super
	// last

	[[nodiscard]] static bool getKeyPress(Key key);
	[[nodiscard]] static bool getKeyRelese(Key key);
	[[nodiscard]] static bool getKeyHold(Key key);
	static void disableCursor();
	static void enableCursor();
	[[nodiscard]] static std::pair<float, float> getMouseScreenPosition(); //Consider changeing std::pair to some Vector (Engine struct)
	static void resetStickyKeys();

private:
	Input();
	// 0th bit - press, 1st bit - release, 2nd bit - hold, 3rd bit - resetPress, 4th bit - resetRelease 
	inline static std::array<uint8_t, 125> s_keyStateMap; //changing the size of this array, change it also in cpp
	inline static void* s_window = nullptr;
	inline static float s_xMousePosition = 0.0f;
	inline static float s_yMousePosition = 0.0f;
	
	static void init(void* window);
	//If a key state was not read for two frames it is reset
	enum Action { Press, Relese };
	static void proccesEvents(Action act, int GLFWKeyCode);
	static void proccesEvents(float xpos, float ypos);
	

	friend class AppData;
	friend class Application;
};

}

#endif
