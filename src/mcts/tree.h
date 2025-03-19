#pragma once

#include "node.h"

template <typename GameType>
class MCTree {
public:
    using Move = typename GameType::Move;
    using Node = Node<GameType>;

private:
    Node* root;
    GameType* game;

public:
    MCTree()
    : game(nullptr),
      root(nullptr) {}

    MCTree(GameType& game)
    : game(&game),
      root(new Node(nullptr, typename GameType::Move(), typename GameType::Color())) 
    { 
        root->color = game.side_to_move(); 
    }

    ~MCTree() {
        delete_tree(root);
    }

    void delete_tree(Node* node) {
        if (node) {
            for (auto* child : node->children) {
                delete_tree(child);
            }
            delete node;
        }
    }

    void init_root(GameType& game) {
        this->game = &game;

        delete root;
        root = new Node(nullptr, typename GameType::Move(), typename GameType::Color());
        root->color = ~game.side_to_move(); 
    }

    Node* get_root() const { return root; }
    Node* traverse();

    void backpropagate(Node* leaf, double value);

    Node* best_child();
    std::vector<Node> principal_variation();
    void update_root(const Move move);

    void print();
};
