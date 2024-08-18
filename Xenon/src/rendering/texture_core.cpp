#include "texture_core.hpp" 
#include "devTools/logger_core.hpp"
#include <stb_image.h>
#include <glad/glad.h>

Core::Texture2D::Texture2D(const std::string& path, int32_t minFilter, int32_t magFilter, int32_t wrapS, int32_t wrapT)
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
		m_width = width;
		m_height = height;
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
}

Core::Texture2D::~Texture2D() { if (m_ID != 0) { glDeleteTextures(1, &m_ID); } }

/**/
Core::Texture2D::Texture2D(const Texture2D& oth)
	:m_ID(0), m_width(oth.m_width), m_height(oth.m_height)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	int32_t param = 0;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	auto* data = new uint8_t[static_cast<uint32_t>(m_width * m_height) * 4 * sizeof(uint8_t)];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete[] data;
	//NOTE: This look kinda wrong but i have no way of testing this right now
}

Core::Texture2D& Core::Texture2D::operator=(const Texture2D& oth)
{
	m_ID = 0;
	m_width = oth.m_width;
	m_height = oth.m_height;
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	int32_t param = 0;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	uint8_t* data = new uint8_t[static_cast<uint32_t>(m_width * m_height) * 4 * sizeof(uint8_t)];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete[] data;
	//NOTE: This look kinda wrong but i have no way of testing this right now
	return *this;
}

Core::Texture2D::Texture2D(Texture2D&& oth) noexcept :m_ID(oth.m_ID), m_width(oth.m_width), m_height(oth.m_height) { oth.m_ID = 0; }

[[nodiscard]] Core::Texture2D& Core::Texture2D::operator=(Texture2D&& oth) noexcept { 
	m_ID = oth.m_ID; 
	m_width = oth.m_width;
	m_height = oth.m_height;
	oth.m_ID = 0; 
	return *this; 
}

int32_t Core::Texture2D::getWidth() const { return m_width; }
int32_t Core::Texture2D::getHeigth() const { return m_height; }

void Core::Texture2D::bind(uint32_t slot) { glActiveTexture(GL_TEXTURE0 + slot); glBindTexture(GL_TEXTURE_2D, m_ID); }
void Core::Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
