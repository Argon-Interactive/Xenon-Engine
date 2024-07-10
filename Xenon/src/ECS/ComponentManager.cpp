#include "ComponentManager.hpp"
#include "ComponentPool.hpp"
#include "ECS/ComponentID.hpp"

namespace Core {

void ComponentManager::resolveRemoval(ComponentID ID, uint64_t oldEntity, uint32_t newInxMajor, uint32_t newInxMinor) {
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
	std::pair<Entity, std::pair<uint32_t, uint32_t>> ret;
	switch(ID) {
		case ComponentID::Test:
		ret = m_test.removeComponent(ent); break;
	}
	resolveRemoval(ID, ret.first, ret.second.first, ret.second.second);
}
}
