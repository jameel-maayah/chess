#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "chess.h"
#include "movegen.h"

#define ROOT_PST 1.2
#define PST 1.1
#define ROOT_CPUCT 4//4
#define CPUCT 2.5//3.5



template <typename GameType>
class Node {
public:
    using Move = typename GameType::Move;
    using Color = typename GameType::Color;

    Node(Node* parent, Move move, Color color)
        : visits(0),
          value(0.0f),
          move(move),
          //policy_index(-1),
          color(color),
          num_children(-1),
          parent(parent) {}

    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }

    bool fully_expanded() {
        return num_children != -1;
    }

    void expand(GameType *game) {
        std::vector<Move> moves = generate_moves(game); // abstract this

        if (moves.size()) {
            children.reserve(moves.size());
            num_children = moves.size();

            for (const Move &move : moves) {
                children.push_back(new Node(this, move, ~color));
            }
        } else {
            num_children = 0;
        }
    }

    Node *best_uct() {
        std::cout << "finding best uct\n";

        Node *best = children[rand() % num_children];
        double max_score = -INFINITY;

        for (int i = 0; i < num_children; i++) {
            double score = children[i]->uct_score();//puct_score(game, children[i]);//root->visits); //(node->parent) ? node->parent->visits : root->visits
            std::cout << "score " << score << "\n";

            if (score > max_score) {
                max_score = score;
                best = children[i];   
            }
        }
        return best;
    }

    /*
    double puct_score() {
        if (visits == 0) {
            if (!parent->parent) {
                return INFINITY;
            } else {
                return parent->value / parent->visits * ((color == WHITE) ? 1 : -1) + // exploitation
                ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1) * prob; // exploration
            }
        }
        return (value / visits) * ((color == WHITE) ? 1 : -1) + // exploitation
            ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1) * prob; // exploration
        //sqrt(2 * log(node->parent->visits) / node->visits) *
        //node->prob;
    }
    */

    double uct_score() {
        if (visits == 0) {
            if (!parent->parent) {
                return INFINITY;
            } else {
                return parent->value / parent->visits * ((color == WHITE) ? 1 : -1) + // exploitation
                ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1);//* prob; // exploration
            }
        }
        return (value / visits) * ((color == WHITE) ? 1 : -1) + // exploitation
            ((parent) ? CPUCT : ROOT_CPUCT) * sqrt(parent->visits) / (visits + 1);// * prob; // exploration
        //sqrt(2 * log(node->parent->visits) / node->visits) *
        //node->prob;
    }


    void print_subtree(int depth) const {
        if (!visits) return;
        //if (node->value > 0 && node->parent) m
        for (int i = 0; i < depth; i++) printf(i == depth - 1 ? "╰━━━━━>" : "       "); //"⤷"
        std::cout << std::string(move) << std::endl;

        for (const Node *child : children) {
            child->print_subtree(depth + 1);
        }
    }
    

    inline int get_visits() const { return visits; }
    inline double get_value() const { return value; }
    //inline double get_prob() const { return prob; }
    inline Move get_move() const { return move; }
    //bool get_is_ready() const { return is_ready; }
    //bool get_is_white() const { return is_white; }

    void set_value(double val) { value = val; }
    //void set_prob(double prob_val) { prob = prob_val; }
    //void set_ready(bool ready) { is_ready = ready; }

    std::vector<Node*>& get_children() { return children; }

//private:
    int visits;
    double value;
    //double prob;
    Move move;
    Color color;
    //int policy_index;

    //bool is_ready;
    //bool is_white;

    Node* parent;
    std::vector<Node*> children;
    int num_children;
};





template <typename GameType>
class MCTree {
public:
    using Move = typename GameType::Move;
    using Node = Node<GameType>;

private:
    std::unique_ptr<Node> root;
    GameType game;

public:
    MCTree(const GameType& game);
    ~MCTree();

    Node* get_root() const { return root.get(); }
    Node* traverse(Node *node);
    void backpropagate(Node* leaf, double value);
    void make_move(const Move move);

    void print();
};

///        : root(std::make_unique<Node>(nullptr, Move(), true)), game(initial_game) {}