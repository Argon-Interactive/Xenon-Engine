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
public:
	explicit ComponentPool(std::pmr::memory_resource* memoryResource = std::pmr::get_default_resource())
	: m_data(memoryResource), m_entitiesToDelete(memoryResource) {}
	~ComponentPool() = default;

	ComponentPool(ComponentPool&&) = delete;
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool &operator=(ComponentPool &&) = delete;
	ComponentPool &operator=(const ComponentPool &) = delete;

	[[nodiscard]] ChunkedArray<T>* data() { return &m_data; }
	[[nodiscard]] size_t size() { return m_data.size(); }

	///////////////////////////////////////
	/// Entity interface
	///////////////////////////////////////


private:
	ChunkedArray<T> m_data;
	//TODO: get rid of std::unordered_map for something more performent
	std::unordered_map<size_t, Entity> m_entLookupTable;
	std::pmr::vector<Entity> m_entitiesToDelete;

	void pm_deleteComponentData(Entity ent) {
		/*
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
		*/
	}
};	
}
#endif
