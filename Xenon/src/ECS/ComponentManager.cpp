#include "ComponentManager.hpp"
#include "ComponentPool.hpp"
#include "ECS/ComponentID.hpp"

namespace Core {

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

}
