#ifndef SCENE_HPP
#define SCENE_HPP

namespace Core { class SceneManager; }

namespace Xenon { //chyba

class Transition {
public:
	Transition();
	~Transition();
	Transition(Transition&&) = delete;
	Transition(const Transition&) = delete;
	Transition& operator=(Transition&) = delete;
	Transition& operator=(Transition&&) = delete;

private:

};

class Scene {
public:
	Scene();
	virtual ~Scene();

	//Scene(Scene&&) = default;
	//Scene(const Scene&) = delete;
	//Scene& operator=(Scene&&) = default;
	//Scene& operator=(const Scene&) = delete;

protected:
	virtual void render() = 0;
	virtual void update(double deltaT) = 0;

};

}

#endif
