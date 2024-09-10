#ifndef _XENON_ECS_COMPONENTPOOL_
#define _XENON_ECS_COMPONENTPOOL_

#include <memory_resource>
#include <unordered_map>

#include "entity.hpp"
#include "chunkedArray.hpp"

namespace Core {

template<typename T>
class ComponentPool {
	static_assert(is_proper_component<T>());
public:
	explicit ComponentPool(std::pmr::memory_resource* memRes = std::pmr::get_default_resource())
	: m_data(memRes) {}

	~ComponentPool() = default;

	ComponentPool(ComponentPool&&) = delete;
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool &operator=(ComponentPool &&) = delete;
	ComponentPool &operator=(const ComponentPool &) = delete;

	[[nodiscard]] size_t size() { return m_data.size(); }

	///////////////////////////////////////
	/// Entity interface
	///////////////////////////////////////
	
	[[nodiscard]] bool hasComponent(Entity ent) { return m_ptrLookupTable.contains(ent); }

	[[nodiscard]] T& getComponent(Entity ent) { return *m_ptrLookupTable.at(ent); }
	[[nodiscard]] T* getComponentPtr(Entity ent) { return m_ptrLookupTable.at(ent); }

	template<typename ...Args>
	T* emplaceComponent(Entity ent, Args ...args) {
		if(m_ptrLookupTable.contains(ent)) [[unlikely]] {
			*m_ptrLookupTable[ent](std::forward<Args>(args)...);
		}
		T* ptr = m_data.emplace_back(std::forward<Args>(args)...);
		m_ptrLookupTable[ent] = ptr;
		m_entLookupTable[ptr] = ent;
		return ptr;
	}
	void addComponent(Entity ent) { 
		T* ptr = m_data.emplace_back(); 
		m_ptrLookupTable[ent] = ptr;
		m_entLookupTable[ptr] = ent;
	}
	void addComponent(Entity ent, const T& data) {
		if(m_ptrLookupTable.contains(ent)) [[unlikely]] {
			*m_ptrLookupTable[ent](data);
		}
		T* ptr = m_data.emplace_back(data); 
		m_ptrLookupTable[ent] = ptr;
		m_entLookupTable[ptr] = ent;
	}
	void addComponent(Entity ent,T&& data) { 
		if(m_ptrLookupTable.contains(ent)) [[unlikely]] {
			*m_ptrLookupTable[ent](data);
		}
		T* ptr = m_data.emplace_back(std::move(data)); 
		m_ptrLookupTable[ent] = ptr;
		m_entLookupTable[ptr] = ent;
	}

	void removeComponent(Entity ent) {
		T* ptr = m_ptrLookupTable.at(ent);
		T* ptrLast = &m_data.back();
		if(ptrLast != ptr) {
			Entity entLast = m_entLookupTable.at(ptrLast);
			*ptr = std::move(*ptrLast);
			m_ptrLookupTable[entLast] = ptr;
			m_entLookupTable[ptr] = entLast;
		}
		m_ptrLookupTable.erase(ent);
		m_entLookupTable.erase(ptrLast);
		ptrLast->destroy();
		m_data.pop_back();
	}

	void purge() {
		m_data.clear();
		m_ptrLookupTable.clear();
		m_entLookupTable.clear();
	}
private:
	ChunkedArray<T> m_data;
	//PERF: get rid of std::unordered_map, for something more performent
	std::unordered_map<Entity, T*> m_ptrLookupTable;
	std::unordered_map<T*, Entity> m_entLookupTable;

	friend class ComponentCluster;
};	

}

#endif // !_XENON_ECS_COMPONENTPOOL_
