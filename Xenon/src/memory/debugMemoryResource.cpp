#include "debugMemoryResource.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

DebugMemoryResource::DebugMemoryResource(std::pmr::memory_resource* upstream)
        : upstream_(upstream) {}

void* DebugMemoryResource::do_allocate(size_t bytes, size_t alignment) {
	void* ptr = upstream_->allocate(bytes, alignment);
	XN_LOG_DEB("MemoryTracker: Allocating {0} bytes at {0}", bytes, ptr);
	return ptr;
}

void DebugMemoryResource::do_deallocate(void* ptr, size_t bytes, size_t alignment) {
	XN_LOG_DEB("MemoryTracker: Deallocating {0} bytes from {0}", bytes, ptr);
	upstream_->deallocate(ptr, bytes, alignment);
}

bool DebugMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
	return upstream_->is_equal(other);
}

}
