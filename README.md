# HKU-ENGG1340-Prj

A simple maze game. Oh also you can break walls. Oh and it's as big as your terminal.

## Features

- Randomly generated maze, the size of your terminal
- Directional button minigame
- Breakable walls
- Smooth movement
- Satisfying gameplay
- Save & Load

## How to play

1. Move
2. Break
3. Finish
4. Replay!

- Use `W`/`S`/`Enter` to navigate main menu
- Use `WASD` to move, complet minigames and break walls
- Use `P` to save & pause the game

## Coding elements

| Coding                   | Elements                                                                       |
| ------------------------ | ------------------------------------------------------------------------------ |
| Randomly generated mazes | Generation of random game sets or events                                       |
| `State`, `lookup`        | Data structures for storing game status (e.g., arrays, STL containers)         |
| `map2D**`, `grid2D**`    | Dynamic memory management (e.g., dynamic arrays, linked lists, STL containers) |
| Save & load              | File input/output (e.g., for loading/saving game status)                       |
| ✔                        | Program codes in multiple files (recall separate compilation)                  |
| ✔                        | Proper indentation and naming styles                                           |
| ✔                        | In-code documentation                                                          |

## Compilation and running instructions

```zsh
cd build
make
./game
```

_Notes_

1. Do NOT resize terminal during game runtime
2. Do NOT scroll terminal during game runtime

## Group Members

- Jax (3036268218)
- Kevin (3036274059)
- Sandy (3036222185)
- Hugo (3036225577)
- Kim (3036226789)

## Attribution

- [https://github.com/138paulmiller/PyMaze/blob/master/maze.py](Paul Miller's python maze generator inspired our Maze2D class)
