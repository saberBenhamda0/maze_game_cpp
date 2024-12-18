#ifndef JOUEUR_H
#define JOUEUR_H

#include "maze.h" // Assuming you have the Maze and Cell classes in maze.h

class Joueur {
private:
    Cell* currentCell;  // Pointer to the current cell the player is in
    int difficulte;  // Size of each cell for drawing purposes
    int mouvements;      // Number of total moves made by the player
    int largeurCellule;
    Texture2D traisor; // Background image texture
    Rectangle backgroundTextureRect;
    string player_name;
public:
    // Constructor initializes player at a starting cell in the maze
    Joueur(Cell* startCell, int difficulte, string player_name) : currentCell(startCell), difficulte(difficulte), player_name(player_name) {
        if(difficulte == 1){
            largeurCellule = 100;
        }
        if(difficulte == 2){
            largeurCellule = 60;
        }   
        if(difficulte == 3){
            largeurCellule = 40;
        }   
        
        traisor = LoadTexture("ship.png");
        backgroundTextureRect = {
            (float)(GetScreenWidth() / 2 - traisor.width / 2),
            (float)(GetScreenHeight() - 100),
            (float)largeurCellule - 10,
            (float)largeurCellule - 10
        };
    }

    // Player movement method, considering maze walls
    void deplacer(char direction, Maze& labyrinthe) {
        Cell* nextCell = nullptr;

        // Check direction and corresponding wall status
        switch (direction) {
        case 'R': // Move right
            if (!currentCell->walls[1]) { // Check right wall
                nextCell = labyrinthe.checkCell(currentCell->x + 1, currentCell->y);
            }
            break;
        case 'L': // Move left
            if (!currentCell->walls[3]) { // Check left wall
                nextCell = labyrinthe.checkCell(currentCell->x - 1, currentCell->y);
            }
            break;
        case 'D': // Move down
            if (!currentCell->walls[2]) { // Check bottom wall
                nextCell = labyrinthe.checkCell(currentCell->x, currentCell->y + 1);
            }
            break;
        case 'U': // Move up
            if (!currentCell->walls[0]) { // Check top wall
                nextCell = labyrinthe.checkCell(currentCell->x, currentCell->y - 1);
            }
            break;
        }

        // If movement is valid (next cell exists and is within bounds)
        if (nextCell) {
            currentCell = nextCell;
            mouvements++;
        }
    }

    // Draws the player at the current cell
    void dessiner() const {
        //DrawRectangle(currentCell->x * largeurCellule, currentCell->y * largeurCellule, largeurCellule, largeurCellule, GREEN);
        float scale = (float)largeurCellule / traisor.width; // Calculate scale based on largeurCellule
        DrawTextureEx(traisor, 
                    (Vector2){ currentCell->x * largeurCellule, currentCell->y * largeurCellule }, 
                    0.0f, 
                    scale, 
                    WHITE);
                  }

    // Check if the player has reached the exit
    bool aAtteintSortie(const Cell* sortieCell) const {
        return (currentCell == sortieCell);
    }

    // Getters for player's position (returns cell coordinates)
    int getX() const { return currentCell->x; }
    int getY() const { return currentCell->y; }
    int getMouvements() const { return mouvements; }

    // Optionally, you could add a setter to modify the current cell if needed
    void setCurrentCell(Cell* newCell) {
        currentCell = newCell;
    }

    Cell* getCurrentCell() const {
    return currentCell;  // Assuming currentCell is a member variable storing the player's current position
}
};

#endif
