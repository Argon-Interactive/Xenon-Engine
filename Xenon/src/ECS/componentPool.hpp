#ifndef _XENON_SRC_ECS_COMPONENTPOOL_
#define _XENON_SRC_ECS_COMPONENTPOOL_

#include <memory_resource>
#include <unordered_map>

#include "entity.hpp"
#include "chunkedArray.hpp"

namespace Core {
class ComponentCluster;

template<typename T>
class ComponentPool {
public:
	~ComponentPool() = default;

	ComponentPool(ComponentPool&&) = delete;
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool &operator=(ComponentPool &&) = delete;
	ComponentPool &operator=(const ComponentPool &) = delete;

	[[nodiscard]] size_t size() { return m_data.size(); }

	///////////////////////////////////////
	/// Entity interface
	///////////////////////////////////////

	[[nodiscard]] T& getComponent(Entity ent) { return *m_ptrLookupTable.at(ent); }
	template<typename ...Args>
	void emplaceComponent(Entity ent, Args ...args) {
		m_data.emplace_back(std::forward<Args>(args)...);
		auto* ptr = &m_data.back();
		m_ptrLookupTable[ent] = ptr;
		m_entLookupTable[ptr] = ent;
	}
	void addComponent(Entity ent) { emplaceComponent(ent); }
	void addComponent(Entity ent, const T& data) { emplaceComponent(ent, data); }
	void addComponent(Entity ent,T&& data) { emplaceComponent(ent, std::move(data)); }
	void removeComponent(Entity ent) { m_entitiesToRemove.push_back(ent); }
	void purge() {
		m_data.clear();
		m_ptrLookupTable.clear();
		m_entLookupTable.clear();
		m_entitiesToRemove.clear();
	}
private:
	explicit ComponentPool(std::pmr::memory_resource* memRes = std::pmr::get_default_resource())
	: m_data(memRes), m_ptrLookupTable(memRes), m_entLookupTable(memRes), m_entitiesToRemove(memRes), m_movedEnts(memRes) {}

	ChunkedArray<T> m_data;
	//PERF: get rid of std::unordered_map for something more performent
	std::pmr::unordered_map<Entity, T*> m_ptrLookupTable;
	std::pmr::unordered_map<T*, Entity> m_entLookupTable;
	std::pmr::vector<Entity> m_entitiesToRemove;
	std::pmr::vector<std::pair<Entity, T*>> m_movedEnts;
	
	void p_resolveRemovals() {
		T* ptr = nullptr;
		for(auto ent : m_entitiesToRemove) {
			if(&m_data.back() == m_ptrLookupTable.at(ent)) {
				m_data.pop_back();
				ptr = &m_data.back();
				m_ptrLookupTable.erase(ent);
				m_entLookupTable.erase(ptr);
				continue;
			}
			ptr = m_ptrLookupTable.at(ent);
			T* ptrLast = &m_data.back();
			Entity entLast = m_entLookupTable.at(ptrLast);
			*ptr = std::move(*ptrLast);
			m_ptrLookupTable[entLast] = ptr;
			m_entLookupTable[ptr] = entLast;
			m_ptrLookupTable.erase(ent);
			m_entLookupTable.erase(ptrLast);
			m_movedEnts.push_back({entLast, ptr});
		}
		m_entitiesToRemove.clear();
	}
	friend class ComponentCluster;
};	
}
#endif
