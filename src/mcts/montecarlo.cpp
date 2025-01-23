#include "montecarlo.h"
#include "chess.h"
#include "tree.h"

int main() {
    Chess game = Chess(START_FEN);

    game.make_move(Move(e2, e4));
    game.make_move(Move(e7, e5));
    game.make_move(Move(d1, f3));
    game.make_move(Move(a7, a5));
    game.make_move(Move(f1, c4));
    game.make_move(Move(a5, a4));

    game.print();

    MCTree<Chess> tree(game);

    //tree.traverse(tree.get_root());
    for (int i = 0; i < 8000; i++) {
        tree.backpropagate(tree.traverse(), .1);
    }

    tree.print();

}
