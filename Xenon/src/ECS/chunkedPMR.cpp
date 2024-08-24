#include "chunkedPMR.hpp"
#include <cstdlib>

Core::ChunkedPMR::~ChunkedPMR() { for(void* ptr : m_dealocationList) ::operator delete(ptr); }

void* Core::ChunkedPMR::do_allocate(std::size_t chunkes, std::size_t alignment) { 
	void* ptr = nullptr;
	std::lock_guard<std::mutex> lock(m_mutex);
	if(chunkes == 1 && !m_chunkPool.empty()) {
		ptr = m_chunkPool.front();
		m_chunkPool.pop_front();
	}
	else {
		ptr = aligned_alloc(alignment, chunkes * CHUNK_SIZE); //NOLINT
		if(ptr == nullptr) throw std::bad_alloc();
		m_dealocationList.push_back(ptr);
	}
	return ptr;
}

void Core::ChunkedPMR::do_deallocate(void* ptr, std::size_t, std::size_t) { std::lock_guard<std::mutex> lock(m_mutex); m_chunkPool.push_back(ptr); } //NOLINT


