#ifndef COMPONENTS_POOL_HPP
#define COMPONENTS_POOL_HPP

#include "ECS/ComponentID.hpp"
#include "LinkedArray.hpp"
#include "ECS/Entity.hpp"
#include <cstddef>
#include <unordered_map>
#include <utility>

namespace Core {

class ComponentManager;

template<typename T>
class CompnentPool {
public:
	CompnentPool() = delete;
	~CompnentPool() = default;
	CompnentPool(const CompnentPool&) = delete;
	CompnentPool(CompnentPool&&) noexcept = delete;
	CompnentPool& operator=(const CompnentPool&) = delete;
	CompnentPool& operator=(CompnentPool&&) noexcept = delete;
private:
	CompnentPool(ComponentManager* cmgr, ComponentID cID) : m_ID(cID), m_componentMgr(cmgr) {}

	[[nodiscard]] T& getComponent(Entity ent) { return m_data[m_indexLookupTable.at(ent)]; }
	void addComponent(Entity ent, const T& data = {}) {
		#ifdef XENON_DEBUG
		if(m_indexLookupTable.contains(ent)) {
			XN_LOG_WAR("Entity {0} already contains component {q}", ent, typeid(T).name());
			return;
		}
		#endif
		m_indexLookupTable[ent] = m_data.size();
		m_data.pushBack(data);
		m_entityList.pushBack(ent);
	}
	void setData(Entity ent, const T& data) { m_data[m_indexLookupTable.at(ent)] = data; }
	[[nodiscard]] std::pair<Entity, size_t> removeComponent(Entity ent) {
		auto inx = m_indexLookupTable.at(ent);
		if(inx != m_data.size() - 1) {
			m_data[inx] = std::move(m_data.back());
			m_entityList[inx] = m_entityList.back();
		}
		Entity movedEnt = m_entityList.back();
		m_indexLookupTable.at(movedEnt) = inx;
		m_data.popBack();
		m_entityList.popBack();
		// TODO: handle last item removal
		return { movedEnt, inx };
	}
	[[nodiscard]] bool hasComponent(Entity ent) { return m_indexLookupTable.contains(ent); }
	void purge() {
		m_data.clear();
		m_entityList.clear();
		m_indexLookupTable.clear();
	}

	ComponentID m_ID;
	ComponentManager* m_componentMgr;
	std::unordered_map<Entity, size_t> m_indexLookupTable;
	LinekdArray<T> m_data;
	LinekdArray<Entity> m_entityList;
	friend class ComponentManager;
	friend class System;
};
}
#endif // !COMPONENTS_POOL_HPP
