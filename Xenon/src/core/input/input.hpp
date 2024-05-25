#ifndef CORE_INPUT_HPP
#define CORE_INPUT_HPP

#include "api.h"
#include <cstdint>

namespace Xenon {
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
		Np_Devide, Np_Multiply, Np_Minus, Np_Add, Np_Enter,
		Back_Quote, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25, Print_Screen, Np_Decimal, Scroll_lock, Num_lock, Backspace
	};
	// These keys are not implemented bcoz i dont think we will need them
	// world 1
	// world 2
	// right super
	// last

	[[nodiscard]] static bool XAPI getKeyPress(Key key);
	[[nodiscard]] static bool XAPI getKeyRelese(Key key);
	[[nodiscard]] static bool XAPI getKeyHold(Key key);
private:
	Input();
	constexpr static int s_keyAmmount = 117;
	static bool s_singletonCheck;
	// 0th bit - press, 1st bit - relese, 2nd bit - hold, 3rd bit - resetPress, 4th bit - resetRelese 
	static XAPI int8_t s_keyStateMap[s_keyAmmount];
	
	static void init();
	//If a key state was not read for two frames it is reset
	static void resetStickyKeys();
	enum class Action { Press, Relese };
	static void proccesEvents(Action act, int GLFWKeyCode);

	friend class Application;
};
}

#endif
