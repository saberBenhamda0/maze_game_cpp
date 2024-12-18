#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "../include/raylib.h"
#include <iterator>
#include "cell.h"
#include <stack>


using namespace std;

int WIDTH = 1280;
int HEIGHT = 631;

class Maze {
public:
    vector<Cell> grid;
    Cell* endCell; // Pointer to the end cell
    bool showSolution = false; // Flag to toggle solution display
    vector<Cell> solutionPath; // Vector to store the solution path

    int cols;
    int rows;
    int difficulte;
    int MAX_DEAD_ENDS = 0;
    Texture2D backgroundTexture; // Background image texture
    Texture2D traisor; // Background image texture
    Rectangle backgroundTextureRect;
    int largeurCellule;

    Maze(int difficulte) : difficulte(difficulte) {
        Cell tempCell(0, 0, difficulte);
        backgroundTexture = LoadTexture("maze.png");        // Load intro image
        traisor = LoadTexture("traiser.png");
        backgroundTextureRect = {
            (float)(GetScreenWidth() / 2 - traisor.width / 2),
            (float)(GetScreenHeight() ),
            (float)traisor.width,
            (float)traisor.height
        };
        int tileSize = tempCell.TILE;

        if (difficulte == 1) {
            MAX_DEAD_ENDS = 5;
            largeurCellule = 100;
        } else if (difficulte == 2) {
            MAX_DEAD_ENDS = 10;
            largeurCellule = 60;
        } else if (difficulte == 3) {
            MAX_DEAD_ENDS = 50;
            largeurCellule = 40;
        }

        cols = WIDTH / tileSize;
        rows = HEIGHT / tileSize;

        // Initialize grid with cells for the entire maze
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                grid.push_back(Cell(col, row, difficulte));
            }
        }
        endCell = nullptr; // Initialize the end cell as nullptr
    }

    void toggleSolution() {
        showSolution = !showSolution; // Toggle the solution visibility
    }

    Cell* checkCell(int x, int y) {
        if (x < 0 || x >= cols || y < 0 || y >= rows)
            return nullptr;
        return &grid[x + y * cols];
    }

    Cell* checkNeighbors(Cell* current) {
        vector<Cell*> neighbors;
        // here we get every cell in contact with the cell we are in
        Cell* top = checkCell(current->x, current->y - 1);
        Cell* right = checkCell(current->x + 1, current->y);
        Cell* bottom = checkCell(current->x, current->y + 1);
        Cell* left = checkCell(current->x - 1, current->y);

        if (top && !top->visited) neighbors.push_back(top);
        if (right && !right->visited) neighbors.push_back(right);
        if (bottom && !bottom->visited) neighbors.push_back(bottom);
        if (left && !left->visited) neighbors.push_back(left);

        if (!neighbors.empty()) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, neighbors.size() - 1);
            Cell* next = neighbors[dis(gen)];
            next->previous = current; // Set the previous pointer
            return next;
        }
        return nullptr;
    }

    void removeWalls(Cell* current, Cell* next) {
        int dx = current->x - next->x;
        if (dx == 1) {
            current->walls[3] = false; // left
            next->walls[1] = false;    // right
        } else if (dx == -1) {
            current->walls[1] = false; // right
            next->walls[3] = false;    // left
        }

        int dy = current->y - next->y;
        if (dy == 1) {
            current->walls[0] = false; // top
            next->walls[2] = false;    // bottom
        } else if (dy == -1) {
            current->walls[2] = false; // bottom
            next->walls[0] = false;    // top
        }
    }
vector<Cell> generateMaze() {
    Cell* current = &grid[0]; // start location
    vector<Cell*> stack; // stack of pointers to cells
    size_t visitedCount = 1; // visited count starts at 1 because the first cell is considered visited

    // Introduce dead ends in the maze
    int deadEndCount = 0;  // Keep track of how many dead ends have been created

    while (visitedCount != grid.size()) {
        current->visited = true;

        Cell* next = checkNeighbors(current);

        if (next) {
            next->visited = true;
            next->previous = current; // Set the current cell as the previous cell for the next one
            visitedCount++;
            stack.push_back(current);
            removeWalls(current, next);
            current = next;

            // Occasionally introduce dead ends
            if (deadEndCount < MAX_DEAD_ENDS) {
                // Check if current cell has no neighbors left after moving
                if (checkNeighbors(current) == nullptr) {
                    deadEndCount++;
                }
            }
        } else if (!stack.empty()) {
            current = stack.back();
            stack.pop_back();
        }
    }

    // Define the end cell (bottom-right corner of the maze)
    endCell = &grid[grid.size() - 1]; // Set the last cell as the end cell

    return grid; // return the generated maze grid
}

    

    void solveMaze() {
    // Ensure the solution path is clear before solving
    solutionPath.clear();

    // Start from the end cell and backtrack to the start cell
    Cell* current = endCell;
    while (current != nullptr) {
        solutionPath.push_back(*current);  // Add the current cell to the solution path
        current = current->previous;  // Move to the previous cell
    }

    // Reverse the path to start from the beginning (start cell will be first)
    reverse(solutionPath.begin(), solutionPath.end());
}

    void draw() const {
    // Draw the background
    DrawTexturePro(
        backgroundTexture,
        (Rectangle){ 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height },
        (Rectangle){ 0, 0, (float)WIDTH, (float)HEIGHT + 75},
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    // Draw each cell
    for (const Cell& cell : grid) {
        cell.draw();
    }

    // Highlight the end cell in red
    if (endCell) {
        float scale = (float)largeurCellule / traisor.width; // Calculate scale based on largeurCellule
        int xPos = endCell->x * endCell->TILE;
        int yPos = endCell->y * endCell->TILE;
        DrawTextureEx(traisor, (Vector2){ xPos, yPos }, 0.0f, scale, WHITE);
    }

    // If the solution flag is true, draw the solution path with smaller rectangles
    if (showSolution) {
        for (const Cell& cell : solutionPath) {
            int xPos = cell.x * cell.TILE;
            int yPos = cell.y * cell.TILE;

            // Draw a smaller rectangle for the solution path (adjust the size here)
            int smallerTile = cell.TILE - 10; // Half the size of the original tile
            DrawRectangle(xPos , yPos, smallerTile, smallerTile, (Color){ 0, 255, 0, 128 }); // Smaller solution path
        }
    }
}


};

#endif