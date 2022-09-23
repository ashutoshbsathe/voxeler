'''
title: Assignment 1 - Voxelized Modeling
authors: Ashutosh B Sathe, Animesh
'''

# Explaination of features implemented

## Drawing the grid

* Usually for drawing a `NxNxN` grid one may need to define `2*(N^N^N)` vertices in the grid_vbo (for storing both `x&y` coords.), but here in this implementation we have tried to reduce the memory overhead from the graphics memory by using the `glDrawArraysInstanced()` function to draw the grid lines instead of using `glDrawArrays()`. This approach helped us to hugely reduce the memory overhead from `O(N^N^N)` to `O(N)`.


* This function helped us to reuse the limited set of vertices (`8*N` to be exact, 2 times for storing both ends of the vertices in both x and y axis) while drawing the grid-lines. We are storing only the end points of a grid-line in each axis. To ensure the robustness of the application, i.e. to handle varying grid sizes (`N`) and ensuring the cell size of `5x5x5, we have defined `N_UNITS` as 5 whereas `DRAW_MIN` and `DRAW_MAX` which defines the bounds of the grid depends on `N_CELLS (N)` and `N_UNITS` such that it can accomodate a varying number of cells of size `5x5x5` each.


**NOTE**: User can vary the value of `N_CELLS` in `main.hpp` to change the grid size, be default it is set to `100` which means a grid of `100x100x100` cells.


* We have created a separate array called **`grid_offsets`** which holds the offset distances between successive grid-lines which is then used by the grid vertex shader while drawing the grid-lines.


* Grid offsets are stored in a way such that the ith offset defines the distance between the i-1th and ith grid line. i.e:

```
	grid_offsets[i] = DRAW_MIN + (i-1) * (DRAW_MAX - DRAW_MIN)/ N_CELLS
```
* While drawing the grid-lines, we first started with the horizontal lines in `x-z` plane, later we are rotating the lines by `90 degrees` to draw vertical lines in the `x-z` plane. This procedure draws a 2D grid in `x-z` plane, which is finally repeated in other `y` axis values to fill the whole 3D volume.

* Plots representing drawing of the grid lines:


## Drawing cursor cube
