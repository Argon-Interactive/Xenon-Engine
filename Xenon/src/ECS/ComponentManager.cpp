#include "ComponentManager.hpp"
#include "ComponentsPool.hpp"
#include "ECS/ComponentID.hpp"

namespace Core {

void ComponentManager::resolveRemoval(ComponentID ID, uint64_t oldEntity, size_t newInx) {
	switch (ID) {
		
	}
}

void ComponentManager::addComponent(ComponentID ID, Entity ent) {
		switch (ID) {
			case ComponentID::Test:
			m_test.addComponent(ent); break;
		}
}

void ComponentManager::purgeComponent(ComponentID ID) {
	switch(ID) {
		case ComponentID::Test:
		m_test.purge(); break;
	}
}

void ComponentManager::purgeAll() {
	m_test.purge();
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
