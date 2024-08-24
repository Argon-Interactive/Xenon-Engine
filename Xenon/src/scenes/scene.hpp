#ifndef _XENON_SCENES_SCENE_
#define _XENON_SCENES_SCENE_

#include "ECS/componentCluster.hpp"
#include "ECS/entity.hpp"

#ifdef __DEBUG__
#include "memory/debugMemoryResource.hpp"
#endif // !__DEBUG__

#include <memory_resource>
#include <string>
#include <cstdint>

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

	void unload();

	Entity createEntity();
	void deleteEntity(Entity uuid);

	[[nodiscard]] uint64_t getBuildIndex() const;
	[[nodiscard]] bool isRuntimeCreated() const;

private:
	class SceneMemory {
	public:
		explicit SceneMemory(const std::string& name = "Scene");
		[[nodiscard]] std::pmr::memory_resource* get();
	private:
#ifdef __DEBUG__
		DebugMemoryResource mem2;
		std::pmr::unsynchronized_pool_resource mem1;
		DebugMemoryResource mem0;
#else
		std::pmr::unsynchronized_pool_resource mem0;
#endif // !__DEBUG__
	};

	SceneMemory m_sceneMemory;
	ComponentCluster m_components;

	inline static Entity s_entityID = std::numeric_limits<uint64_t>::max();
	
	bool m_runtimeCreated;
	uint64_t m_buildIndex;

	inline std::string p_debugIndex() const {
		return m_runtimeCreated ? "" : std::to_string(m_buildIndex);
	}

	friend class SceneManager;
};

}

#endif // !_XENON_SCENES_SCENE_
