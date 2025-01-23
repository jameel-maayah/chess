#include <bit>
#include <vector>
#include <cstdint>
#include <limits>

class BitField {
public:
    explicit constexpr BitField(size_t size)
     : size(size), internal((size + 63) / 64, 0) {}
    ~BitField() {}

    constexpr inline void set(size_t index)                       {   internal[index / 64] |= (1ULL << (index % 64));         }
    constexpr inline bool operator[](size_t index) const  {   return (internal[index / 64] &  (1ULL << (index % 64))) != 0;   }

    constexpr inline void fill() {
        for (size_t chunk = 0; chunk < internal.size(); ++chunk) {
            internal[chunk] = std::numeric_limits<uint64_t>::max();
        }
        if (size % 64 != 0) {
            internal.back() = (1ULL << (size % 64)) - 1;
        }
    }

    constexpr inline void clear() {
        for (size_t chunk = 0; chunk < internal.size(); ++chunk) {
            internal[chunk] = 0;
        }
    }

    constexpr inline size_t get_first() const {
        for (size_t chunk = 0; chunk < internal.size(); chunk++) {
            if (internal[chunk]) {
                return chunk * 64 + __builtin_ctzll(internal[chunk]);
            }
        }
        return -1;
    }

    explicit operator std::string() const {
        std::string str(size, '0');
        for (size_t i = 0; i < size; ++i) {
            if ((*this)[i]) {
                str[i] = '1';
            }
        } return str;
    }
private:
    size_t size;
    std::vector<uint64_t> internal;
};