#ifndef _XENON_SRC_ECS_COMPONENTS_POOL_
#define _XENON_SRC_ECS_COMPONENTS_POOL_

#include "ECS/ComponentID.hpp"
#include "LinkedArray.hpp"
#include "ECS/Entity.hpp"
#include "devTools/logger_core.hpp" //NOLINT
#include <strings.h>
#include <utility>
#include <unordered_map>

namespace Core {

class ComponentManager;

template<typename T>
class ComponentPool {
public:
	ComponentPool() = delete;
	~ComponentPool() = default;
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool(ComponentPool&&) noexcept = delete;
	ComponentPool& operator=(const ComponentPool&) = delete;
	ComponentPool& operator=(ComponentPool&&) noexcept = delete;
private:
	using LAInxType = LinkedArray<T>::index;
	ComponentPool(ComponentManager* cmgr, ComponentID cID) : m_ID(cID), m_componentMgr(cmgr) {}

	[[nodiscard]] T& getComponent(Entity ent) { return m_data[m_indexLookupTable.at(ent)]; }
	[[nodiscard]] T& getComponent(uint32_t inxMajor, uint32_t inxMinor) { return m_data[LAInxType(inxMajor, inxMinor)]; }
	[[nodiscard]] bool hasComponent(Entity ent) { return m_indexLookupTable.contains(ent); }
	void setData(Entity ent, const T& data) { m_data[m_indexLookupTable.at(ent)] = data; }

	void addComponent(Entity ent, const T& data = {}) {
		#ifdef XENON_DEBUG
		if(m_indexLookupTable.contains(ent)) {
			XN_LOG_WAR("Entity {0} already contains component {q}", ent, typeid(T).name());
			return;
		}
		#endif
		m_indexLookupTable.emplace(ent, m_data.getMMIndexLast());
		m_data.pushBack(data);
		m_entityList.pushBack(ent);
	}
	[[nodiscard]] std::pair<Entity, LAInxType> removeComponent(Entity ent) {
		#ifdef XENON_DEBUG
			if(!m_indexLookupTable.contains(ent)) XN_LOG_WAR("Tried to remove an component from an entity that doesnt have it");
		#endif
		auto inx = m_indexLookupTable.at(ent);
		if(!m_data.isLast(inx)) {
			m_data[inx] = std::move(m_data.back());
			m_entityList[m_data.getIndex(inx)];
		}
		Entity movedEnt = m_entityList.back();
		m_indexLookupTable.at(movedEnt) = inx;
		m_data.popBack();
		m_entityList.popBack();
		return {movedEnt, inx};
	}
	void purge() {
		m_data.clear();
		m_entityList.clear();
		m_indexLookupTable.clear();
	}

	ComponentID m_ID;
	ComponentManager* m_componentMgr;
	std::unordered_map<Entity, LAInxType> m_indexLookupTable;
	LinkedArray<T> m_data;
	LinkedArray<Entity> m_entityList;
	friend class ComponentManager;
};
}
#endif // !COMPONENTS_POOL_HPP
