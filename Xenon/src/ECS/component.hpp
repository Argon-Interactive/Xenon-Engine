#ifndef _XENON_ECS_COMPONENT_
#define _XENON_ECS_COMPONENT_

namespace Core {

template<typename T>
class Reference;

class Component {
public:
	Component();
	virtual ~Component();
	Component(const Component &);
	Component(Component &&) noexcept;
	Component &operator=(const Component &);
	Component &operator=(Component &&) noexcept;

private:
	Reference<Component>* m_listHead = nullptr;

	void p_atDelete();
	void p_atMove();

	template<typename T>
	friend class Reference;
};

}

#endif // !_XENON_ECS_COMPONENT_
