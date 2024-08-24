#include "debugMemoryResource.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

DebugMemoryResource::DebugMemoryResource(std::string name, std::pmr::memory_resource* upstream)
        : m_upstream(upstream), m_name(std::move(name)) {}

void* DebugMemoryResource::do_allocate(size_t bytes, size_t alignment) {
	void* ptr = m_upstream->allocate(bytes, alignment);
	XN_LOG_DEB("MemoryTracker: ({0}) Allocating {0} bytes at {0}", m_name, bytes, ptr);
	return ptr;
}

void DebugMemoryResource::do_deallocate(void* ptr, size_t bytes, size_t alignment) {
	XN_LOG_DEB("MemoryTracker: ({0}) Deallocating {0} bytes from {0}", m_name, bytes, ptr);
	m_upstream->deallocate(ptr, bytes, alignment);
}

bool DebugMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
	return m_upstream->is_equal(other);
}

}
