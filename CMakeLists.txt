cmake_minimum_required(VERSION 3.16)
project(chess_nnue_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(chess_nnue_tests
    tests/test_main.cpp
    src/chess/board.cpp
    src/nnue/nnue.cpp
    src/data/data_loader.cpp
    src/training/trainer.cpp
)

target_include_directories(chess_nnue_tests PRIVATE src)

add_executable(chess_nnue_cli
    src/cli/main.cpp
    src/chess/board.cpp
    src/nnue/nnue.cpp
    src/data/data_loader.cpp
    src/training/trainer.cpp
)

target_include_directories(chess_nnue_cli PRIVATE src)
