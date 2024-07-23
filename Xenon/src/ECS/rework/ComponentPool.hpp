#ifndef _XENON_SRC_ECS_COMPONENTPOOL_
#define _XENON_SRC_ECS_COMPONENTPOOL_
#include <memory_resource>
#include <vector>
#include <cmath>

#include "ChunkedArray.hpp"

namespace Core {

using Entity = uint64_t;

template<typename T>
class ComponentPool {
public:
	ComponentPool(std::pmr::memory_resource* memoryResource) : m_data(memoryResource) {

	}
	~ComponentPool() {

	}
	ComponentPool(ComponentPool&&) = delete;
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool &operator=(ComponentPool &&) = delete;
	ComponentPool &operator=(const ComponentPool &) = delete;

	///////////////////////////////////////
	/// Entity interface
	///////////////////////////////////////

private:
	ChunkedArray<T> m_data;
};	
}
#endif
