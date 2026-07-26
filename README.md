# sudodoku

A minimal sudoku client written using raylib

<p align="center">
  <img src="sudodoku.gif" alt="Screenshot" height="300">
</p>

## Keybinds

- **Arrow Keys / WASD**: Move caret
- **1-9**: Place number
- **Shift + 1-9**: Add pencil mark
- **0 / Shift + 0**: Clear cell / Clear pencil marks
- **P**: Pause/Unpause game
- **R**: Reset current game
- **ESC**: Save progress and exit to menu

## CLI Usage

```bash
sudodoku play <difficulty> [index]     # Play a puzzle (easy, medium, hard)
sudodoku list [easy|medium|hard]       # List unsolved puzzles
sudodoku add <puzzle> <difficulty>     # Add a new 81-character puzzle string
sudodoku history                       # View solve history
sudodoku status                        # View puzzle statistics
sudodoku ongoing                       # View currently paused/in-progress games
```

## Managing Puzzle Files

Sudodoku stores all of its data in `~/.sudodoku/`:

```text
~/.sudodoku/
├── in_progress.txt
├── puzzles_easy.txt
├── puzzles_medium.txt
├── puzzles_hard.txt
└── solved.txt
```

You can edit these files directly if you want to import, export, or manage puzzles in bulk.

### Puzzle files

`puzzles_easy.txt`, `puzzles_medium.txt`, and `puzzles_hard.txt` each contain one puzzle per line as an 81-character puzzle string.

Example:

```text
530070000600195000098000060800060003400803001700020006060000280000419005000080079
```

You can paste multiple puzzle strings into the appropriate file, one puzzle per line. Sudodoku will automatically load them.

> **Note:** Ensure every puzzle is a valid 81-character Sudoku puzzle.
