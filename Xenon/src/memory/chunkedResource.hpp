#ifndef _XENON_MEMORY_CHUNKEDRESOURCE_
#define _XENON_MEMORY_CHUNKEDRESOURCE_

#include <condition_variable>
#include <cstddef>
#include <forward_list>
#include <memory_resource>
#include <set>

namespace Core {

class ChunkedResource : public std::pmr::memory_resource {

public:
	explicit ChunkedResource(std::pmr::memory_resource* upstream = std::pmr::get_default_resource());
	~ChunkedResource() override;
	ChunkedResource(ChunkedResource &&) = delete;
	ChunkedResource(const ChunkedResource &) = delete;
	ChunkedResource &operator=(ChunkedResource &&) = delete;
	ChunkedResource &operator=(const ChunkedResource &) = delete;

protected:
	void* do_allocate(std::size_t bytes, std::size_t alignment) override;
	void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
	[[nodiscard]] bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:
	struct FreeSpace {
		mutable std::byte* address;
		mutable std::size_t chunks;
		struct Compare {
			bool operator()(const FreeSpace& a, const FreeSpace& b) const {
				return a.address < b.address;
			}
		};
	};
	struct Block {
		void* address;
		std::size_t size;
	};

	static constexpr std::size_t s_CHUNK_SIZE = 4096; //When changing this value change the error message as well
	static constexpr std::size_t s_MINIMAL_FREE_CHUNKS = 64;
	// These two are sizes in terms of chunks:
	static constexpr std::size_t s_START_BLOCK_SIZE = 256ull;			// 1 MB
	static constexpr std::size_t s_MAX_BLOCK_SIZE = 256ull * 256ull;	// 256 MB

	std::forward_list<Block> m_blocks;
	std::set<FreeSpace, FreeSpace::Compare> m_freeChunks;
	std::size_t m_blockSize = s_START_BLOCK_SIZE;
	std::size_t m_freeChunksCount;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	std::pmr::memory_resource* m_upstream;

	void* allocFromPool();
	void addToPool(std::byte* chunkPtr);
	void tryReplenishPool();
	void tryDeplatePool();
};

}

#endif // !_XENON_MEMORY_CHUNKEDRESOURCE_
