#ifndef _XENON_MEMORY_DEBUGMEMORYRESOURCE_
#define _XENON_MEMORY_DEBUGMEMORYRESOURCE_

#include <memory_resource>
#include <string>

namespace Core {

class DebugMemoryResource : public std::pmr::memory_resource {
public:
    explicit DebugMemoryResource(std::string name = "unknown", std::pmr::memory_resource* upstream = std::pmr::get_default_resource());

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* ptr, size_t bytes, size_t alignment) override;
    [[nodiscard]] bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:
    std::pmr::memory_resource* m_upstream;
	std::string m_name;
};

}

#endif // !_XENON_MEMORY_DEBUGMEMORYRESOURCE_
