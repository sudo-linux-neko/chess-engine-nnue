// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#include <filesystem>
#include <iostream>
#include <vector>

#include "chess/board.hpp"
#include "data/data_loader.hpp"
#include "nnue/nnue.hpp"

int main() {
    chess::Board board;
    board.reset();
    auto feats = board.piece_square_features();
    if (feats.size() != 64) return 1;

    nnue::NNUE net(64, 64);
    std::vector<nnue::TrainingSample> samples;
    nnue::TrainingSample sample;
    for (int i = 0; i < 64; ++i) sample.features[i] = i;
    sample.target = 1;
    samples.push_back(sample);
    net.train(samples, 2, 0.01);
    const int eval = net.evaluate(sample.features);
    std::cout << "Eval=" << eval << '\n';

    const std::filesystem::path data_dir = "tests/data";
    const auto loaded = data::DataLoader::load_from_folder(data_dir);
    if (loaded.samples.empty()) return 2;
    std::cout << "Loaded samples=" << loaded.samples.size() << '\n';
    return 0;
}
