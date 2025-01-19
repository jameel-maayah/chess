#include <stdlib.h>

unsigned long long zobrist_table[64][16];
unsigned long long zobrist_stm;
// add castling + EP later

class Zobrist {
    private:
        uint64_t prng(uint64_t seed);

    public:
        void update( /* */ );
};