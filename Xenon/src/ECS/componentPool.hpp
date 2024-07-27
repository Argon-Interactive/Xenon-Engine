#ifndef _XENON_SRC_ECS_COMPONENTPOOL_
#define _XENON_SRC_ECS_COMPONENTPOOL_

#include <memory_resource>
#include <unordered_map>
#include <algorithm>

#include "entity.hpp"
#include "chunkedArray.hpp"

namespace Core {

template<typename T>
class ComponentPool {
	using ChunkedArrayMMindexType = ChunkedArray<T>::MMindex;
public:
	explicit ComponentPool(std::pmr::memory_resource* memoryResource = std::pmr::get_default_resource())
	: m_data{memoryResource}, m_entitiesToDelete(memoryResource) {}
	~ComponentPool() = default;

	ComponentPool(ComponentPool&&) = delete;
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool &operator=(ComponentPool &&) = delete;
	ComponentPool &operator=(const ComponentPool &) = delete;

	[[nodiscard]] ChunkedArray<T>& data() { return m_data; }

	///////////////////////////////////////
	/// Entity interface
	///////////////////////////////////////

	[[nodiscard]] T& getComponent(Entity ent) { return m_data[m_inxLookupTable.at(ent)]; }
	[[nodiscard]] T& getComponent(ChunkedArrayMMindexType inx) { return m_data[inx]; }

	[[nodiscard]] bool hasComponent(Entity ent) { return m_inxLookupTable.contains(ent); }
	[[nodiscard]] bool isIndexValid(ChunkedArrayMMindexType inx) { return m_entLookupTable.contains(m_data.getIndex(inx)); }

	void addEntity(Entity ent) { emplaceEntity(ent); }
	void addEntity(Entity ent, const T& data) { emplaceEntity(ent, data); }
	void addEntity(Entity ent, T&& data) { emplaceEntity(ent, std::move(data)); }

	template<typename ...Args>
	void emplaceEntity(Entity ent, Args&&... args) {
		m_data.emplace_back(std::forward<Args>(args)...);
		auto inx = m_data.getMMindexBack();
		m_inxLookupTable[ent] = inx;
		m_entLookupTable[m_data.getIndex(inx)] = ent;
	}
	void removeEntity(Entity ent) { m_entitiesToDelete.push_back(ent); }
	void setComponentData(Entity ent, const T& data) { m_data.at(m_inxLookupTable(ent)) = data; }
	void setComponentData(Entity ent, T&& data) { m_data.at(m_inxLookupTable(ent)) = std::move(data); }
	void purge() {
		m_data.clear();
		m_inxLookupTable.clear();
		m_entLookupTable.clear();
		m_entitiesToDelete.clear();
	}
	void resolveRemovals() {
		std::sort(m_entitiesToDelete.begin(), m_entitiesToDelete.end(), std::greater<ChunkedArrayMMindexType>());
		for(auto ent : m_entitiesToDelete) pm_deleteComponentData(ent);
	}

private:
	ChunkedArray<T> m_data;
	//TODO: get rid of std::unordered_map for something more performent
	std::unordered_map<Entity, ChunkedArrayMMindexType> m_inxLookupTable;
	std::unordered_map<size_t, Entity> m_entLookupTable;
	std::pmr::vector<Entity> m_entitiesToDelete;

	void pm_deleteComponentData(Entity ent) {
		auto inx = m_inxLookupTable.at(ent);
		auto inxLast = m_data.getMMindexBack();
		if(m_inxLookupTable.at(ent) == inxLast) {
			m_data.pop_back();
			m_entLookupTable.erase(m_data.getIndex(inx));
			m_inxLookupTable.erase(ent);
			return;
		}
		auto entLast = m_entLookupTable.at(m_data.getIndex(inxLast));
		m_data[inx] = std::move(m_data[inxLast]);
		m_inxLookupTable[m_entLookupTable.at(inxLast)] = inx;
		m_entLookupTable[m_data.getIndex(inx)] = entLast;
		m_inxLookupTable.erase(ent);
		m_entLookupTable.erase(m_data.getIndex(inxLast));
		m_data.pop_back();
		//TODO: resolve components index dependencies when a component is moved
		//I was thinking about adding compennt to a movedComponent list and when a component is read through index a check weather this index was moved 
		//is made and if so the saved value is updated
	}
};	
}
#endif
