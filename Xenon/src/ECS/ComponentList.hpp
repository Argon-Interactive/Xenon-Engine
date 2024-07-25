#ifndef _XENON_SRC_ECS_COMPONENTLIST_
#define _XENON_SRC_ECS_COMPONENTLIST_

#include "ComponentPool.hpp"

namespace Core {
template<typename T>
class ComponentList {
	class ComponentListNode;
public:
	
private:
};

template<typename T>
class ComponentList<T>::ComponentList {
	ComponentPool<T>* m_data;
	ComponentPool<T>* m_next;
	ComponentPool<T>* m_prev;
};

}
#endif // !_XENON_SRC_ECS_COMPONENTLIST_
