#ifndef INPUT
#define INPUT
#include "../api.h"
#include <utility>

namespace Xenon {
	enum key {
		Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M,
		semicolon, quotation_mark, comma, period, dash, equals, shlash, backshlash, brackets_open, brackets_close,
		f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
		nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr0,
		np1, np2, np3, np4, np5, np6, np7, np8, np9, np0,
		np_shlash, np_multiply, np_subtract, np_add, np_enter, np_decimal,
		shift, rshift, control, rcontrol, alr, ralt, tab, caps_lock, esc, del, backspace, home, end, numlock, space, enter,
		up, down, left, right
	};
	
	class XAPI Input {
		public:	
			~Input() = default;
			Input(const Input&) = delete;
			Input& operator=(const Input&) = delete;
			Input(Input&&) = delete;
			Input& operator=(Input&&) = delete;

			static Input& getInstance();

			bool isKeyPressed();
			bool isKeyHeld();
			char getLastKey();
			std::pair<float, float> getMousePos();

		private:
			Input() = default;
			static Input m_instance;
			const char charList[] = {};
			const char shiftCharList[] = {};
	};
}
#endif // !INPUT
