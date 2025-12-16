#ifndef FIXED_VECTOR_MEMORY_RESOURCE_HPP
#define FIXED_VECTOR_MEMORY_RESOURCE_HPP

#include <memory_resource>
#include <vector>
#include <cstddef>

// Стратегия 2: Фиксированный блок + vector для учёта блоков
class fixed_vector_memory_resource : public std::pmr::memory_resource {
private:
    std::vector<std::pair<void*, std::size_t>> allocated_blocks;
    char* fixed_buffer;
    std::size_t buffer_size;
    std::size_t offset;
    bool is_owned;

public:
    explicit fixed_vector_memory_resource(std::size_t size);
    fixed_vector_memory_resource(void* buffer, std::size_t size);
    ~fixed_vector_memory_resource() override;

    fixed_vector_memory_resource(const fixed_vector_memory_resource&) = delete;
    fixed_vector_memory_resource& operator=(const fixed_vector_memory_resource&) = delete;

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
};

#endif
