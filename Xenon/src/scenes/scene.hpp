#ifndef _XENON_SCENES_SCENE_
#define _XENON_SCENES_SCENE_

#include "ECS/componentCluster.hpp"
#include "ECS/entity.hpp"

#include <memory_resource>
#include <vector>
#include <cstdint>
#include <memory>

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

	void setBuildIndex(uint64_t index);
	[[nodiscard]] uint64_t getBuildIndex() const;

	[[nodiscard]] bool isRuntimeCreated() const;

private:
	class SceneMemory {
	public:
		SceneMemory();
		void addMemoryResource(std::unique_ptr<std::pmr::memory_resource>& res);
		[[nodiscard]] std::pmr::memory_resource* get() const noexcept;
		std::pmr::memory_resource* m_mres;
		std::vector<std::unique_ptr<std::pmr::memory_resource>> m_resourceChain;
	};

	class StaticSceneMemory : public SceneMemory {
	public:
		StaticSceneMemory();
	};

	class DynamicSceneMemory : public SceneMemory {
	public:
		DynamicSceneMemory();
	};

	StaticSceneMemory m_staticSceneMemory;
	DynamicSceneMemory m_dynamicSceneMemory;

	std::vector<uint64_t> m_entities;   // men titties

	ComponentCluster m_staticComponentChunk;
	ComponentCluster m_dynamicComponentChunk;
	
	bool m_runtimeCreated;
	uint64_t m_buildIndex;


	friend class SceneManager;
};

}

#endif // _XENON_SCENES_SCENE_
