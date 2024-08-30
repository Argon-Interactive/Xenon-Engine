#ifndef _XENON_SCENES_SCENE_
#define _XENON_SCENES_SCENE_

#include "memory/chunkedPMR.hpp"
#include "ECS/componentCluster.hpp"
#include "ECS/entity.hpp"

#ifdef __DEBUG__
#include "memory/debugMemoryResource.hpp"
#endif // !__DEBUG__

#include <memory_resource>
#include <string>
#include <cstdint>
#include <atomic>

namespace Core {

class Scene {
public:
	Scene();
	explicit Scene(uint64_t buildIndex);
	~Scene();
	Scene(const Scene &) = delete;
	Scene(Scene &&) = delete;
	Scene &operator=(const Scene &) = delete;
	Scene &operator=(Scene &&) = delete;

	Entity createEntity();
	void deleteEntity(Entity entity);

	template<class Component>
	[[nodiscard]] Component& getComponent(Entity entity) {
		return m_components.get<Component>().getComponent(entity);
	}

	template<class Component>
	void addComponent(Entity entity) {
		m_components.get<Component>().addComponent(entity, {});
	}

	template<class Component>
	void removeComponent(Entity entity) {
		m_components.get<Component>().removeComponent(entity);
	}

	[[nodiscard]] uint64_t getBuildIndex() const;
	[[nodiscard]] bool isRuntimeCreated() const;

	static void setEntityStartID(Entity id);

private:
	class SceneMemory {
	public:
		explicit SceneMemory(const std::string& name = "Scene");
		[[nodiscard]] std::pmr::memory_resource* get();
	private:
#ifdef __DEBUG__
		DebugMemoryResource mem2;
		ChunkedPMR mem1;
		DebugMemoryResource mem0;
#else
		ChunkedPMR mem0;
#endif // !__DEBUG__
	};

	SceneMemory m_sceneMemory;
	ComponentCluster m_components;

	bool m_runtimeCreated;
	uint64_t m_buildIndex;

	inline static std::atomic<Entity> s_entityID = 0;
	inline static Entity s_entityStartID = std::numeric_limits<uint64_t>::max();

	inline std::string p_debugIndex() const {
		return m_runtimeCreated ? "" : std::to_string(m_buildIndex);
	}

	friend class SceneManager;
};

}

#endif // !_XENON_SCENES_SCENE_
