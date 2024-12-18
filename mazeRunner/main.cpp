#include <iostream>
#include "raylib.h"
#include "classes/cell.h"
#include "classes/maze.h"
#include "classes/joueur.h"
#include "classes/GameController.h"

using namespace std;

int LARGEUR_CELLULE = 60;
int tailleX, tailleY;  // Dimensions of the maze

// Define button positions and sizes
Rectangle toggleSolutionButton = { WIDTH - 200, HEIGHT - 20, 200, 100 };  // Toggle solution button near bottom-right
Rectangle restartButton = { WIDTH - 200 - 160 - 10, HEIGHT - 20, 200, 100 };  // Restart button placed next to the first one

Texture2D restartTexture;
Texture2D toggleTexture;


void DrawButtonWithImage(Rectangle button, Texture2D texture) {
    // Draw the texture directly as the button (no need for background)
    DrawTexturePro(
        texture,
        (Rectangle){ 0, 0, (float)texture.width, (float)texture.height }, // Full texture
        button,  // Fit texture to button size
        (Vector2){ 0, 0 },  // No offset
        0.0f,  // No rotation
        WHITE  // Default color
    );
}


bool IsButtonPressed(Rectangle button) {
    // Check if the mouse is over the button and is clicked
    return CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

int main() {
    InitWindow(WIDTH, HEIGHT + 75, "The Maze Runner");
    SetTargetFPS(60);
    InitAudioDevice();
    Music intro = LoadMusicStream("intro.mp3");
    GameController GameController;

    int difficulte = GameController.afficherMenuDifficulte();  // Select game difficulty level

   restartTexture = LoadTexture("restart_button.png");  // Image for Restart button
    toggleTexture = LoadTexture("toggle_solution.png"); 
    Texture2D end_background = LoadTexture("end_background.png");
    Texture2D win_background = LoadTexture("win_background.png");
    Texture2D end_background1 = LoadTexture("end_background1.png");


    bool jeuTermine = false;     // Flag to indicate if the game is over
    bool gameCompleted = false;  // Flag to indicate if the player reached the exit
    bool scoreSaved = false;

    Maze maze(difficulte);        // Initialize the maze
    maze.generateMaze();

    int playerChoice = GameController.playerChoice;        // Generate maze based on difficulty

    Cell* startCell = &maze.grid[0];       // Start at the first cell
    Cell* exitCell = maze.endCell;   // Define the exit cell for the maze
    Joueur joueur1(startCell, difficulte, GameController.player1Name);  // Initialize player at start cell

    Joueur* joueur2 = nullptr;  // Declare joueur2 as nullptr initially, in case it's a single player game
    if (playerChoice == 2) {
        joueur2 = new Joueur(&maze.grid[(maze.rows - 1) * maze.cols], difficulte, GameController.player2Name);  // Start at the last row
    }

    // Solve the maze
    maze.solveMaze();

    while (!WindowShouldClose()) {
        GameController.miseAJour(jeuTermine);  // Update GameController
        UpdateMusicStream(intro);
    PlayMusicStream(intro);

        // Check if the game is over or completedf
        if (jeuTermine ) {
            if (!scoreSaved) {
                if (joueur1.aAtteintSortie(exitCell)) {
                    GameController.saveScore(GameController.player1Name, GameController.calculate_score(difficulte));
                    scoreSaved = true;  // Set flag to true to prevent further saves
                }
                else if (joueur2 && joueur2->aAtteintSortie(exitCell)) {
                    GameController.saveScore(GameController.player2Name, GameController.calculate_score(difficulte));
                    scoreSaved = true;  // Set flag to true to prevent further saves
                }
            }

            static bool leaderboardDisplayed = false; // Static to persist its value across frames
            BeginDrawing();
            DrawTexturePro(
                    end_background,
                    (Rectangle){ 0, 0, (float)end_background.width, (float)end_background.height },
                    (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
                    (Vector2){ 0, 0 },
                    0.0f,
                    WHITE
                );
            StopMusicStream(intro);

           /* while(!leaderboardDisplayed) {
                GameController.displayLeaderboard();
                if (IsKeyPressed(KEY_ENTER)){
                    leaderboardDisplayed = true; // Set to true after displaying the leaderboard
                }
            }*/

            DrawText("Game Over! Press 'R' to Restart", WIDTH / 2 - 350, HEIGHT / 2, 50, RED);
            EndDrawing();


            // Check for 'R' key to restart
            if (IsKeyPressed(KEY_R)){ /*-----------game complete------------ */
        // Reset the game
        GameController.reset();
        PlayMusicStream(intro);
        GameController.condition = true;
        difficulte = GameController.afficherMenuDifficulte();
        maze = Maze(difficulte);
        maze.generateMaze();
        startCell = &maze.grid[0];
        exitCell = maze.endCell;
        joueur1 = Joueur(startCell, difficulte, GameController.player1Name);

        if (playerChoice == 2) {
            delete joueur2;  // Clean up previous joueur2 if it exists
            joueur2 = new Joueur(&maze.grid[(maze.rows - 1) * maze.cols], difficulte, GameController.player2Name);
        } else {
            joueur2 = nullptr;  // Set to nullptr if no second player
        }
        jeuTermine = false;
        gameCompleted = false;
        leaderboardDisplayed = false;
    }


            continue;
        }

       if (gameCompleted) {
    if (!scoreSaved) {
        // Save scores when a player completes the game
        if (joueur1.aAtteintSortie(exitCell)) {
            GameController.saveScore(GameController.player1Name, GameController.calculate_score(difficulte));
            scoreSaved = true;
        }
        else if (joueur2 && joueur2->aAtteintSortie(exitCell)) {
            GameController.saveScore(GameController.player2Name, GameController.calculate_score(difficulte));
            scoreSaved = true;
        }
    }
static bool restart_frame = true;
    static bool leaderboardDisplayed = false; // Keeps track if the leaderboard is displayed

    BeginDrawing();

    // Step 1: Show the win background if the leaderboard has not been displayed
if (!leaderboardDisplayed) {
    // Check for key press to transition to leaderboard
    if (IsKeyPressed(KEY_ENTER)) {
        leaderboardDisplayed = true; // Mark that leaderboard is now displayed
    } else {
        // Draw the win background
        DrawTexturePro(
            win_background,
            (Rectangle){ 0, 0, (float)win_background.width, (float)win_background.height },
            (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );

        // Display victory message

        if (joueur1.aAtteintSortie(exitCell)) {
            char message[150];  // Buffer to hold the full message
            snprintf(message, sizeof(message), "Congratulations! %s", GameController.player1Name);  // Safely format message
            DrawText(message, WIDTH / 2 - MeasureText(message, 50) / 2, HEIGHT / 2 - 50, 50, GREEN);
        } 
        else if (joueur2 && joueur2->aAtteintSortie(exitCell)) {
            char message[150];  // Buffer for the full message
            snprintf(message, sizeof(message), "Congratulations! %s", GameController.player2Name);  // Safely format message
            DrawText(message, WIDTH / 2 - MeasureText(message, 50) / 2, HEIGHT / 2 - 50, 50, GREEN);
        }

        DrawText("Press 'ENTER' to View Leaderboard", WIDTH / 2 - MeasureText("Press 'ENTER' to View Leaderboard", 30) / 2, HEIGHT / 2 + 10, 30, GREEN);

        
    }
} while(leaderboardDisplayed && restart_frame) {
    // Display the leaderboard
    GameController.displayLeaderboard();
    if(IsKeyPressed(KEY_ENTER)){
        restart_frame = false;
    }
}

if(!restart_frame){

            StopMusicStream(intro);
   
            DrawTexturePro(
                    end_background1,
                    (Rectangle){ 0, 0, (float)end_background1.width, (float)end_background1.height },
                    (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
                    (Vector2){ 0, 0 },
                    0.0f,
                    WHITE
                );
            DrawText("Press 'R' to Restart", WIDTH / 2 - 350, HEIGHT / 2, 50, GREEN);

}




    EndDrawing();

    // Wait for the player to press 'ENTER' to transition to the leaderboard

    // Check for 'R' key to restart the game
    if (IsKeyPressed(KEY_R)) { /*-----------game complete------------ */
        // Reset the game
        GameController.reset();
        PlayMusicStream(intro);
        GameController.condition = true;
        difficulte = GameController.afficherMenuDifficulte();
        maze = Maze(difficulte);
        maze.generateMaze();
        startCell = &maze.grid[0];
        exitCell = maze.endCell;
        joueur1 = Joueur(startCell, difficulte, GameController.player1Name);

        if (playerChoice == 2) {
            delete joueur2;  // Clean up previous joueur2 if it exists
            joueur2 = new Joueur(&maze.grid[(maze.rows - 1) * maze.cols], difficulte, GameController.player2Name);
        } else {
            joueur2 = nullptr;  // Set to nullptr if no second player
        }
        jeuTermine = false;
        gameCompleted = false;
        leaderboardDisplayed = false;
        restart_frame = true;  // Reset leaderboard display state for next game
    }
    continue;
}


        // Check for button click to toggle solution
        if (IsButtonPressed(toggleSolutionButton)) {
            maze.toggleSolution();
        }

        // Handle player movement for player 1
        if (IsKeyPressed(KEY_RIGHT)) joueur1.deplacer('R', maze);
        if (IsKeyPressed(KEY_LEFT)) joueur1.deplacer('L', maze);
        if (IsKeyPressed(KEY_DOWN)) joueur1.deplacer('D', maze);
        if (IsKeyPressed(KEY_UP)) joueur1.deplacer('U', maze);

        // Handle player 2 movement if two players
        if (playerChoice == 2 && joueur2) {
            if (IsKeyPressed(KEY_D)) joueur2->deplacer('R', maze); // Right
            if (IsKeyPressed(KEY_A)) joueur2->deplacer('L', maze); // Left
            if (IsKeyPressed(KEY_S)) joueur2->deplacer('D', maze); // Down
            if (IsKeyPressed(KEY_W)) joueur2->deplacer('U', maze); // Up
        }

        // Check if player reached the exit
        if (joueur1.aAtteintSortie(exitCell) || (joueur2 && joueur2->aAtteintSortie(exitCell))) {
            gameCompleted = true;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw maze, player, and countdown timer
        GameController.afficherCompteARebours(10, 10, 20, BLACK);  // Show the countdown timer

        maze.draw();
        joueur1.dessiner();
        if (playerChoice == 2 && joueur2) {
            joueur2->dessiner();
        }
         if(difficulte == 1 ||difficulte == 2 || difficulte == 3 ){
       GameController.afficherCompteARebours(10, 10, 20, BLACK);

        }
        
   /* --------------------------------------------(---------------------------------------------------------------------------------------------------- hna fen button conditio dyala*/     
    if (IsButtonPressed(restartButton)) {

    // Reset GameController timer
   GameController.afficherCompteARebours(10, 10, 20, BLACK);  // Show the countdown timer
    GameController.reset();

    // Reset other game components
    maze = Maze(difficulte);  // Reinitialize maze with current difficulty
    maze.generateMaze();  // Regenerate the maze
    startCell = &maze.grid[0];
    exitCell = maze.endCell;

    // Reinitialize the player(s)
    joueur1 = Joueur(startCell, difficulte, GameController.player1Name);
    GameController.demarrer(difficulte);  // Start the GameController

    // Update GameController to reflect the reset state
    GameController.condition = true;
    GameController.miseAJour(jeuTermine);

    maze.draw();  // Redraw the maze

    maze.solveMaze();  // Optionally solve the maze

    // Reset player 2 if applicable
    if (playerChoice == 2) {
        delete joueur2;  // Clean up previous player 2 if it exists
        joueur2 = new Joueur(&maze.grid[(maze.rows - 1) * maze.cols], difficulte, GameController.player2Name);
    } else {
        joueur2 = nullptr;  // Set player 2 to nullptr if not needed
    }

    // Reset game state flags
    jeuTermine = false;
    gameCompleted = false;
}

       DrawButtonWithImage(toggleSolutionButton, toggleTexture);  // Show Toggle Solution button with an image
       DrawButtonWithImage(restartButton, restartTexture);        // Show Restart button with an image

        EndDrawing();
    }

    // Clean up dynamically allocated joueur2
    if (joueur2) {
        delete joueur2;
    }

   UnloadTexture(restartTexture);
   UnloadTexture(toggleTexture);
    
    CloseWindow();
    return 0 ;
}
