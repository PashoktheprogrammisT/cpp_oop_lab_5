#include "fixed_vector_memory_resource.hpp"
#include <iostream>
#include <cstring>

fixed_vector_memory_resource::fixed_vector_memory_resource(std::size_t size)
    : fixed_buffer(new char[size]), buffer_size(size), offset(0), is_owned(true) {}

fixed_vector_memory_resource::fixed_vector_memory_resource(void* buffer, std::size_t size)
    : fixed_buffer(static_cast<char*>(buffer)), buffer_size(size), offset(0), is_owned(false) {}

fixed_vector_memory_resource::~fixed_vector_memory_resource() {
    if (!allocated_blocks.empty()) {
        std::cout << "Warning: " << allocated_blocks.size() 
                  << " blocks not deallocated" << std::endl;
    }
    if (is_owned) delete[] fixed_buffer;
}

void* fixed_vector_memory_resource::do_allocate(std::size_t bytes, std::size_t alignment) {
    std::size_t space = buffer_size - offset;
    void* ptr = fixed_buffer + offset;
    
    if (std::align(alignment, bytes, ptr, space)) {
        offset = static_cast<char*>(ptr) - fixed_buffer + bytes;
        allocated_blocks.emplace_back(ptr, bytes);
        return ptr;
    }
    throw std::bad_alloc();
}

void fixed_vector_memory_resource::do_deallocate(void* p, std::size_t bytes, std::size_t alignment) {
    for (auto it = allocated_blocks.begin(); it != allocated_blocks.end(); ++it) {
        if (it->first == p) {
            allocated_blocks.erase(it);
            break;
        }
    }
}

bool fixed_vector_memory_resource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}
