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
