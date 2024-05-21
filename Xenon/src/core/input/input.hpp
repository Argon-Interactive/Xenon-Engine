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
		Np_Devide, Np_Multiply, Np_Minus, Np_Add, Np_Enter,
		Back_Quote, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, Print_Screen, Np_Decimal, Scroll_lock, Num_lock, Backspace
	};
	// 
	// world 1
	// world 2
	// right super
	// last

	bool XAPI getKeyPress();
	bool XAPI getKeyRelesed();
	bool XAPI getKeyHeld();
private:
	constexpr static int s_keyAmmount = 117;
	static bool s_singletonCheck;
	bool m_pressedMap[s_keyAmmount];
	bool m_relesedMap[s_keyAmmount];
	bool m_heldMap[s_keyAmmount];
};
}

#endif
