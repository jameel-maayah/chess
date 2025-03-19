#include "node.h"
#include "tree.h"



template <typename GameType>
Node<GameType> *MCTree<GameType>::traverse() {
    Node *node = root;

    while (node->fully_expanded()) {
        node = node->best_uct();
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

                // Value of a draw is the contempt factor
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
Node<GameType>* MCTree<GameType>::best_child() {
    Node *best = nullptr;
    int max_visits = -1;

    for (auto* child : root->children) {
        if (child->get_visits() > max_visits) {
            max_visits = child->get_visits();
            best = child;
        }
    }

    return best;
}

template <typename GameType>
void MCTree<GameType>::update_root(Move move) {
    Node *new_root = nullptr;
    typename GameType::Color color = ~root->color;

    for (auto *child : root->children) {
        if (child->move == move) {
            new_root = child;
            new_root->parent = nullptr;
            break;
        } else {
            delete child;
        }
    }

    if (new_root) {
        root = new_root;
    } else {
        root = new Node(nullptr, Move(), color);
    }
}


template <typename GameType>
void MCTree<GameType>::print() {
    root->print_subtree(0);
}

template class MCTree<Chess>;