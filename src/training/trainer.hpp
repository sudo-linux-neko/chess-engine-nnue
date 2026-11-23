// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "data/data_loader.hpp"
#include "nnue/nnue.hpp"

namespace training {

class Trainer {
public:
    Trainer(int threads = 2);
    void train_from_folder(const std::filesystem::path& folder, const std::string& model_path, int epochs = 8);
    void self_play_loop(const std::filesystem::path& data_folder, const std::string& model_path, int rounds = 10);

private:
    int threads_;
};

}  // namespace training
