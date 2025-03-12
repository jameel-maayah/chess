#include <stdlib.h>
#include <stdio.h>

#include <torch/torch.h>
#include <torch/script.h>
#include <ATen/ATen.h>

#include <vector>
#include <time.h>

#include "network.h"

ChessNet model;

// Dummy net for testing
ChessNetImpl::ChessNetImpl() {
    const int filters = 64;
    const int num_blocks = 6;

    conv1 = register_module("conv1", torch::nn::Conv2d(torch::nn::Conv2dOptions(13, filters, 3).padding(1)));
    bn1 = register_module("bn1", torch::nn::BatchNorm2d(filters));

    for (int i = 0; i < num_blocks - 1; ++i) {
        conv_blocks->push_back(torch::nn::Conv2d(torch::nn::Conv2dOptions(filters, filters, 3).padding(1)));
        conv_blocks->push_back(torch::nn::BatchNorm2d(filters));
        conv_blocks->push_back(torch::nn::ReLU());
        conv_blocks->push_back(torch::nn::Conv2d(torch::nn::Conv2dOptions(filters, filters, 3).padding(1)));
        conv_blocks->push_back(torch::nn::BatchNorm2d(filters));
        conv_blocks->push_back(torch::nn::ReLU());
    }
    conv_blocks = register_module("conv_blocks", conv_blocks);

    policy_conv = register_module("policy_conv", torch::nn::Conv2d(torch::nn::Conv2dOptions(filters, 2, 1)));
    policy_bn = register_module("policy_bn", torch::nn::BatchNorm2d(2));
    policy_fc = register_module("policy_fc", torch::nn::Linear(2 * 8 * 8, 64 * 64));

    value_conv = register_module("value_conv", torch::nn::Conv2d(torch::nn::Conv2dOptions(filters, 1, 1)));
    value_bn = register_module("value_bn", torch::nn::BatchNorm2d(1));
    value_fc1 = register_module("value_fc1", torch::nn::Linear(8 * 8, 256));

    value_fc2 = register_module("value_fc2", torch::nn::Linear(256, 3));
}

std::pair<torch::Tensor, torch::Tensor> ChessNetImpl::forward(torch::Tensor x) {
    x = torch::relu(bn1(conv1(x)));
    x = conv_blocks->forward(x);

    auto p = torch::relu(policy_bn(policy_conv(x)));
    p = p.view({p.size(0), -1});
    p = policy_fc(p); // Dont apply softmax yet, cross entropy loss expects raw logits

    auto v = torch::relu(value_bn(value_conv(x)));
    v = v.view({v.size(0), -1});
    
    v = torch::relu(value_fc1(v));
    v = value_fc2(v);

    return {p, v};
}

int64_t count_parameters(ChessNet &model) {
    int64_t total = 0;

    for (const auto& param : model->parameters()) {
        total += param.numel();
    }
    return total;
}

void test_inference_speed(int batch_size, int seconds) {
    auto device = torch::kMPS;

    model->to(device);
    model->to(torch::kFloat32);
    model->eval();

    torch::Tensor input = torch::randn({batch_size, 13, 8, 8}, torch::TensorOptions().dtype(torch::kFloat32).device(device));
    model->forward(input);
    int i = 0;

    clock_t start_time = clock();
    clock_t end_time = start_time + (clock_t)(seconds * CLOCKS_PER_SEC);

    while (clock() < end_time) {
        auto outputs = model->forward(input); 
        i++;
    }
    std::cout << i / seconds << "inferences/sec, batch size " << batch_size << ", " << batch_size * i / seconds << " nodes/s" << std::endl;
}