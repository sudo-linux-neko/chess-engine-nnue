// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace chess {

enum class PieceType : uint8_t { None = 0, Pawn, Knight, Bishop, Rook, Queen, King };
enum class Color : uint8_t { White = 0, Black = 1 };
struct Piece {
    PieceType type{PieceType::None};
    Color color{Color::White};
};

class Board {
public:
    Board();
    void reset();
    bool load_fen(const std::string& fen);
    std::string to_fen() const;
    bool make_move(const std::string& move);
    bool make_move_from_uci(const std::string& uci);
    std::string generate_legal_moves() const;
    Piece at(int square) const;
    bool is_in_check(Color side) const;
    bool is_game_over() const;
    std::array<int, 64> piece_square_features() const;

private:
    std::array<Piece, 64> board_{};
    Color side_to_move_{Color::White};
    bool white_castling_kingside_{true};
    bool white_castling_queenside_{true};
    bool black_castling_kingside_{true};
    bool black_castling_queenside_{true};
    int en_passant_target_{-1};
    int halfmove_clock_{0};
    int fullmove_number_{1};
};

}  // namespace chess
