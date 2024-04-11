#pragma once
#include"engDataStruct.hpp"
#include"texture_core.hpp"
#include<queue>
#include<memory>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

namespace Core {

	struct Quad {};

	class Camera {
	public:
		Camera();
		virtual ~Camera();

		virtual glm::mat4 getMatrix() const;

	protected:
		
	};

	class OrthographicCamera : Camera {
	public:
		OrthographicCamera();
		~OrthographicCamera() override;

		glm::mat4 getMatrix() const override;
	private:

	};

	class PerspictiveCamera : Camera {
	public:
		PerspictiveCamera();
		~PerspictiveCamera() override;

		glm::mat4 getMatrix() const override;
	private:

	};


	class Renderer2D {
	public:
		~Renderer2D();
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator = (const Renderer2D&) = delete;
		static Renderer2D& getInstance();

		Xenon::ID createStaticLayer(Core::Quad quadList[], size_t quadListSize, std::shared_ptr<Core::Texture2D> textureList[], size_t textureListSize);
		Xenon::ID createDynamicLayer();
		void deleteStaticLayer(Xenon::ID layerID);
		void deleteDynamicLayer(Xenon::ID layerID);

		void render();


		
	private:
		static Renderer2D s_instance;
		Renderer2D() = default;
		struct m_layerData { uint32_t VAO, VBO, EBO; };
		std::deque<Xenon::ID> m_freeIDList;
		std::vector<m_layerData> m_layers;
		std::vector<std::shared_ptr<Core::Texture2D>> m_textures;
	};

	
}