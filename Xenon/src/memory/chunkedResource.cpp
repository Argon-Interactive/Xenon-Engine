#include "chunkedResource.hpp"

#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <thread>

namespace Core {

ChunkedResource::ChunkedResource(std::pmr::memory_resource* upstream) 
: m_freeChunksCount(m_blockSize), m_upstream(upstream) {
	std::unique_lock lock(m_mutex);
	void* ptr = m_upstream->allocate(m_blockSize * s_CHUNK_SIZE, s_CHUNK_SIZE);
	m_blocks.emplace_back(ptr, m_blockSize * s_CHUNK_SIZE);
	m_freeChunks.emplace(static_cast<std::byte*>(ptr), m_blockSize, 0);
	lock.unlock();
	m_cond.notify_all();
}

ChunkedResource::~ChunkedResource() {
	for(const auto& block : m_blocks)
		m_upstream->deallocate(block.address, block.size, s_CHUNK_SIZE); 
}

void* ChunkedResource::do_allocate(std::size_t bytes, std::size_t alignment) { 
	std::unique_lock lock(m_mutex);
	if (bytes == s_CHUNK_SIZE) {
		while(m_freeChunks.empty()) m_cond.wait(lock);
		return allocFromPool();
	}

	// This allocation is special: it is only for pre-defined scene data
	void* ptr = m_upstream->allocate(bytes, alignment);
	m_blocks.emplace_back(ptr, bytes);
	return ptr;
}

void ChunkedResource::do_deallocate(void* ptr, [[maybe_unused]] std::size_t bytes, [[maybe_unused]] std::size_t alignment) { 
	const std::lock_guard<std::mutex> lock(m_mutex);
	addToPool(static_cast<std::byte*>(ptr));
}

bool ChunkedResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept { 
	return this == &other; 
}



std::size_t ChunkedResource::findBlock(std::byte* ptr) const {
	for(std::size_t i = 0; i < m_blocks.size(); i++) {
		const auto& blk = m_blocks[i];
		if(blk.address <= ptr && blk.address > ptr - blk.size)
			return i;
	}
	throw std::invalid_argument("Trying to deallocate memory that is not in any ChunkedResource block");
}

void* ChunkedResource::allocFromPool() {
	auto chunk = m_freeChunks.begin();
	auto* ptr = chunk->address;
	if(chunk->chunks > 1) {
		chunk->address += s_CHUNK_SIZE;
		chunk->chunks--;
	} else {
		m_freeChunks.erase(chunk);
	}
	m_freeChunksCount--;
	tryReplenishPool();
	return ptr;
}

void ChunkedResource::addToPool(std::byte* chunkPtr) {
	auto blockIndex = findBlock(chunkPtr);
	const FreeSpace chunk(chunkPtr, 1, blockIndex);
	auto it = m_freeChunks.lower_bound(chunk);

	bool mergedPrev = false;
	bool mergedNext = false;

	auto prevIt = it;
	if(it != m_freeChunks.begin()) {
		prevIt = std::prev(it);
		if(prevIt->address + prevIt->chunks == chunkPtr) {
			prevIt->chunks++;
			mergedPrev = true;
		}
	}

	if(it != m_freeChunks.end() && it->address == chunkPtr + s_CHUNK_SIZE) {
		if(mergedPrev) {
			prevIt->chunks += it->chunks;
			m_freeChunks.erase(it);
		} else {
			it->address -= s_CHUNK_SIZE;
			it->chunks++;
		}
		mergedNext = true;
	}

	if(!mergedPrev && !mergedNext)
		m_freeChunks.insert(it, chunk);
	m_freeChunksCount++;
	tryDeplatePool();
}

void ChunkedResource::tryReplenishPool() {
	if(m_allocating) return;
	if(m_freeChunksCount >= s_MINIMAL_FREE_CHUNKS) return;

	m_allocating = true;
	std::thread t{ [this](){
		std::unique_lock lock(m_mutex);
		m_blockSize *= 2;
		if(m_blockSize > s_MAX_BLOCK_SIZE) m_blockSize = s_MAX_BLOCK_SIZE;

		lock.unlock();
		void* ptr = m_upstream->allocate(m_blockSize * s_CHUNK_SIZE, s_CHUNK_SIZE);
		lock.lock();

		m_allocating = false;
		m_blocks.emplace_back(ptr, m_blockSize * s_CHUNK_SIZE);
		m_freeChunks.emplace(static_cast<std::byte*>(ptr), m_blockSize, m_blocks.size() - 1);
		m_freeChunksCount += m_blockSize;
		lock.unlock();
		m_cond.notify_all();
	}};
	t.detach();
}

void ChunkedResource::tryDeplatePool() {
	return;
	if(m_freeChunksCount < 2 * s_MINIMAL_FREE_CHUNKS + m_blockSize) return;
	if(std::prev(m_freeChunks.end())->chunks != m_blockSize) return;

	std::thread t{ [this](){
		std::unique_lock lock(m_mutex);
		auto block = std::prev(m_freeChunks.end());
		m_freeChunksCount -= m_blockSize;
		m_freeChunks.erase(block);
		
		lock.unlock();
		m_upstream->deallocate(block->address, s_CHUNK_SIZE * m_blockSize); 
	}};
	t.detach();
}

}
