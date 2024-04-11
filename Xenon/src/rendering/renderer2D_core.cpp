#include "renderer2D_core.hpp"
#include "logger.hpp"
#include <glad/glad.h>
#include<algorithm>

#define vertice int
#define textureSlotsAmmount 32 //TODO Make it device dependent

Core::Renderer2D::~Renderer2D()
{

}

Core::Renderer2D& Core::Renderer2D::getInstance() { return s_instance; }

Xenon::ID Core::Renderer2D::createStaticLayer(Core::Quad quadList[], size_t quadListSize, Core::Texture2D textureList[], size_t textureListSize)
{
	if (textureListSize > textureSlotsAmmount) { XN_LOG_ERR("{0} exeeds the allowed ammount of textures per layer, the allowed ammount is {0}", textureListSize, textureSlotsAmmount); return 0; }
	Xenon::ID layerId = 0;
	if (m_freeIDList.size() < 5) { //This 5 is there to make sure that freshly deleted layer wont have its id immediately reused 
		m_layers.push_back({0, 0, 0});
		layerId = m_layers.size() - 1;

		m_textures.reserve(textureSlotsAmmount);
		//TODO Make sure this works bcoz im not 100% sure vector.reserve doesnt changes vector.size and vector.end
		std::fill_n(m_textures.end(), textureSlotsAmmount, std::shared_ptr<Core::Texture2D>(nullptr));
		memcpy(&m_textures[m_textures.size()], textureList, textureListSize * sizeof(std::shared_ptr<Core::Texture2D>));
	}
	else {
		layerId = m_freeIDList.front();
		m_freeIDList.pop_front();

		//TODO Check if the data is changed correctly
		std::fill_n(m_textures.begin() + layerId * textureSlotsAmmount, textureSlotsAmmount, std::shared_ptr<Core::Texture2D>(nullptr));
		memcpy(&m_textures[layerId * textureSlotsAmmount], textureList, textureListSize * sizeof(std::shared_ptr<Core::Texture2D>)); 
	}
	uint32_t* indices = new uint32_t[quadListSize * 6];
	for (int i = 0; i < quadListSize; ++i) {
		indices[0 + i * 6] = 0 + i * 4;
		indices[1 + i * 6] = 1 + i * 4;
		indices[2 + i * 6] = 2 + i * 4;
		indices[3 + i * 6] = 2 + i * 4;
		indices[4 + i * 6] = 3 + i * 4;
		indices[5 + i * 6] = 1 + i * 4;
	}

	m_layerData* buffers = &m_layers[layerId];

	glCreateVertexArrays(1, &(buffers->VAO));
	glBindVertexArray(buffers->VAO);
	glGenBuffers(2, &(buffers->VBO)); //TODO This should generate VBO and EBO but i have no way of testing if this works rn (it should be tested)

	glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);
	glBufferData(GL_ARRAY_BUFFER, quadListSize * 4 * sizeof(vertice), quadList, GL_STATIC_DRAW); //TODO Vertice should be implemented rn its just an int as a placeholder (define at the beginning of the files)

	GLsizei stride = 5 * sizeof(float) + 1 * sizeof(uint32_t) + 4 * sizeof(uint8_t);
	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1); //uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2); //texId
	glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(3); //color
	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, stride, (void*)(5 * sizeof(float) + 1 * sizeof(uint32_t)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadListSize * 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] indices;
	return layerId;
}

Xenon::ID Core::Renderer2D::createDynamicLayer()
{
	return Xenon::ID();
}

void Core::Renderer2D::deleteStaticLayer(Xenon::ID ID)
{
	//opengl should automaticly ignore invalid id's
	if (std::find(m_freeIDList.begin(), m_freeIDList.end(), ID) == std::end(m_freeIDList)) { XN_LOG_ERR("Static layer with the id: {0} was already deleted", ID); return; }
	glDeleteBuffers(2, &(m_layers[ID].VBO));
	glDeleteVertexArrays(1, &(m_layers[ID].VAO));
	m_freeIDList.push_back(ID);
}

void Core::Renderer2D::deleteDynamicLayer(Xenon::ID LayerID)
{
}
