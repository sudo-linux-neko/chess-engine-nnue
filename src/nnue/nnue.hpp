// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace nnue {

struct TrainingSample {
    std::array<int, 64> features{};
    int target{0};
};

class NNUE {
public:
    explicit NNUE(int feature_count = 64, int hidden = 64);
    int evaluate(const std::array<int, 64>& features) const;
    void train(const std::vector<TrainingSample>& samples, int epochs = 6, double learning_rate = 0.01);
    void save(const std::string& path) const;
    void load(const std::string& path);

private:
    int feature_count_;
    int hidden_;
    std::vector<double> weights_;
    std::vector<double> bias_;
};

}  // namespace nnue
