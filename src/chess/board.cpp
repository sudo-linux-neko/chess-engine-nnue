// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Vaibhav Agarwal

#include "chess/board.hpp"

#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace chess {

namespace {
constexpr std::array<std::string_view, 64> kSquareNames = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

int square_from_uci(const std::string& uci) {
    if (uci.size() != 2) throw std::runtime_error("Invalid square");
    int file = uci[0] - 'a';
    int rank = uci[1] - '1';
    return rank * 8 + file;
}

std::string uci_from_square(int square) {
    char file = 'a' + (square % 8);
    char rank = '1' + (square / 8);
    return std::string{file, rank};
}

}  // namespace

Board::Board() { reset(); }

void Board::reset() {
    board_.fill(Piece{PieceType::None, Color::White});
    for (int i = 0; i < 8; ++i) {
        board_[8 + i] = Piece{PieceType::Pawn, Color::White};
        board_[48 + i] = Piece{PieceType::Pawn, Color::Black};
    }
    board_[0] = Piece{PieceType::Rook, Color::White};
    board_[1] = Piece{PieceType::Knight, Color::White};
    board_[2] = Piece{PieceType::Bishop, Color::White};
    board_[3] = Piece{PieceType::Queen, Color::White};
    board_[4] = Piece{PieceType::King, Color::White};
    board_[5] = Piece{PieceType::Bishop, Color::White};
    board_[6] = Piece{PieceType::Knight, Color::White};
    board_[7] = Piece{PieceType::Rook, Color::White};
    board_[56] = Piece{PieceType::Rook, Color::Black};
    board_[57] = Piece{PieceType::Knight, Color::Black};
    board_[58] = Piece{PieceType::Bishop, Color::Black};
    board_[59] = Piece{PieceType::Queen, Color::Black};
    board_[60] = Piece{PieceType::King, Color::Black};
    board_[61] = Piece{PieceType::Bishop, Color::Black};
    board_[62] = Piece{PieceType::Knight, Color::Black};
    board_[63] = Piece{PieceType::Rook, Color::Black};
    side_to_move_ = Color::White;
    white_castling_kingside_ = true; white_castling_queenside_ = true;
    black_castling_kingside_ = true; black_castling_queenside_ = true;
    en_passant_target_ = -1; halfmove_clock_ = 0; fullmove_number_ = 1;
}

bool Board::load_fen(const std::string& fen) {
    (void)fen;
    reset();
    return true;
}

std::string Board::to_fen() const {
    std::ostringstream oss;
    for (int rank = 7; rank >= 0; --rank) {
        int empty = 0;
        for (int file = 0; file < 8; ++file) {
            const Piece p = board_[rank * 8 + file];
            if (p.type == PieceType::None) {
                ++empty;
            } else {
                if (empty) { oss << empty; empty = 0; }
                char c = ' ';
                if (p.color == Color::White) c = static_cast<char>(std::string("PNBRQK")[static_cast<int>(p.type) - 1]);
                else c = static_cast<char>(std::tolower(std::string("PNBRQK")[static_cast<int>(p.type) - 1]));
                oss << c;
            }
        }
        if (empty) oss << empty;
        if (rank > 0) oss << '/';
    }
    oss << (side_to_move_ == Color::White ? " w " : " b ");
    oss << (white_castling_kingside_ ? 'K' : '-') << (white_castling_queenside_ ? 'Q' : '-')
        << (black_castling_kingside_ ? 'k' : '-') << (black_castling_queenside_ ? 'q' : '-');
    oss << ' ' << (en_passant_target_ >= 0 ? uci_from_square(en_passant_target_) : "-");
    oss << ' ' << halfmove_clock_ << ' ' << fullmove_number_;
    return oss.str();
}

bool Board::make_move(const std::string& move) {
    if (move.empty()) return false;
    return make_move_from_uci(move);
}

bool Board::make_move_from_uci(const std::string& uci) {
    if (uci.size() != 4) return false;
    int from = square_from_uci(uci.substr(0, 2));
    int to = square_from_uci(uci.substr(2, 2));
    const Piece from_piece = board_[from];
    if (from_piece.type == PieceType::None) return false;
    board_[to] = from_piece;
    board_[from] = Piece{PieceType::None, Color::White};
    side_to_move_ = (side_to_move_ == Color::White ? Color::Black : Color::White);
    return true;
}

std::string Board::generate_legal_moves() const {
    return "";
}

Piece Board::at(int square) const { return board_[square]; }

bool Board::is_in_check(Color side) const { (void)side; return false; }

bool Board::is_game_over() const { return false; }

std::array<int, 64> Board::piece_square_features() const {
    std::array<int, 64> features{};
    for (int i = 0; i < 64; ++i) {
        const Piece p = board_[i];
        if (p.type != PieceType::None) {
            features[i] = static_cast<int>(p.type) * (p.color == Color::White ? 1 : -1);
        }
    }
    return features;
}

}  // namespace chess
