#pragma once
#include<array>
#include<vector>
#include<map>
#include<string>
#include<glm/glm.hpp>
#include"../dataStructures.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Core
////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Core
{
	struct BufferObject
	{
		BufferObject() = default;
		~BufferObject() = default;
		BufferObject(const BufferObject&) = delete;
		BufferObject& operator=(const BufferObject&) = delete;
		uint32_t VBO, EBO, VAO;
		uint32_t count;
	};
	struct Quad
	{
		glm::vec2 pos;
		glm::vec2 scale;
		float rot;
		Xenon::Color col;
		Xenon::Texture* tex;
	};
	struct Line
	{
		glm::vec2 pos0;
		glm::vec2 pos1;
		Xenon::Color color;
	};
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Xenon
////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Xenon
{
	class Texture
	{
		//TODO
	};

	class OrthographicCamera2D
	{
		//TODO
	};

	class PerspectiveCamera2D
	{
		//TODO
	};

	class StaticLayer2D
	{
	public:
		StaticLayer2D();
		~StaticLayer2D();
		StaticLayer2D(const StaticLayer2D&) = delete;
		StaticLayer2D& operator=(const StaticLayer2D&) = delete;
		void addQuad(glm::vec2 position, glm::vec2 scale = {1, 1}, float rotation = 0.f, Xenon::Color color = {255, 255, 255, 255}, Xenon::Texture* texture = nullptr);
		void addLine(glm::vec2 pos0, glm::vec2 pos1, Xenon::Color color);
		void compute();
	private:
		std::vector<Core::BufferObject> m_buffers;
		std::map<Xenon::Texture*, uint32_t> m_testureList;
		std::vector<Core::Quad> m_quadList;
		std::vector<Core::Line> m_lineList;
		uint32_t m_lineVBO;
		int m_textureSlotAmmount;
		int m_textureCounter = 0;

	};

	class DynamicLayer2D
	{
	public:
		DynamicLayer2D();
		~DynamicLayer2D();
		DynamicLayer2D(const DynamicLayer2D&) = delete;
		DynamicLayer2D& operator=(const DynamicLayer2D&) = delete;;

	private:

	};

	class Renderer2D
	{
	public:
		static void init();
		void draw(const StaticLayer2D& layer);
		void draw(const DynamicLayer2D& layer);

	private:
		Renderer2D();

	};
}

// Vertex params

//position		vec2
//color			int
//tectureUV		vec2
//texture slot	int