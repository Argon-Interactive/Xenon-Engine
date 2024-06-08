#ifndef SCENE_HPP
#define SCENE_HPP

namespace Core { class SceneManager; }

namespace Xenon { //chyba

class Scene {
public:
	Scene();
	virtual ~Scene();

	Scene(Scene&&) = delete;
	Scene(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;

protected:
	virtual void render() = 0;
	virtual void update(double deltaT) = 0;

	friend class Core::SceneManager;
};

}

#endif
