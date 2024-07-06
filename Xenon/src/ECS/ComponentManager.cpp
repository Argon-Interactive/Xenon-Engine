#include "ComponentsPool.hpp"
#include "ECS/ComponentID.hpp"
#include "ComponentManager.hpp"

namespace Core {


void ComponentManager::resolveRemoval(ComponentID ID, uint64_t oldEntity, size_t newInx) {
	switch (ID) {
		
	}
}

void ComponentManager::addComponent(ComponentID ID, Entity ent, int data) {
	switch (ID) {
		case ComponentID::Test:
		m_test.addComponent(ent, data); break;
	}
}

void ComponentManager::setComponentData(ComponentID ID, Entity ent, int data) {
	switch (ID) {
		case ComponentID::Test:
		m_test.setData(ent, data); break;

	}

}

void ComponentManager::removeComponent(ComponentID ID, Entity ent) {
	std::pair<Entity, size_t> ret;
	switch(ID) {
		case ComponentID::Test:
		ret = m_test.removeComponent(ent); break;
	}
	resolveRemoval(ID, ret.first, ret.second);
}
}
