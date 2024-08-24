#ifndef _XENON_ECS_CHUNKEDPMR_
#define _XENON_ECS_CHUNKEDPMR_

#include <list>
#include <memory_resource>

namespace Core {

class ChunkedPMR : public std::pmr::memory_resource {
	static constexpr std::size_t s_CHUNK_SIZE = 4096; //When changing this value change the error message as well
public:
	explicit ChunkedPMR(std::pmr::memory_resource* upstream = std::pmr::get_default_resource());
	~ChunkedPMR() override;
	ChunkedPMR(ChunkedPMR &&) = delete;
	ChunkedPMR(const ChunkedPMR &) = delete;
	ChunkedPMR &operator=(ChunkedPMR &&) = delete;
	ChunkedPMR &operator=(const ChunkedPMR &) = delete;

protected:
	void* do_allocate(std::size_t bytes, std::size_t alignment = s_CHUNK_SIZE) override;
	void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment = s_CHUNK_SIZE) override;
	[[nodiscard]] bool do_is_equal(const std::pmr::memory_resource& oth) const noexcept override { return this == &oth; }
private:
	std::list<void*> m_chunkPool;
	std::list<void*> m_dealocationList;
	std::mutex m_mutex;
	std::pmr::memory_resource* m_upstream;
};
}

#endif // !_XENON_ECS_CHUNKEDPMR_
