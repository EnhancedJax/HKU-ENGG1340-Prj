# HKU-ENGG1340-Prj

A simple maze game. Oh also you can break walls. Oh and it's as big as your terminal.

<img width="763" alt="image" src="https://github.com/EnhancedJax/HKU-ENGG1340-Prj/assets/80020581/843d6f7d-119b-4ae4-ae88-b9cc9331c6c3">

Gameplay demo [here](https://connecthkuhk-my.sharepoint.com/:v:/g/personal/enhanjax_connect_hku_hk/EYksp2tpznBNp7xgifFYfiEB93MZzN7DHTvgsQctzYIcEQ?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=cKgLle)!

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
| Yep                      | Program codes in multiple files (recall separate compilation)                  |
| Sure                     | Proper indentation and naming styles                                           |
| Of course                | In-code documentation                                                          |

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

- Paul Miller's [python maze generator](https://github.com/138paulmiller/PyMaze/blob/master/maze.py) for inspiring and providing logic for our maze generation algorithm.
