#pragma once

#include <torch/torch.h>
#include <torch/script.h>
#include <ATen/ATen.h>

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <atomic>

#include <stdexcept>

#include "network.h"
#include "chess.h"

using at::Tensor;

torch::Device MPSdevice(torch::kMPS);

// Move this somewhere else
template<typename GameType>
Tensor to_tensor(const GameType *game) {
    throw std::logic_error("to_tensor: Unsupported GameType");
}

template<>
Tensor to_tensor<Chess>(const Chess *game) {
    static Piece::Type pieces[13] = {
        Piece::WHITE_KING, Piece::WHITE_QUEEN, Piece::WHITE_ROOK, Piece::WHITE_BISHOP,
        Piece::WHITE_KNIGHT, Piece::WHITE_PAWN, Piece::EMPTY, Piece::BLACK_PAWN, Piece::BLACK_KNIGHT,
        Piece::BLACK_BISHOP, Piece::BLACK_ROOK, Piece::BLACK_QUEEN, Piece::BLACK_KING
    };    

    Tensor state_tensor = torch::zeros({13, 8, 8}, torch::kFloat32).to(torch::kCPU);
    auto accessor = state_tensor.accessor<float, 3>();

    if (game->side_to_move() == WHITE) {
        for (int piece = 0; piece < 13; piece++) {
            U64 bb = game->piece_mask(pieces[piece]);
            int ix = 0;

            while (bb) {
                int rshift = __builtin_ctzll(bb);
                bb >>= rshift;
                ix += rshift;

                accessor[12 - piece][7 - ix%8][ix/8] = 1.0f;
                bb &= bb - 1;
            }
        }
        
    } else {
        for (int piece = 0; piece < 13; piece++) {
            U64 bb = game->piece_mask(pieces[piece]);
            int ix = 0;

            while (bb) {
                int rshift = __builtin_ctzll(bb);
                bb >>= rshift;
                ix += rshift;

                accessor[piece][ix%8][ix/8] = 1.0f;
                bb &= bb - 1;
            }
        }
    }
    state_tensor[6].fill_(0); // Unused plane; will eventually use it for castling and en passant
    return state_tensor;
}


template <typename GameType>
struct InferenceRequest {
    GameType *game;
    Node<GameType> *node;
    int thread_id;
};

template<typename GameType>
class InferenceWorker {
public:
    InferenceWorker(int batch_size, ChessNet &model)
     : running(true),
     batch_size(batch_size),
     model(model) {}
    ~InferenceWorker() { halt(); }

    void run();
    void enqueue(const InferenceRequest<GameType>& request);

    void halt() {
        running = false;
        cv.notify_all();
    }
private:
    std::atomic<bool> running;
    std::queue<InferenceRequest<GameType>> queue;
    std::mutex queue_mutex;
    std::condition_variable cv;

    int batch_size;
    ChessNet &model;
};


template<typename GameType>
void InferenceWorker<GameType>::enqueue(const InferenceRequest<GameType>& request) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    queue.push(request);
    cv.notify_all();
}


template<typename GameType>
void InferenceWorker<GameType>::run() {
    while (running) {
        std::vector<InferenceRequest<GameType>> batch;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [&] { return !queue.empty() || !running; });

            if (!running && queue.empty()) break;

            while (!queue.empty() && batch.size() < batch_size) {
                InferenceRequest<GameType> top = queue.front();
                if (!top.node->is_ready) batch.push_back(top);
                queue.pop();
            }
        }

        if (batch.empty()) continue;
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<Tensor> states;
        for (const auto &req : batch) {
            states.push_back(to_tensor<GameType>(req.game));
        }

        auto input_batch = torch::stack(states).to(MPSdevice);
        Tensor policy_batch, value_batch;

        {
            torch::NoGradGuard no_grad;
            //std::lock_guard lock(training_mutex); // Training is not thread-safe

            //auto [policy_batch, value_batch] = model->forward(input_batch);
            std::tie(policy_batch, value_batch) = model->forward(input_batch);
        }

        // WDL value head
        value_batch = torch::softmax(value_batch, 1);
        
        policy_batch = policy_batch.to(torch::kCPU);
        value_batch = value_batch.to(torch::kCPU);
        
        auto policy_accessor = policy_batch.accessor<float, 2>();
        auto value_accessor = value_batch.accessor<float, 2>();

        // PUT THIS IN ANOTHER THREAD
        for (size_t i = 0; i < states.size(); ++i) {
            InferenceRequest<GameType> &req = batch[i];
            Node<GameType> *node = req.node;

            float sum_logits = 0;
            float pst = (node->parent ? PST : ROOT_PST);

            //assert(node->num_children);
            
            // Softmax the policy only after masking invalid moves
            for (int c = 0; c < req.node->num_children; c++) {
                Node<GameType> *child = node->children[c];

                float prob = exp(policy_accessor[i][child->policy_index] / pst);
                child->prob = prob;
                sum_logits += prob;
            }
            
            for (int c = 0; c < req.node->num_children; c++) {
                node->children[c]->prob /= sum_logits; 
            }

            // Compute expected value (pWin - pLoss)
            node->value = (value_accessor[0][0] - value_accessor[0][2]) * ((req.game->side_to_move() == WHITE) ? 1 : -1);
            node->is_ready = true; // Node is ready to backpropogate the value
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << std::setw(15) << std::left << "Inferences/s:"
                << std::setw(15) << std::right << static_cast<double>(1000000 * batch.size()) / std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
                << std::setw(15) << std::right << "Batch size:"
                << std::setw(15) << std::right << batch.size() 
                << std::endl;
        
    }
}