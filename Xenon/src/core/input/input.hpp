#ifndef CORE_INPUT_HPP
#define CORE_INPUT_HPP

#include "api.h"

namespace Xenon {
class Input {
public:
	Input();
	~Input() = default;
	Input(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(const Input&&) = delete;

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

	bool XAPI getKeyPress();
	bool XAPI getKeyRelesed();
	bool XAPI getKeyHeld();
private:
	static bool s_singletonCheck;
	bool m_pressedMap[121];
	bool m_relesedMap[121];
	bool m_heldMap[121];
};
}

#endif
