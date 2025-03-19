#pragma once

#include <torch/torch.h>
#include <torch/script.h>
#include <ATen/ATen.h>

struct ChessNetImpl : torch::nn::Module {
    torch::nn::Conv2d conv1{nullptr};
    torch::nn::BatchNorm2d bn1{nullptr};
    torch::nn::Sequential conv_blocks;

    torch::nn::Conv2d policy_conv{nullptr};
    torch::nn::BatchNorm2d policy_bn{nullptr};
    torch::nn::Linear policy_fc{nullptr};

    torch::nn::Conv2d value_conv{nullptr};
    torch::nn::BatchNorm2d value_bn{nullptr};
    torch::nn::Linear value_fc1{nullptr};
    torch::nn::Linear value_fc2{nullptr};

    ChessNetImpl();
    std::pair<torch::Tensor, torch::Tensor> forward(torch::Tensor x);
    std::shared_ptr<torch::nn::Module> clone();
};

TORCH_MODULE(ChessNet);


void test_inference_speed(int batch_size, int seconds);
int64_t count_parameters(ChessNet &model);