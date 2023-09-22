# Conway's Game of Life 
This is an implementation of Conway's Game of Life, a cellular automaton devised by John Conway, using Unreal Engine 5.3 with C++.
It is a zero-player game that simulates the evolution of the population of cells, where each cell can be either dead or alive.

![logo]

## Rules
The evolution of the population is determined by the following rules:

1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

These rules give rise to interesting patterns as the generations progress.

## Visualize
- Live and Dead cells.
- Generation and Population counting.

## Customization

### Instance
Customizations you can make to the cellular automaton instance (without the need to modify code):
- Color of Live/Dead cells.
- Width and Height of the board (grid of cells).
- Time between generations (In Rate).
- Input for each action (create, clear, stop/resume).

![customization1]

### Input
- Hold and drag to create or destroy cells (Default: Enter).
- Clear the board completely (Default: Space Bar).
- Stop/Resume the creation of new generations (Default: Left Mouse Click).

![customization2]

## Preview
![preview]

[logo]: GameOfLife.png 
[customization1]: Images/Customization1.png
[customization2]: Images/Customization2.png
[preview]: Images/Preview.gif
