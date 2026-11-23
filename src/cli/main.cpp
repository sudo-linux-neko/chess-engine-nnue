// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#include <filesystem>
#include <iostream>
#include <string>

#include "training/trainer.hpp"

int main(int argc, char** argv) {
    std::filesystem::path data_folder = "data";
    std::string model_path = "model.nnue";
    int epochs = 6;
    int rounds = 2;
    int threads = 2;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--data" && i + 1 < argc) data_folder = argv[++i];
        else if (arg == "--model" && i + 1 < argc) model_path = argv[++i];
        else if (arg == "--epochs" && i + 1 < argc) epochs = std::stoi(argv[++i]);
        else if (arg == "--rounds" && i + 1 < argc) rounds = std::stoi(argv[++i]);
        else if (arg == "--threads" && i + 1 < argc) threads = std::stoi(argv[++i]);
        else if (arg == "--self-play") {
            training::Trainer trainer(threads);
            trainer.self_play_loop(data_folder, model_path, rounds);
            return 0;
        }
    }

    training::Trainer trainer(threads);
    trainer.train_from_folder(data_folder, model_path, epochs);
    return 0;
}
