#include "node.h"
#include "tree.h"

template <typename GameType>
Node<GameType> *MCTree<GameType>::traverse() {
    Node *node = root.get();

    while (node->fully_expanded()) {
        node = node->best_uct();
        std::cout << node << std::endl;
        game->make_move(node->move);

        if (game->get_status()) {
            node->set_terminal(game->get_status());

            switch (game->get_status()) {
                case WIN_WHITE:
                    node->value += 1.0f;
                    break;
                case WIN_BLACK: 
                    node->value += -1.0f;
                    break;
                    
                default:
                    break;
            }
            node->num_children = 0;
            node->is_ready = true;

            break;
        }
    }

    if (node->num_children == -1) {
        node->expand(game);
    } 
    return node;
}

template <typename GameType>
void MCTree<GameType>::backpropagate(Node *node, double value) {
    node->visits++;
    while (node->parent) {
        node = node->parent;
        node->visits++;
        node->value += value;
        game->undo_move();
    }
}

template <typename GameType>
void MCTree<GameType>::print() {
    root->print_subtree(0);
}

template class MCTree<Chess>;
