#include"engDataStruct.hpp"
#include"texture_core.hpp"
#include<queue>
#include<memory>

namespace Core {

	struct Quad {};

	class Renderer2D {
	public:
		~Renderer2D();
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator = (const Renderer2D&) = delete;
		static Renderer2D& getInstance();

		Xenon::ID createStaticLayer(Core::Quad quadList[], size_t quadListSize, Core::Texture2D textureList[], size_t textureListSize);
		Xenon::ID createDynamicLayer();
		void deleteStaticLayer(Xenon::ID ID);
		void deleteDynamicLayer(Xenon::ID layerID);

		void render();


		
	private:
		struct m_layerData {
			uint32_t VAO, VBO, EBO;
		};
		Renderer2D();
		static Renderer2D s_instance;
		std::deque<Xenon::ID> m_freeIDList;
		std::vector<m_layerData> m_layers;
		std::vector<std::shared_ptr<Core::Texture2D>> m_textures;
	};

	
}