#include "chunkedPMR.hpp"
#include <cassert>
#include <cstdlib>

Core::ChunkedPMR::ChunkedPMR(std::pmr::memory_resource* upstream) : m_upstream(upstream) {}

Core::ChunkedPMR::~ChunkedPMR() { for(void* ptr : m_dealocationList) m_upstream->deallocate(ptr, s_CHUNK_SIZE, s_CHUNK_SIZE); }

void* Core::ChunkedPMR::do_allocate(std::size_t bytes, std::size_t alignment) { 
	assert(bytes % s_CHUNK_SIZE == 0 && "Memory allocation in ChunkedPMR must be a multiple of 4096 bytes");
	void* ptr = nullptr;
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(bytes == 1 && !m_chunkPool.empty()) {
		ptr = m_chunkPool.front();
		m_chunkPool.pop_front();
	}
	else {
		ptr = m_upstream->allocate(bytes, alignment);
		if(ptr == nullptr) throw std::bad_alloc();
		m_dealocationList.push_back(ptr);
	}
	return ptr;
}

void Core::ChunkedPMR::do_deallocate(void* ptr, [[maybe_unused]] std::size_t bytes, [[maybe_unused]] std::size_t alignment) { 
	const std::lock_guard<std::mutex> lock(m_mutex);
	m_chunkPool.push_back(ptr); 
}

