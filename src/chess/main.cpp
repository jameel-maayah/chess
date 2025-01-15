#include "chess.h"

int main() {
    std::string_view fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Chess chess(fen);
    chess.print();
    return 0;
}
