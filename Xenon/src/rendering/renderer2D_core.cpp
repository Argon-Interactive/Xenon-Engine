#include "renderer2D_core.hpp"
#include "logger.hpp"
#include <glad/glad.h>
#include<algorithm>

#define textureSlotsAmmount 32 //TODO Make it device dependent


glm::mat4 Core::Camera::getMatrix() const { return glm::mat4(); }

Core::OrthographicCamera::OrthographicCamera() :pos(0.f, 0.f, 0.f), size(100.f, 100.f) {}
Core::OrthographicCamera::OrthographicCamera(float x, float y, float z, float width, float height) :pos(x, y, z), size(width, height) {}
glm::mat4 Core::OrthographicCamera::getMatrix() const { return glm::ortho(pos.x, pos.x + size.x, pos.y, pos.y + size.y, -10.f, 10.f); }



Core::Renderer2D Core::Renderer2D::s_instance;

Core::Renderer2D::~Renderer2D()
{
	for (auto a : m_layers)
	{
		glDeleteBuffers(2, &(a.VBO));
		glDeleteVertexArrays(1, &(a.VAO));
	}
}

Core::Renderer2D& Core::Renderer2D::getInstance() { return s_instance; }

Xenon::ID Core::Renderer2D::createStaticLayer(Core::Quad quadList[], size_t quadListSize, std::shared_ptr<Core::Texture2D> textureList[], size_t textureListSize)
{
	if (textureListSize > textureSlotsAmmount) { XN_LOG_ERR("{0} exeeds the allowed ammount of textures per layer, the allowed ammount is {0}", textureListSize, textureSlotsAmmount); return 0; }
	Xenon::ID layerId = 0;
	if (m_freeIDList.size() < 5) { //This 5 is there to make sure that freshly deleted layer wont have its id immediately reused 
		m_layers.push_back({0, 0, 0});
		layerId = m_layers.size() - 1;

		m_textures.reserve(textureSlotsAmmount);
		//TODO Make sure this works bcoz im not 100% sure vector.reserve doesnt changes vector.size and vector.end
		for (int i = 0; i < 32; ++i) m_textures.emplace_back(nullptr);
		if (!textureList) { memcpy(&m_textures[m_textures.size() - textureSlotsAmmount], textureList, textureListSize * sizeof(std::shared_ptr<Core::Texture2D>)); }
	}
	else {
		layerId = m_freeIDList.front();
		m_freeIDList.pop_front();

		//TODO Check if the data is changed correctly
		std::fill_n(m_textures.begin() + layerId * textureSlotsAmmount, textureSlotsAmmount, std::shared_ptr<Core::Texture2D>(nullptr));
		if (!textureList) { memcpy(&m_textures[layerId * textureSlotsAmmount], textureList, textureListSize * sizeof(std::shared_ptr<Core::Texture2D>)); }
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
	glGenBuffers(2, &(buffers->VBO));

	glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);
	glBufferData(GL_ARRAY_BUFFER, quadListSize * 4 * sizeof(Core::Vertice), quadList, GL_STATIC_DRAW); //TODO Vertice should be implemented rn its just an int as a placeholder

	GLsizei stride = 10 * sizeof(float);
	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1); //uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2); //texId
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(3); //color
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadListSize * 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	buffers->size = quadListSize;

	delete[] indices;
	return layerId;
}

Xenon::ID Core::Renderer2D::createDynamicLayer()
{
	return Xenon::ID();
}

void Core::Renderer2D::deleteStaticLayer(Xenon::ID layerID)
{
	//opengl should automaticly ignore invalid ids
	if (std::find(m_freeIDList.begin(), m_freeIDList.end(), layerID) == std::end(m_freeIDList)) { XN_LOG_ERR("Static layer with the id: {0} was already deleted", layerID); return; }
	glDeleteBuffers(2, &(m_layers[layerID].VBO));
	glDeleteVertexArrays(1, &(m_layers[layerID].VAO));
	m_layers[layerID] = { 0, 0, 0 };
	m_freeIDList.push_back(layerID);
}

void Core::Renderer2D::deleteDynamicLayer(Xenon::ID LayerID)
{
}

void Core::Renderer2D::render(const Core::Camera& camera, Core::Shader& shader, Xenon::ID layerID) {
	//TODO Textures, and rendering without texture
	m_layerData* buffer = &m_layers[layerID];
	glBindVertexArray(buffer->VAO);
	glBindBuffer(GL_VERTEX_ARRAY, buffer->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);
	shader.bind();

	shader.setUniformMatrix4("u_projMatrix", camera.getMatrix());

	glDrawElements(GL_TRIANGLES, buffer->size * 6, GL_UNSIGNED_INT, nullptr);
}
