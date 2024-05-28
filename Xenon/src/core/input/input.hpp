#ifndef CORE_INPUT_HPP
#define CORE_INPUT_HPP

#include "api.h"
#include <array>
#include <cstdint>

#define XN_KEY_A Xenon::Input::Key::A
#define XN_KEY_B Xenon::Input::Key::B
#define XN_KEY_C Xenon::Input::Key::C
#define XN_KEY_D Xenon::Input::Key::D
#define XN_KEY_E Xenon::Input::Key::E
#define XN_KEY_F Xenon::Input::Key::F
#define XN_KEY_G Xenon::Input::Key::G
#define XN_KEY_H Xenon::Input::Key::H
#define XN_KEY_I Xenon::Input::Key::I
#define XN_KEY_J Xenon::Input::Key::J
#define XN_KEY_K Xenon::Input::Key::K
#define XN_KEY_L Xenon::Input::Key::L
#define XN_KEY_M Xenon::Input::Key::M
#define XN_KEY_N Xenon::Input::Key::N
#define XN_KEY_O Xenon::Input::Key::O
#define XN_KEY_P Xenon::Input::Key::P
#define XN_KEY_Q Xenon::Input::Key::Q
#define XN_KEY_R Xenon::Input::Key::R
#define XN_KEY_S Xenon::Input::Key::S
#define XN_KEY_T Xenon::Input::Key::T
#define XN_KEY_U Xenon::Input::Key::U
#define XN_KEY_V Xenon::Input::Key::V
#define XN_KEY_W Xenon::Input::Key::W
#define XN_KEY_X Xenon::Input::Key::X
#define XN_KEY_Y Xenon::Input::Key::Y
#define XN_KEY_Z Xenon::Input::Key::Z
#define XN_KEY_NP0 Xenon::Input::Key::Np0
#define XN_KEY_NP1 Xenon::Input::Key::Np1
#define XN_KEY_NP2 Xenon::Input::Key::Np2
#define XN_KEY_NP3 Xenon::Input::Key::Np3
#define XN_KEY_NP4 Xenon::Input::Key::Np4
#define XN_KEY_NP5 Xenon::Input::Key::Np5
#define XN_KEY_NP6 Xenon::Input::Key::Np6
#define XN_KEY_NP7 Xenon::Input::Key::Np7
#define XN_KEY_NP8 Xenon::Input::Key::Np8
#define XN_KEY_NP9 Xenon::Input::Key::Np9
#define XN_KEY_0 Xenon::Input::Key::Nr0
#define XN_KEY_1 Xenon::Input::Key::Nr1
#define XN_KEY_2 Xenon::Input::Key::Nr2
#define XN_KEY_3 Xenon::Input::Key::Nr3
#define XN_KEY_4 Xenon::Input::Key::Nr4
#define XN_KEY_5 Xenon::Input::Key::Nr5
#define XN_KEY_6 Xenon::Input::Key::Nr6
#define XN_KEY_7 Xenon::Input::Key::Nr7
#define XN_KEY_8 Xenon::Input::Key::Nr8
#define XN_KEY_9 Xenon::Input::Key::Nr9
#define XN_KEY_ENTER Xenon::Input::Key::Enter
#define XN_KEY_SHIFT Xenon::Input::Key::Shift
#define XN_KEY_CONTROL Xenon::Input::Key::Control
#define XN_KEY_ALT Xenon::Input::Key::Alt
#define XN_KEY_CAPS_LOCK Xenon::Input::Key::Caps_Lock
#define XN_KEY_TAB Xenon::Input::Key::Tab
#define XN_KEY_ESC Xenon::Input::Key::Esc
#define XN_KEY_WINDOWS Xenon::Input::Key::Windows
#define XN_KEY_COMMAND Xenon::Input::Key::Command
#define XN_KEY_SUPER Xenon::Input::Key::Super
#define XN_KEY_ALT_GR Xenon::Input::Key::Alt_Gr
#define XN_KEY_RIGHT_CONTROL Xenon::Input::Key::Right_Control
#define XN_KEY_RIGHT_SHIFT Xenon::Input::Key::Right_Shift
#define XN_KEY_SPACE Xenon::Input::Key::Space
#define XN_KEY_APOSTROPHE Xenon::Input::Key::Apostrophe
#define XN_KEY_UP Xenon::Input::Key::Up
#define XN_KEY_DOWN Xenon::Input::Key::Down
#define XN_KEY_LEFT Xenon::Input::Key::Left
#define XN_KEY_RIGHT Xenon::Input::Key::Right
#define XN_KEY_F1 Xenon::Input::Key::F1
#define XN_KEY_F2 Xenon::Input::Key::F2
#define XN_KEY_F3 Xenon::Input::Key::F3
#define XN_KEY_F4 Xenon::Input::Key::F4
#define XN_KEY_F5 Xenon::Input::Key::F5
#define XN_KEY_F6 Xenon::Input::Key::F6
#define XN_KEY_F7 Xenon::Input::Key::F7
#define XN_KEY_F8 Xenon::Input::Key::F8
#define XN_KEY_F9 Xenon::Input::Key::F9
#define XN_KEY_F10 Xenon::Input::Key::F10
#define XN_KEY_F11 Xenon::Input::Key::F11
#define XN_KEY_F12 Xenon::Input::Key::F12
#define XN_KEY_F13 Xenon::Input::Key::F13
#define XN_KEY_F14 Xenon::Input::Key::F14
#define XN_KEY_F15 Xenon::Input::Key::F15
#define XN_KEY_F16 Xenon::Input::Key::F16
#define XN_KEY_F17 Xenon::Input::Key::F17
#define XN_KEY_F18 Xenon::Input::Key::F18
#define XN_KEY_F19 Xenon::Input::Key::F19
#define XN_KEY_F20 Xenon::Input::Key::F20
#define XN_KEY_F21 Xenon::Input::Key::F21
#define XN_KEY_F22 Xenon::Input::Key::F22
#define XN_KEY_F23 Xenon::Input::Key::F23
#define XN_KEY_F24 Xenon::Input::Key::F24
#define XN_KEY_F25 Xenon::Input::Key::F25
#define XN_KEY_INSERT Xenon::Input::Key::Insert
#define XN_KEY_DELETE Xenon::Input::Key::Delete
#define XN_KEY_HOME Xenon::Input::Key::Home
#define XN_KEY_END Xenon::Input::Key::End
#define XN_KEY_COMMA Xenon::Input::Key::Comma
#define XN_KEY_DASH Xenon::Input::Key::Dash
#define XN_KEY_PERIOD Xenon::Input::Key::Period
#define XN_KEY_SLASH Xenon::Input::Key::Slash
#define XN_KEY_SEMICOLON Xenon::Input::Key::Semicolon
#define XN_KEY_EQUALS Xenon::Input::Key::Equals
#define XN_KEY_PAGE_UP Xenon::Input::Key::PageUp
#define XN_KEY_PAGE_DOWN Xenon::Input::Key::PageDown
#define XN_KEY_PAUSE Xenon::Input::Key::Pause
#define XN_KEY_LEFT_BRAKET Xenon::Input::Key::Left_Braket
#define XN_KEY_RIGHT_BRAKET Xenon::Input::Key::Right_Braket
#define XN_KEY_BACKSLASH Xenon::Input::Key::Backslash
#define XN_KEY_NP_DEVIDE Xenon::Input::Key::Np_Devide
#define XN_KEY_NP_MULTIPLY Xenon::Input::Key::Np_Devide
#define XN_KEY_NP_SUBTRACT Xenon::Input::Key::Np_Subtract
#define XN_KEY_NP_ADD Xenon::Input::Key::Np_Add
#define XN_KEY_NP_ENTER Xenon::Input::Key::Np_Enter
#define XN_KEY_Np_DECIMAL Xenon::Input::Key::Np_Decimal
#define XN_KEY_BACK_QUOTE Xenon::Input::Key::Back_Quote
#define XN_KEY_PRINT_SCREEN Xenon::Input::Key::Print_Screen
#define XN_KEY_SCROLL_LOCK Xenon::Input::Key::Scroll_Lock
#define XN_KEY_NUM_LOCK Xenon::Input::Key::Num_Lock
#define XN_KEY_BACKSPACE Xenon::Input::Key::Backspace
#define XN_KEY_LMB Xenon::Input::Key::Left_Mouse_Button
#define XN_KEY_RMB Xenon::Input::Key::Right_Mouse_Button
#define XN_KEY_MMB Xenon::Input::Key::Middle_Mouse_Button
#define XN_KEY_MB1 Xenon::Input::Key::Left_Mouse_Button
#define XN_KEY_MB2 Xenon::Input::Key::Right_Mouse_Button
#define XN_KEY_MB3 Xenon::Input::Key::Middle_Mouse_Button
#define XN_KEY_MB4 Xenon::Input::Key::Mouse_Button_4
#define XN_KEY_MB5 Xenon::Input::Key::Mouse_Button_5
#define XN_KEY_MB6 Xenon::Input::Key::Mouse_Button_6
#define XN_KEY_MB7 Xenon::Input::Key::Mouse_Button_7
#define XN_KEY_MB8 Xenon::Input::Key::Mouse_Button_8

namespace XAPI Xenon {
class Input {
public:
	~Input() = default;
	Input(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(const Input&&) = delete;

	/* DO NOT TOUCH. IT WAS METICULOUSLY CRAFTED BY HAND */
	enum class Key {
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

private:
	Input();
	static bool s_singletonCheck;
	// 0th bit - press, 1st bit - relese, 2nd bit - hold, 3rd bit - resetPress, 4th bit - resetRelese 
	static std::array<uint8_t, 125> s_keyStateMap; //changeing the size of whis array, change it also in cpp
	static void* s_window;
	static float s_xMousePosition;
	static float s_yMousePosition;
	
	static void init(void* window);
	//If a key state was not read for two frames it is reset
	static void resetStickyKeys();
	enum class Action { Press, Relese };
	static void proccesEvents(Action act, int GLFWKeyCode);
	static void proccesEvents(float xpos, float ypos);
	

	friend class Application;
};
}

#endif
