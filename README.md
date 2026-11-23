# chess-engine-nnue

This project is a compact C++ starter for a CPU-first chess training pipeline. It includes:

- a small chess board representation
- a lightweight NNUE-style evaluation model
- a folder-based data loader that accepts CSV and PGN-style files
- a CLI entry point that can train from a folder or run self-play iterations

## How to build

```bash
cmake -S . -B build
cmake --build build -j2
```

## How to run

Train from a folder:

```bash
./build/chess_nnue_cli --data data --model model.nnue --epochs 6
```

Run self-play iterations:

```bash
./build/chess_nnue_cli --data data --model model.nnue --self-play --rounds 3 --threads 2
```

## Data layout

Place training files inside a folder such as `data/`.

Supported examples:
- CSV files with feature columns and a final target column
- PGN files containing moves
- plain text files (the loader will convert them into simple samples)

## Notes

This is intentionally lightweight and CPU-friendly. It is not a full-strength chess engine and should not be treated as a production-grade Stockfish replacement. The goal here is a working, easy-to-extend baseline you can improve.
