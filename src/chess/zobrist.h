#include <stdlib.h>

unsigned long long zobrist_table[64][13];
unsigned long long zobrist_stm;

// add castling + EP later

uint64_t prng(uint64_t seed);