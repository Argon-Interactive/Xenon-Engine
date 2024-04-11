#include"engDataStruct.hpp"
#include"texture_core.hpp"
#include<glm/glm.hpp>

namespace Core {

	struct Quad {};

	class Renderer2D {
	public:
		~Renderer2D() = default;
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator = (const Renderer2D&) = delete;
		static Renderer2D& getInstance();
		void addStaticQuad(const Quad& quad);
		void addStaticQuad(Quad* quadList, size_t listSize);
		Xenon::XenID addDynamicLayer();
		Xenon::XenID setDynamicLayerData(Xenon::XenID LayerID, Quad* quadList, size_t listSize, Core::Texture* TextureList, int32_t textureAmmount);
		void removeDynamicLayer(Xenon::XenID LayerID);
		void render();


		
	private:
		Renderer2D();
		static Renderer2D s_instance;

	};

	
}