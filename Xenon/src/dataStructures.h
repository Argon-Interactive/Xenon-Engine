#pragma once

namespace Xenon {

	struct Color
	{
		uint8_t r, g, b, a;
		Color() {}
		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}
		Color operator+(Color col)
		{ return { static_cast<uint8_t>(r + col.r), static_cast<uint8_t>(g + col.g), static_cast<uint8_t>(b + col.b), static_cast<uint8_t>(a + col.a) }; }
		Color operator+=(Color col) 
		{ static_cast<uint8_t>(r += col.r); static_cast<uint8_t>(g += col.g); static_cast<uint8_t>(b += col.b); static_cast<uint8_t>(a += col.a); return *this; }
		Color operator-(Color col)
		{ return { static_cast<uint8_t>(r - col.r), static_cast<uint8_t>(g - col.g), static_cast<uint8_t>(b - col.b), static_cast<uint8_t>(a - col.a) }; }
		Color operator-=(Color col) 
		{ static_cast<uint8_t>(r -= col.r); static_cast<uint8_t>(g -= col.g); static_cast<uint8_t>(b -= col.b); static_cast<uint8_t>(a -= col.a); return *this; }
		Color operator*(float mul)
		{ return { 
			static_cast<uint8_t>(static_cast<float>(r) * mul),
			static_cast<uint8_t>(static_cast<float>(g) * mul),
			static_cast<uint8_t>(static_cast<float>(b) * mul),
			static_cast<uint8_t>(static_cast<float>(a) * mul)
		}; }
		Color operator*=(float mul)
		{
			r = static_cast<uint8_t>(static_cast<float>(r) * mul);
			g = static_cast<uint8_t>(static_cast<float>(g) * mul);
			b = static_cast<uint8_t>(static_cast<float>(b) * mul);
			a = static_cast<uint8_t>(static_cast<float>(a) * mul);
			return *this;
		}
		Color operator/(float div)
		{ return { 
			static_cast<uint8_t>(static_cast<float>(r) / div),
			static_cast<uint8_t>(static_cast<float>(g) / div),
			static_cast<uint8_t>(static_cast<float>(b) / div),
			static_cast<uint8_t>(static_cast<float>(a) / div)
		}; }
		Color operator/=(float div)
		{
			r = static_cast<uint8_t>(static_cast<float>(r) / div);
			g = static_cast<uint8_t>(static_cast<float>(g) / div);
			b = static_cast<uint8_t>(static_cast<float>(b) / div);
			a = static_cast<uint8_t>(static_cast<float>(a) / div);
			return *this;
		}
		bool operator==(Color col)
		{ return (r == col.r) && (g == col.g) && (b == col.b) && (a == col.a); }
		int getCompressed()
		{ return (static_cast<int>(r) | static_cast<int>(g) << 8 | static_cast<int>(b) << 16 | static_cast<int>(a) << 24); } //ABGR
	};

	struct Vec2
	{
		union {
			struct {
				float x, y;
			};
			struct {
				float u, v;
			};
		};
	};
}

