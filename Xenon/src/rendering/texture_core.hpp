#include <string>

namespace Core {
	class Texture {
	public:
		Texture(const std::string& path, int32_t minFilter, int32_t magFilter, int32_t wrapS, int32_t wrapT);
		~Texture();
		Texture(const Texture& oth);
		Texture& operator=(const Texture& oth);
		Texture(Texture&& oth); 
		Texture& operator=(Texture&& oth);

		int32_t getWidth();
		int32_t getHeigth();
		void bind();
		static void unbind();


	private:
		uint32_t m_ID;
		int32_t m_width, m_height;
	};

	//TODO
	class TextureAtlas {
	public:
		TextureAtlas();
		~TextureAtlas();
		TextureAtlas(const TextureAtlas& oth) = delete; //TODO Implementing all of these probably is a good idea
		TextureAtlas& operator=(const TextureAtlas& oth) = delete; 
		TextureAtlas(TextureAtlas&& oth) = delete; 
		TextureAtlas& operator=(TextureAtlas&& oth) = delete;

	private:

	};

}