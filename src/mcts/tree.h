#pragma once

#include "node.h"

template <typename GameType>
class MCTree {
public:
    using Move = typename GameType::Move;
    using Node = Node<GameType>;

private:
    std::unique_ptr<Node> root;
    GameType &game;

public:
    MCTree(GameType& game) 
    : game(game),
      root(std::make_unique<Node>(nullptr, typename GameType::Move(), typename GameType::Color())) 
      { root->color = ~game.side_to_move(); }
    ~MCTree() { /* delete game ? */}

    Node *get_root() const { return root.get(); }
    Node *traverse();

    void backpropagate(Node* leaf, double value);

    Node *best_child();
    std::vector<Node> principal_variation();
    void make_move(const Move move);

    void print();
};
