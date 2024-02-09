#pragma once
#include<glm/glm.hpp>

namespace core {

	struct color
	{
		uint8_t r, g, b, a;
		color() {}
		color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}
		color operator+(color col)
		{ return { r + col.r, g + col.g, b + col.b, a + col.a }; }
		color operator+=(color col) 
		{ r += col.r; g += col.g; b += col.b; a += col.a; return *this; }
		color operator-(color col)
		{ return { r - col.r, g - col.g, b - col.b, a - col.a }; }
		color operator-=(color col) 
		{ r -= col.r; g -= col.g; b -= col.b; a -= col.a; return *this; }
		color operator*(float mul)
		{ return { 
			static_cast<uint8_t>(static_cast<float>(r) * mul),
			static_cast<uint8_t>(static_cast<float>(g) * mul),
			static_cast<uint8_t>(static_cast<float>(b) * mul),
			static_cast<uint8_t>(static_cast<float>(a) * mul)
		}; }
		color operator*=(float mul)
		{
			r = static_cast<uint8_t>(static_cast<float>(r) * mul);
			g = static_cast<uint8_t>(static_cast<float>(g) * mul);
			b = static_cast<uint8_t>(static_cast<float>(b) * mul);
			a = static_cast<uint8_t>(static_cast<float>(a) * mul);
			return *this;
		}
		color operator/(float div)
		{ return { 
			static_cast<uint8_t>(static_cast<float>(r) / div),
			static_cast<uint8_t>(static_cast<float>(g) / div),
			static_cast<uint8_t>(static_cast<float>(b) / div),
			static_cast<uint8_t>(static_cast<float>(a) / div)
		}; }
		color operator/=(float div)
		{
			r = static_cast<uint8_t>(static_cast<float>(r) / div);
			g = static_cast<uint8_t>(static_cast<float>(g) / div);
			b = static_cast<uint8_t>(static_cast<float>(b) / div);
			a = static_cast<uint8_t>(static_cast<float>(a) / div);
			return *this;
		}
		bool operator==(color col)
		{ return (r == col.r) && (g == col.g) && (b == col.b) && (a == col.a); }
	};

	struct vertice2
	{
		glm::vec2 pos;
		core::color color;
		glm::vec2 uv;
		uint32_t texID;
	};
}