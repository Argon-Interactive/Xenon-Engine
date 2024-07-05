#ifndef _XENON_SCENES_SCENE_
#define _XENON_SCENES_SCENE_

#include <vector>
#include <cstdint>

namespace Core {

// class SceneManager;

class Scene {
public:
	Scene();
	virtual ~Scene();
	Scene(Scene&&) = delete;
	Scene(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;

	void unload();

	uint64_t createEntity();
	void deleteEntity(uint64_t uuid);

	int64_t getBuildIndex();

private:
	std::vector<uint64_t> m_entities;   // men titties
	
	bool m_runtimeCreated = true;
	int64_t m_buildIndex;

};

}

#endif // _XENON_SCENES_SCENE_
