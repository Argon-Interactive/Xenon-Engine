#include "texture_core.hpp" 
#include <logger.hpp>
#include <stb_image.h>
#include <glad/glad.h>

Core::Texture::Texture(const std::string& path, int32_t minFilter, int32_t magFilter, int32_t wrapS, int32_t wrapT)
	:m_ID(0), m_width(0), m_height(0)
{
	int width = 0, height = 0, bpp = 0;
	stbi_set_flip_vertically_on_load(true);
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	if (!data) { XN_LOG_ERR("Texture at {0} failed to load", path); }
	else {
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
}

Core::Texture::~Texture() { if (m_ID != 0) { glDeleteTextures(1, &m_ID); } }

Core::Texture::Texture(const Texture& oth)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	int32_t param;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	uint8_t* data = new uint8_t[oth.m_width * oth.m_height * 4 * sizeof(uint8_t)];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, oth.m_width, oth.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete[] data;
	m_width = oth.m_width;
	m_height = oth.m_height;
	//This look kinda wrong but i have no way of testing this right now
}

Core::Texture& Core::Texture::operator=(const Texture& oth)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	int32_t param;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	uint8_t* data = new uint8_t[oth.m_width * oth.m_height * 4 * sizeof(uint8_t)];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, oth.m_width, oth.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete[] data;
	m_width = oth.m_width;
	m_height = oth.m_height;
	//This look kinda wrong but i have no way of testing this right now
	return *this;
}

Core::Texture::Texture(Texture&& oth) :m_ID(oth.m_ID), m_width(oth.m_width), m_height(oth.m_height) { oth.m_ID = 0; }

[[noexcept]] Core::Texture& Core::Texture::operator=(Texture&& oth) { 
	m_ID = oth.m_ID; 
	m_width = oth.m_width;
	m_height = oth.m_height;
	oth.m_ID = 0; 
	return *this; 
}
