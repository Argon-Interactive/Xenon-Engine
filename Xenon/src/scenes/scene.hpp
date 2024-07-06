#ifndef _XENON_SCENES_SCENE_
#define _XENON_SCENES_SCENE_

#include <vector>
#include <cstdint>

namespace Core {

// class SceneManager;

class Scene {
public:
	Scene();
	~Scene();

	void unload();

	uint64_t createEntity();
	void deleteEntity(uint64_t uuid);

	void setBuildIndex(int64_t index);
	[[nodiscard]] int64_t getBuildIndex() const;

private:
	std::vector<uint64_t> m_entities;   // men titties
	
	bool m_runtimeCreated = true;
	int64_t m_buildIndex = -1;

};

}

#endif // _XENON_SCENES_SCENE_
