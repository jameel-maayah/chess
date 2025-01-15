#include <iostream>

#include <string_view>
#include <sstream>
#include <ranges>

#include <vector>

#include "chess.h"
#include "bitboard.h"
#include "constants.h"
#include "piece.h"
#include "move.h"



Chess::Chess(std::string_view fen) {
    const std::string_view delimiter = " ";
    auto args = std::views::split(fen, delimiter);

    for (const auto& arg : args) {
        std::string token(arg.begin(), arg.end()); 
        std::cout << token << std::endl;
    }
    // todo: fen parsing
}

Chess::~Chess() { /* might need later? */ }

void Chess::print() {
    printf("status %d, hash %d\n", status, zobrist_hash);

    for (Square sq = a1; sq < NULL_SQ; ++sq) {
        std::cout << board[sq];
        if (int(sq+1) % 8 == 0) std::cout << std::endl;
    }
    // todo: ascii piece representations
}