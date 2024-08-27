#ifndef _XENON_ECS_COMPONENTPOOL_
#define _XENON_ECS_COMPONENTPOOL_

#include <functional>
#include <memory_resource>
#include <unordered_map>

#include "entity.hpp"
#include "chunkedArray.hpp"

namespace Core {
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
	[[nodiscard]] T* getComponentPtr(Entity ent) { return m_ptrLookupTable.at(ent); }
	template<typename ...Args>
	T* emplaceComponent(Entity ent, Args ...args) {
		auto* ptr = m_data.emplace_back(std::forward<Args>(args)...);
		m_ptrLookupTable[ent] = ptr;
		m_entLookupTable[ptr] = ent;
		return ptr;
	}
	void addComponent(Entity ent, const T& data) { m_entitiesToAdd.emplace_back(ent, data); }
	void addComponent(Entity ent,T&& data) { m_entitiesToAdd.emplace_back(ent, std::move(data)); }
	void removeComponent(Entity ent) { m_entitiesToRemove.push_back(ent); }
	void purge() {
		m_data.clear();
		m_ptrLookupTable.clear();
		m_entLookupTable.clear();
		m_entitiesToRemove.clear();
	}
private:
	explicit ComponentPool(std::pmr::memory_resource* memRes = std::pmr::get_default_resource())
	: m_data(memRes) {}

	ChunkedArray<T> m_data;
	//PERF: get rid of std::unordered_map, and maybe std::vector, for something more performent
	std::unordered_map<Entity, T*> m_ptrLookupTable;
	std::unordered_map<T*, Entity> m_entLookupTable;
	std::vector<Entity> m_entitiesToRemove;
	std::vector<std::pair<Entity, T>> m_entitiesToAdd;
	std::vector<std::pair<Entity, T*>> m_movedEnts;
	
	void p_resolveRemovals() {
		m_movedEnts.reserve(m_entitiesToRemove.size());
		for(auto ent : m_entitiesToRemove) {
			T* ptr = m_ptrLookupTable.at(ent);
			T* ptrLast = &m_data.back();
			if(ptrLast != ptr) {
				Entity entLast = m_entLookupTable.at(ptrLast);
				*ptr = std::move(*ptrLast);
				m_ptrLookupTable[entLast] = ptr;
				m_entLookupTable[ptr] = entLast;
				m_movedEnts.push_back({entLast, ptr});
			}
			m_ptrLookupTable.erase(ent);
			m_entLookupTable.erase(ptrLast);
			m_data.pop_back();
		}
		m_entitiesToRemove.clear();
	}
	template<typename DT>
	void p_resolveDependencies(const std::vector<std::pair<Entity, DT*>>& movedEnts, std::function<void(T&, DT*)> resolveFunc ) {
		for(auto pair : movedEnts) {
			resolveFunc(getComponent(pair.first), pair.second);
		}
	}
	void p_resolveAdditions() {
		m_movedEnts.reserve(m_entitiesToAdd.size());
		for(auto pair : m_entitiesToAdd) {
			auto* ptr = emplaceComponent(pair.first, pair.second);
			m_movedEnts.emplace_back(pair.first, ptr);
		}
		m_entitiesToAdd.clear();
	}
	friend struct ComponentCluster;
};	
}
#endif // !_XENON_ECS_COMPONENTPOOL_
