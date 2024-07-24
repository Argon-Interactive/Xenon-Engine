#ifndef _XENON_SRC_ECS_COMPONENTPOOL_
#define _XENON_SRC_ECS_COMPONENTPOOL_

#include <memory_resource>
#include <unordered_map>

#include "Entity.hpp"
#include "ChunkedArray.hpp"

namespace Core {

template<typename T>
class ComponentPool {
	using ChunkedArrayMMindexType = ChunkedArray<T>::MMindex;
public:
	explicit ComponentPool(std::pmr::memory_resource* memoryResource)
	: m_data{memoryResource}, m_inxLookupTable{memoryResource}, m_entLookupTable{memoryResource} {}
	~ComponentPool() = default;

	ComponentPool(ComponentPool&&) = delete;
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool &operator=(ComponentPool &&) = delete;
	ComponentPool &operator=(const ComponentPool &) = delete;

	///////////////////////////////////////
	/// Entity interface
	///////////////////////////////////////

	[[nodiscard]] T& getComponent(Entity ent) { return m_data[m_inxLookupTable.at(ent)]; }
	[[nodiscard]] T& getComponent(ChunkedArrayMMindexType inx) { return m_data[inx]; }

	[[nodiscard]] bool hasComponent(Entity ent) { return m_inxLookupTable.contains(ent); }
	[[nodiscard]] bool isIndexValid(ChunkedArrayMMindexType inx) { return m_entLookupTable.contains(inx); }

	void addEntity(Entity ent) { emplaceEntity(ent); }
	void addEntity(Entity ent, const T& data) { emplaceEntity(ent, data); }
	void addEntity(Entity ent, T&& data) { emplaceEntity(ent, std::move(data)); }

	template<typename ...Args>
	void emplaceEntity(Entity ent, Args&&... args) {
		m_data.emplace_back(std::forward<Args>(args)...);
		auto inx = m_data.getMMindexBack();
		m_inxLookupTable[ent] = inx;
		m_entLookupTable[inx] = ent;
	}
	void removeEntity(Entity ent) {
		auto inx = m_inxLookupTable.at(ent);
		auto inxLast = m_data.getMMindexBack();
		if(m_inxLookupTable.at(ent) == inxLast) {
			m_data.pop_back();
			m_entLookupTable.erase(inx);
			m_inxLookupTable.erase(ent);
			return;
		}
		auto entLast = m_entLookupTable.at(inxLast);
		m_data[inx] = std::move(m_data[inxLast]);
		m_inxLookupTable[m_entLookupTable.at(inxLast)] = inx;
		m_entLookupTable[inx] = entLast;
		m_inxLookupTable.erase(ent);
		m_entLookupTable.erase(inxLast);
		m_data.pop_back();
		//TODO resolve components index dependencies when a component is moved
		//I was thinking about adding compennt to a movedComponent list and when a component is read through index a check weather this index was moved 
		//is made and if so the saved value is updated
	}
	void setComponentData(Entity ent, const T& data) { m_data.at(m_inxLookupTable(ent)) = data; }
	void setComponentData(Entity ent, T&& data) { m_data.at(m_inxLookupTable(ent)) = std::move(data); }
	void purge() {
		m_data.clear();
		m_inxLookupTable.clear();
		m_entLookupTable.clear();
	}

private:
	ChunkedArray<T> m_data;
	//TODO get rid of std::unordered_map for something more performent
	std::pmr::unordered_map<Entity, ChunkedArrayMMindexType> m_inxLookupTable;
	std::pmr::unordered_map<ChunkedArrayMMindexType, Entity> m_entLookupTable;
};	
}
#endif
