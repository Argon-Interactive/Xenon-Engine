#pragma once
#include <cstdint>
#include <string>
#include"../api.h"

namespace Core {
	class XAPI Texture2D {
	public:
		Texture2D(const std::string& path, int32_t minFilter, int32_t magFilter, int32_t wrapS, int32_t wrapT);
		~Texture2D();
		Texture2D(const Texture2D& oth);
		Texture2D& operator=(const Texture2D& oth);
		Texture2D(Texture2D&& oth); 
		Texture2D& operator=(Texture2D&& oth);

		int32_t getWidth();
		int32_t getHeigth();
		void bind(uint32_t slot = 0);
		static void unbind();


	private:
		uint32_t m_ID;
		int32_t m_width, m_height;
	};

	//TODO
	class Texture2DAtlas {
	public:
		Texture2DAtlas();
		~Texture2DAtlas();
		Texture2DAtlas(const Texture2DAtlas& oth) = delete; //TODO Implementing all of these probably is a good idea
		Texture2DAtlas& operator=(const Texture2DAtlas& oth) = delete; 
		Texture2DAtlas(Texture2DAtlas&& oth) = delete; 
		Texture2DAtlas& operator=(Texture2DAtlas&& oth) = delete;

	private:

	};

}
