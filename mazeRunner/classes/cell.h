#ifndef CELL_H
#define CELL_H
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "../include/raylib.h"
#include <iterator>

using namespace std;

class Cell {
public:
    int x, y; // x column position and y row position
    bool visited = false; // default value for if the cell is visited by the algorithm
    vector<bool> walls = { true, true, true, true }; // top, right, bottom, left
    int difficulte;
    int TILE = 0;
    int thickness = 4; // thickness of wall

    // New member to store the previous cell for pathfinding
    Cell* previous = nullptr; // Used for backtracking during pathfinding

    Cell(int x, int y, int difficulte) : x(x), y(y), difficulte(difficulte) {
        if(difficulte == 1) {
            TILE = 100;
        }
        else if(difficulte == 2) {
            TILE = 60;
        }
        else if(difficulte == 3) {
            TILE = 40;
        }
        else {
            difficulte = 1;
        }
    }

    // Draw method to display the cell and its walls
    void draw() const {
    int xPos = x * TILE; 
    int yPos = y * TILE; 
    Color wallColor = { 110, 99, 48, 255 };  // RGBA values

    // Drawing walls
    if (walls[0]) // top
        DrawLineEx({ (float)xPos, (float)yPos }, { (float)(xPos + TILE), (float)yPos }, thickness, wallColor);
    if (walls[1]) // right
        DrawLineEx({ (float)(xPos + TILE), (float)yPos }, { (float)(xPos + TILE), (float)(yPos + TILE) }, thickness, wallColor);
    if (walls[2]) // bottom
        DrawLineEx({ (float)(xPos + TILE), (float)(yPos + TILE) }, { (float)xPos, (float)(yPos + TILE) }, thickness, wallColor);
    if (walls[3]) // left
        DrawLineEx({ (float)xPos, (float)(yPos + TILE) }, { (float)xPos, (float)yPos }, thickness, wallColor);

    // Optionally, you can draw the previous cell path for the solution
   
}

};

#endif