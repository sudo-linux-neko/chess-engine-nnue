// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#include "training/trainer.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

namespace training {

Trainer::Trainer(int threads) : threads_(threads) {}

void Trainer::train_from_folder(const std::filesystem::path& folder, const std::string& model_path, int epochs) {
    data::TrainingSet set = data::DataLoader::load_from_folder(folder);
    if (set.samples.empty()) {
        std::cerr << "No training data found in " << folder << '\n';
        return;
    }
    nnue::NNUE model(64, 64);
    model.train(set.samples, epochs, 0.01);
    model.save(model_path);
    std::cout << "Training complete. Saved model to " << model_path << '\n';
}

void Trainer::self_play_loop(const std::filesystem::path& data_folder, const std::string& model_path, int rounds) {
    for (int i = 0; i < rounds; ++i) {
        data::TrainingSet set = data::DataLoader::load_from_folder(data_folder);
        if (set.samples.empty()) break;
        nnue::NNUE model(64, 64);
        model.load(model_path);
        model.train(set.samples, 2, 0.005);
        model.save(model_path);
        std::cout << "Self-play iteration " << (i + 1) << " complete.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

}  // namespace training
