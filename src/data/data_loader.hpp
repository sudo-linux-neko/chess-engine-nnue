// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "nnue/nnue.hpp"

namespace data {

struct TrainingSet {
    std::vector<nnue::TrainingSample> samples;
};

class DataLoader {
public:
    static TrainingSet load_from_folder(const std::filesystem::path& folder);
    static std::vector<std::filesystem::path> discover_files(const std::filesystem::path& folder);
};

}  // namespace data
