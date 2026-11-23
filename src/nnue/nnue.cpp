// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#include "nnue/nnue.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

namespace nnue {

namespace {
double sigmoid(double x) { return 1.0 / (1.0 + std::exp(-x)); }
}

NNUE::NNUE(int feature_count, int hidden) : feature_count_(feature_count), hidden_(hidden) {
    const int w_size = feature_count_ * hidden_;
    weights_.resize(w_size, 0.0);
    bias_.resize(hidden_, 0.0);
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-0.1, 0.1);
    for (int i = 0; i < w_size; ++i) weights_[i] = dist(rng);
    for (int i = 0; i < hidden_; ++i) bias_[i] = dist(rng);
}

int NNUE::evaluate(const std::array<int, 64>& features) const {
    double sum = 0.0;
    for (int i = 0; i < hidden_; ++i) {
        double acc = bias_[i];
        for (int j = 0; j < feature_count_; ++j) {
            acc += weights_[i * feature_count_ + j] * static_cast<double>(features[j]);
        }
        sum += sigmoid(acc);
    }
    const double out = sum / static_cast<double>(hidden_);
    return static_cast<int>(std::lround(out * 2000.0 - 1000.0));
}

void NNUE::train(const std::vector<TrainingSample>& samples, int epochs, double learning_rate) {
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (const auto& sample : samples) {
            std::vector<double> activations(hidden_, 0.0);
            for (int i = 0; i < hidden_; ++i) {
                double acc = bias_[i];
                for (int j = 0; j < feature_count_; ++j) {
                    acc += weights_[i * feature_count_ + j] * static_cast<double>(sample.features[j]);
                }
                activations[i] = sigmoid(acc);
            }
            const double pred = [&]() {
                double s = 0.0;
                for (double a : activations) s += a;
                return s / static_cast<double>(hidden_);
            }();
            const double error = sample.target - pred;
            for (int i = 0; i < hidden_; ++i) {
                const double delta = error * (1.0 - activations[i]) * activations[i] * learning_rate;
                bias_[i] += delta;
                for (int j = 0; j < feature_count_; ++j) {
                    weights_[i * feature_count_ + j] += delta * static_cast<double>(sample.features[j]);
                }
            }
        }
    }
}

void NNUE::save(const std::string& path) const {
    std::ofstream out(path);
    out << feature_count_ << '\n' << hidden_ << '\n';
    for (double w : weights_) out << w << '\n';
    for (double b : bias_) out << b << '\n';
}

void NNUE::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Cannot open NNUE file");
    in >> feature_count_ >> hidden_;
    weights_.resize(feature_count_ * hidden_);
    bias_.resize(hidden_);
    for (double& w : weights_) in >> w;
    for (double& b : bias_) in >> b;
}

}  // namespace nnue
