// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#include "data/data_loader.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace data {
namespace fs = std::filesystem;

namespace {
std::string to_lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, delim)) out.push_back(item);
    return out;
}
}  // namespace

std::vector<fs::path> DataLoader::discover_files(const fs::path& folder) {
    std::vector<fs::path> files;
    if (!fs::exists(folder)) return files;
    for (const auto& entry : fs::recursive_directory_iterator(folder)) {
        if (entry.is_regular_file()) files.push_back(entry.path());
    }
    return files;
}

TrainingSet DataLoader::load_from_folder(const fs::path& folder) {
    TrainingSet set;
    for (const auto& path : discover_files(folder)) {
        const std::string ext = to_lower(path.extension().string());
        std::ifstream in(path);
        if (!in) continue;
        if (ext == ".csv") {
            std::string line;
            std::getline(in, line);
            while (std::getline(in, line)) {
                auto cols = split(line, ',');
                if (cols.size() < 2) continue;
                nnue::TrainingSample sample;
                for (int i = 0; i < 64; ++i) {
                    sample.features[i] = std::stoi(cols[i % cols.size()]);
                }
                sample.target = std::stoi(cols.back());
                set.samples.push_back(sample);
            }
        } else if (ext == ".pgn") {
            std::string line;
            std::vector<std::string> moves;
            while (std::getline(in, line)) {
                if (line.rfind("[", 0) == 0) continue;
                if (line.empty()) continue;
                auto parts = split(line, ' ');
                for (const auto& part : parts) {
                    if (part.empty()) continue;
                    if (part[0] == '[' || part[0] == '1' || part[0] == '.') continue;
                    moves.push_back(part);
                }
            }
            if (!moves.empty()) {
                nnue::TrainingSample sample;
                for (int i = 0; i < 64; ++i) sample.features[i] = 0;
                sample.target = static_cast<int>(moves.size());
                set.samples.push_back(sample);
            }
        } else {
            std::string line;
            while (std::getline(in, line)) {
                if (line.empty()) continue;
                nnue::TrainingSample sample;
                for (int i = 0; i < 64; ++i) sample.features[i] = 0;
                sample.target = static_cast<int>(line.size());
                set.samples.push_back(sample);
            }
        }
    }
    return set;
}

}  // namespace data
