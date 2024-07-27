#ifndef _XENON_SCENES_SCENE_
#define _XENON_SCENES_SCENE_

#include <vector>
#include <cstdint>
#include "ECS/entity.hpp"

namespace Core {

// class SceneManager;

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

	[[nodiscard]] bool runtimeCreated() const;

private:
	std::vector<uint64_t> m_entities;   // men titties
	
	bool m_runtimeCreated;
	uint64_t m_buildIndex;

	friend class SceneManager;
};

}

#endif // _XENON_SCENES_SCENE_
