#include <atomic>
#include <cstdint>
#include <cstddef>

template <
    typename DataT,
    size_t N = 0,
    typename IndexT = uint32_t,
    bool lazy_push = false,
    bool lazy_pop = false
>    
class mpmc_queue {
public:
    explicit mpmc_queue(uint32_t n = N);
    bool push(value_type d);
    bool pop(value_type& d);
    bool push(value_type d, index_type& i);
    bool pop(value_type& d, index_type& i);
    bool exchange(index_type& i, value_type old_value, value_type new_value);
    bool push_keep_n(value_type d);
    bool push_keep_n(value_type d, index_type& i);

private:
    std::atomic<index_type> _write_index alignas(2 * cachelinesize);
    std::atomic<index_type> _read_index alignas(2 * cachelinesize);
    array_t _array;
};