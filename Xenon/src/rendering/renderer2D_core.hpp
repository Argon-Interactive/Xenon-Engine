#ifndef _XENON_RENDERING_RENDERER2D_
#define _XENON_RENDERING_RENDERER2D_

#include"temporary/engDataStruct.hpp"
#include"texture_core.hpp"
#include"shader.hpp"
#include<deque>
#include<memory>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

namespace Core {
	
	struct Vertice { //temp
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec4 color; //TODO mybe make it not 4 floats idk
		float texId;
	};

	struct Quad {
		Vertice v0, v1, v2, v3;
		Quad(Vertice vert0, Vertice vert1, Vertice vert2, Vertice vert3) :v0(vert0), v1(vert1), v2(vert2), v3(vert3) {}
	};

	class Camera {
	public:
		Camera() = default;
		virtual ~Camera() = default;

		virtual glm::mat4 getMatrix() const;

	protected:
	};

	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera();
		OrthographicCamera(float x, float y, float z, float width, float height);
		~OrthographicCamera() override = default;

		glm::mat4 getMatrix() const override;

		glm::vec3 pos;
		glm::vec2 size;
	private:
		glm::vec3 m_lookAt; //TODO in 2D its useless but i think it might be needed in 3D (if itll ever come to that)
	};

	class PerspictiveCamera : public Camera {
	public:
		PerspictiveCamera();
		~PerspictiveCamera() override;

		glm::mat4 getMatrix() const override;
	private:
		//TODO
		glm::vec3 m_pos;
		glm::vec3 m_lookAt;
		double m_fov;
	};


	class Renderer2D {
	public:
		Renderer2D() = default;
		~Renderer2D();
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator = (const Renderer2D&) = delete;

		Xenon::ID createStaticLayer(Core::Quad quadList[], size_t quadListSize, std::shared_ptr<Core::Texture2D> textureList[], size_t textureListSize);
		Xenon::ID createDynamicLayer();
		void deleteStaticLayer(Xenon::ID &layerID);
		void deleteDynamicLayer(Xenon::ID &layerID);

		void render(const Core::Camera& camera, Core::Shader& shader, Xenon::ID layerID);
		
	private:
		struct m_layerData { uint32_t VAO, VBO, EBO, size; };
		std::deque<Xenon::ID> m_freeIDList;
		std::vector<m_layerData> m_layers;
		std::vector<std::shared_ptr<Core::Texture2D>> m_textures; //at some point maybe put this inside m_layerData
	};

	
}

#endif // !_XENON_RENDERING_RENDERER2D_
