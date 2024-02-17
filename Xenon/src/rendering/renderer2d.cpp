#include "renderer2d.h"
#include "../devTools/logger.hpp"
#include "../dataStructures.h"

#include"renderer2d.h"
#include<stb_image.h>
#include<glad.h>
#include<glm/glm.hpp>




////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Texture
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Xenon::Texture::Texture(std::string filepath)
	:m_ID(0)
{
	int width = 0; int height = 0; int bpp = 0;
	stbi_set_flip_vertically_on_load(1);
	uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);
	if (data == nullptr) { XN_LOG_ERR("Faild to load texture: \"{0}\"", filepath); return; }
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}
Xenon::Texture::~Texture() { if (m_ID != 0) { glDeleteTextures(1, &m_ID); }}
void Xenon::Texture::bind(uint32_t slot) { glActiveTexture(GL_TEXTURE0 + slot); glBindTexture(GL_TEXTURE_2D, m_ID); }
void Xenon::Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// StaticLayer2D
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Xenon::StaticLayer2D::StaticLayer2D()
{
	m_textureList.push_back(std::vector<Xenon::Texture*>());
	m_renderAmmouts.push_back(0);
	m_textureMap[nullptr] = -1;
}

Xenon::StaticLayer2D::~StaticLayer2D()
{

}

void Xenon::StaticLayer2D::addQuad(Xenon::Vec2 position, Xenon::Vec2 scale, float rotation, Xenon::Color color, Xenon::Texture* texture)
{
	if (m_isComputed) return;
	if (!m_textureMap.contains(texture)) { 
		m_textureMap[texture] = m_textureCounter++;
		m_textureList[m_textureLayer].push_back(texture);
		if (m_textureCounter == Xenon::Renderer2D::textureSlotsAmmout)
		{ m_textureLayer++; m_textureCounter = 0; m_textureList.push_back(std::vector<Xenon::Texture*>()); m_renderAmmouts.push_back(0); }
	}
	int texslot = m_textureMap[texture];
	int texadr = (texslot == -1) ? 0 : texslot;
	if (m_vertList.size() < texadr + 1) m_vertList.push_back(std::vector<Vertice2>());
	m_vertList[texadr].reserve(4);
	int col = color.getCompressed();
	m_vertList[texadr].push_back({position.x - scale.x / 2.f, position.y - scale.y / 2.f, 0.f, 0.f, col, texslot});
	m_vertList[texadr].push_back({position.x + scale.x / 2.f, position.y - scale.y / 2.f, 0.f, 0.f, col, texslot});
	m_vertList[texadr].push_back({position.x - scale.x / 2.f, position.y + scale.y / 2.f, 0.f, 0.f, col, texslot});
	m_vertList[texadr].push_back({position.x + scale.x / 2.f, position.y + scale.y / 2.f, 0.f, 0.f, col, texslot});
	m_renderAmmouts[m_textureLayer]++;
	m_quadAmmout++;
}
void Xenon::StaticLayer2D::compute()
{
	m_isComputed = true;
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertList.size() * sizeof(Vertice2), m_vertList.data(), GL_STATIC_DRAW);
	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float) + 2 * sizeof(int), (void*)(0 * sizeof(float)));
	// uv 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float) + 2 * sizeof(int), (void*)(2 * sizeof(float))); 
	// color
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, 4 * sizeof(float) + 2 * sizeof(int), (void*)(4 * sizeof(float))); 
	// texture slot 
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, 4 * sizeof(float) + 2 * sizeof(int), (void*)(4 * sizeof(float) + sizeof(int))); 

	m_textureMap.clear();
	m_vertList.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Renderer2D
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Xenon::Renderer2D::init()
{
	m_EBOSize = 0;
	m_EBO = 0;
	glGenBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_EBO);
}

void Xenon::Renderer2D::draw(const Core::Shader& shader, const Xenon::OrthographicCamera2D& camera, const StaticLayer2D& layer)
{
	shader.bind();
	glBindBuffer(GL_ARRAY_BUFFER, layer.m_VBO);
	if (layer.m_quadAmmout > m_EBOSize)
	{
		uint32_t* newData = genEBO(layer.m_quadAmmout);
		glDeleteBuffers(1, &m_EBO);
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_EBOSize * 6 * sizeof(uint32_t), newData, GL_STATIC_DRAW);
		delete[] newData;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	for (int i = 0; i < layer.m_renderAmmouts.size(); ++i)
	{
		for (int j = 0; j < layer.m_textureList[i].size(); ++j)
		{
			layer.m_textureList[i][j]->bind(j);
		}
		glDrawElements(GL_TRIANGLES, layer.m_renderAmmouts[i] * 6, GL_UNSIGNED_INT, nullptr);
	}
}

void Xenon::Renderer2D::draw(const DynamicLayer2D& layer)
{
}

uint32_t* Xenon::Renderer2D::genEBO(int quadAmmount)
{
	//TODO Make this funcion not regenerate the entire buffer everytime it is called

	uint32_t* EBOData = new uint32_t[quadAmmount * 6];
	for (int i = 0; i < quadAmmount * 4; i += 4)
	{
		EBOData[i] = i;
		EBOData[i + 1] = i;
		EBOData[i + 2] = i + 1;
		EBOData[i + 3] = i + 2;
		EBOData[i + 4] = i + 2;
		EBOData[i + 5] = i + 3;
	}
	m_EBOSize = quadAmmount;
	return EBOData;
}

int Xenon::Renderer2D::textureSlotsAmmout;
uint32_t  Xenon::Renderer2D::m_EBO;
uint32_t  Xenon::Renderer2D::m_EBOSize;
