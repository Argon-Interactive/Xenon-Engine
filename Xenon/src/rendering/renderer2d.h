#pragma once
#include<vector>
#include<map>
#include<string>
#include"../dataStructures.h"
#include"../core.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Xenon
////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Xenon
{
	class XAPI Texture
	{
	public:
		Texture(std::string filepath);
		~Texture();
		void bind(uint32_t slot = 0);
		static void unbind();
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(Texture&&) = delete;
	private:
		uint32_t m_ID;
	};

	class OrthographicCamera2D
	{
		//TODO
	};

	class PerspectiveCamera2D
	{
		//TODO
	};

	class XAPI StaticLayer2D
	{
	public:
		StaticLayer2D();
		~StaticLayer2D();
		StaticLayer2D(const StaticLayer2D&) = delete;
		StaticLayer2D& operator=(const StaticLayer2D&) = delete;
		void addQuad(Xenon::Vec2 position, Xenon::Vec2 scale = { 1, 1 }, float rotation = 0.f, Xenon::Color color = { 255, 255, 255, 255 }, Xenon::Texture* texture = nullptr);
		void compute();
	private:
		struct Vertice2
		{
			float x;
			float y;
			float u;
			float v;
			int color;
			int textureSlot;
		};
		uint32_t m_VBO = 0;
		std::map<Xenon::Texture*, uint32_t> m_textureMap;
		std::vector<std::vector<Vertice2>> m_vertList; //TODO Fix this memory segmentation
		std::vector<std::vector<Xenon::Texture*>> m_textureList;
		std::vector<int> m_renderAmmouts;
		int m_textureCounter = 0;
		int m_textureLayer = 0;
		int m_quadAmmout = 0;
		bool m_isComputed = false;
		friend class Renderer2D;
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

	class XAPI Renderer2D
	{
	public:
		static void init();
		void draw(const Xenon::OrthographicCamera2D& camera, const StaticLayer2D& layer);
		void draw(const DynamicLayer2D& layer);
		static int textureSlotsAmmout;
	private:
		Renderer2D();
		static uint32_t* genEBO(int quadAmmount);
		static uint32_t m_EBO;
		static uint32_t m_EBOSize;
	};
}
