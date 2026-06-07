#pragma once

#include <vector>

namespace low_latency {

class arena_allocator {
    std::vector<std::byte> buffer;

    struct destructor_entry {
        void *ptr;
        void (*callback)(void*);
    };

    std::vector<destructor_entry> callback;
    size_t allocated;

    template<typename T>
    size_t getPadding(void* ptr) {
        size_t alignment = alignof(T);
        size_t misalignment = reinterpret_cast<uintptr_t>(ptr) % alignment;
        return misalignment == 0 ? 0 : alignment - (misalignment % alignment);
    }

public:
    arena_allocator(size_t size_): buffer{size_}, allocated{0} {}
    arena_allocator(const arena_allocator& that) = delete;
    arena_allocator& operator=(const arena_allocator& that) = delete;

    arena_allocator(arena_allocator&& that) : buffer{std::move(that.buffer)}, 
        callback{std::move(that.callback)},
        allocated{std::move(that.allocated)} {}
    
    arena_allocator& operator=(arena_allocator&& that) {
        if (this != &that) {
            buffer = std::move(that.buffer);
            callback = std::move(that.callback);
            allocated = std::move(that.allocated);
        }
        
        return *this;
    }

    template<typename T, typename... Args>
    T* allocate(Args&&... args) {
        size_t size = sizeof(T);
        size_t padding = getPadding<T>(&buffer[allocated]);
        if (allocated + size + padding > buffer.size()) {
            throw std::bad_alloc();
        }

        T* res = new (&buffer[allocated + padding]) T(std::forward<Args>(args)...);
        
        destructor_entry entry;
        entry.ptr = res;
        entry.callback = [](void* ptr) {
            static_cast<T*>(ptr)->~T();
        };

        callback.push_back(entry);
        
        allocated += padding + size;
        return res;
    }

    ~arena_allocator() {
        for (auto ptr = callback.rbegin(); ptr != callback.rend(); ptr++) {
            (*ptr).callback((*ptr).ptr);
        }
    }
};

};