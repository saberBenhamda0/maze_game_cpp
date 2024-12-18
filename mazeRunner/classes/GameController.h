#ifndef GameController_H
#define GameController_H

#include "raylib.h"
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <algorithm> // For std::random_shuffle
#include <fstream>   // For file operations
#include <sstream>   // For string stream
#include <string>

class GameController {
private:
    // Timer properties
    float tempsInitial;
    float tempsRestant;
    float delay_time;
    bool enCours;

    // Game state flags
    bool jeuCommence;
    bool introShown;
    bool isLeaderboardVisible;
    bool diffucilty_chosen;



    // Intro splash screen and button image
    Texture2D introImage;
    Texture2D difficultyBackgroundImage;
    Texture2D startButtonImage; // Button image
    Texture2D leaderboardButtonImage; // Button image
    Texture2D settingsButtonImage; // Button image
    Texture2D leaderboard_background;
    Texture2D player_background;
    Texture2D sand_clock;

    // Button position and size
    Rectangle startButtonRect;
    Rectangle leaderboardButtonRect;
    Rectangle settingsButtonRect;
    Rectangle onePlayerButton = { 180, 400, 400, 60 };  // Button for one player
    Rectangle twoPlayerButton = { 180, 480, 400, 60 };  // Button for two players


    Font font = LoadFont("dungeon.ttf");

    // File path for leaderboard
    const std::string leaderboardFilePath = "score.txt";

    // A structure to hold score information
    struct Score {
        std::string playerName;
        float score;
    };

public:
    int playerChoice = 0;  // Initialize with 0 (no choice)
    char player2Name[100] = { 0 };  // Store Player 2's name (max 100 characters)
    char player1Name[100] = { 0 };  // Store Player 1's name (max 100 characters)
    bool condition = true;
    Music intro2 = LoadMusicStream("intro2.mp3");

    GameController()
        : tempsInitial(60.0f), tempsRestant(60.0f), enCours(false), jeuCommence(false), introShown(false) {
        introImage = LoadTexture("maze_runner.png");        // Load intro image
        leaderboard_background = LoadTexture("leaderboard_background.png");        // Load leaderboard_background image
        difficultyBackgroundImage = LoadTexture("menu_background.png");
        startButtonImage = LoadTexture("start.png"); // Load button image
        leaderboardButtonImage = LoadTexture("leaderboard_button.png");
        settingsButtonImage = LoadTexture("settings.png");
        player_background = LoadTexture("player_background.png");
        sand_clock = LoadTexture("sand_clock.png");

        // Define button position and size
         startButtonRect = {
    (float)(GetScreenWidth() - (startButtonImage.width * 0.5f) - 200), // Keep it to the right with a 50-pixel margin
    (float)(GetScreenHeight() * 0.7f),                               // Move it down to 10% of the screen height
    (float)(startButtonImage.width ),                          // Scale width down by 50%
    (float)(startButtonImage.height )                          // Scale height down by 50%
};

leaderboardButtonRect = {
    (float)(0 + (startButtonImage.width * 0.5f) + 100),  // Move it to the left, 1/4 of screen width
    (float)(GetScreenHeight() * 0.7f),                               // Vertical position
    (float)leaderboardButtonImage.width,                             // Button width
    (float)leaderboardButtonImage.height                             // Button height
};

        settingsButtonRect = {
            (float)(3 * GetScreenWidth() / 4 - settingsButtonImage.width / 2),
            (float)(GetScreenHeight() - 250),
            (float)settingsButtonImage.width,
            (float)settingsButtonImage.height
        };
    }

    ~GameController() {
        UnloadTexture(introImage);
        UnloadTexture(startButtonImage);
        UnloadTexture(leaderboardButtonImage);
        UnloadTexture(settingsButtonImage);
        UnloadTexture(leaderboard_background);
        UnloadTexture(player_background);
        UnloadTexture(sand_clock);
    }

        void displayLeaderboard() {

        std::vector<Score> leaderboard = loadLeaderboardScores();
        std::sort(leaderboard.begin(), leaderboard.end(), [](const Score& a, const Score& b) {
            return a.score > b.score;
        });
        // Clear only if the background or screen content needs to be updated
        BeginDrawing();
        
        // Draw the background once (if static, can be done once outside the loop)
        // Display the leaderboard text
        DrawTexturePro(
            leaderboard_background,
            (Rectangle){ 0, 0, (float)leaderboard_background.width, (float)leaderboard_background.height },
            (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );
        // Draw the leaderboard title at the top-center
        const char* title = "LEADERBOARD";
        Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, 50, 2);
        float titleX = (GetScreenWidth() - titleSize.x) / 2;
        float titleY = 120;

        DrawText(title, titleX, titleY, 50, DARKGREEN); // Base text
        DrawText(title, titleX - 2, titleY - 2, 50, WHITE); // Outline effect

        // Draw each leaderboard entry below the title
        int yOffset = HEIGHT / 2 - 120 ; // Start drawing entries below the title

        int leaderboard_size = leaderboard.size() > 7 ? 7 : leaderboard.size() ; 
        for (size_t i = 0; i <  leaderboard_size; ++i) {
            // Format the text for the leaderboard entry
            char buffer[100];
            sprintf(buffer, "%d. %s - %.2f", i + 1, leaderboard[i].playerName.c_str(), leaderboard[i].score);

            float xOffset = WIDTH / 2 - 200; // Horizontal offset for alignment

            // Shadow effect for text
            DrawText(buffer, xOffset + 2, yOffset + 2, 30, DARKGRAY);

            // Main text
            DrawText(buffer, xOffset, yOffset, 30, (Color){ 50, 205, 50, 255 } /*light green color */);

            yOffset += 40; // Increment yOffset for spacing between entries
        }

        EndDrawing();
    }

        void afficherSplashScreen() {
            while (!WindowShouldClose() && !introShown) {
            UpdateMusicStream(intro2);
                BeginDrawing();
                ClearBackground(RAYWHITE);
                PlayMusicStream(intro2);

                // Draw the intro image centered on the screen
                DrawTexturePro(
                    introImage,
                    (Rectangle){ 0, 0, (float)introImage.width, (float)introImage.height },
                    (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
                    (Vector2){ 0, 0 },
                    0.0f,
                    WHITE
                );

                // Check if mouse is hovering over the Start button
                if (CheckCollisionPointRec(GetMousePosition(), startButtonRect)) {
                    // Draw the Start button image with a hover effect
                    DrawTextureEx(startButtonImage, (Vector2){ startButtonRect.x, startButtonRect.y }, 0.0f, 1.0f, LIGHTGRAY);
                } else {
                    // Draw the default Start button image
                    DrawTextureEx(startButtonImage, (Vector2){ startButtonRect.x, startButtonRect.y }, 0.0f, 1.0f, WHITE);
                }

                

                // Check if the user clicked on the Start button
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), startButtonRect)) {
                    introShown = true; // Continue to the next screen
                }

                // Check if mouse is hovering over the Leaderboard button
                if (CheckCollisionPointRec(GetMousePosition(), leaderboardButtonRect)) {
                    // Draw the Leaderboard button image with a hover effect
                    DrawTextureEx(leaderboardButtonImage, (Vector2){ leaderboardButtonRect.x, leaderboardButtonRect.y }, 0.0f, 1.0f, LIGHTGRAY);
                } else {
                    // Draw the default Leaderboard button image
                    DrawTextureEx(leaderboardButtonImage, (Vector2){ leaderboardButtonRect.x, leaderboardButtonRect.y }, 0.0f, 1.0f, WHITE);
                }

                

                // Check if the user clicked on the Leaderboard button
                while(CheckCollisionPointRec(GetMousePosition(), leaderboardButtonRect)) {
                    displayLeaderboard();  // Set a flag to show the leaderboard
                    // Allow user to return or continue (could add a return button to the leaderboard screen)
                }


                EndDrawing();
            }
        }

        string takeinput(char playerName[], int& playerNameLength, int maxLength, bool& isEnterKeyPressed, bool& isInputActive) {
            // If the input is no longer active, stop processing input
            if (!isInputActive) return string(playerName);

            // Process each key event once
            if (IsKeyPressed(KEY_BACKSPACE) && playerNameLength > 0) {
                playerNameLength--;
                playerName[playerNameLength] = '\0'; // Remove the last character
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (isEnterKeyPressed) {
                    // Second Enter press: stop input process
                    isInputActive = false;
                } else {
                    isEnterKeyPressed = true;
                    playerName[playerNameLength] = '\0'; // Null-terminate the string when Enter is pressed
                    return string(playerName); // Return the entered name
                }
            }

            // Process printable characters
            for (int i = 32; i <= 126; i++) {
                if (IsKeyPressed(i) && playerNameLength < maxLength - 1) {
                    playerName[playerNameLength] = (char)i;
                    playerNameLength++;
                }
            }

            // Return the current entered name as a string
            return string(playerName);
        }


void DrawButton(Rectangle button, const char* label) {
    // Draw button background
    DrawRectangleRec(button, GRAY); // Button background
    DrawRectangleLinesEx(button, 3, DARKGRAY); // Border

    // Draw button text (centered)
    Vector2 textSize = MeasureTextEx(GetFontDefault(), label, 20, 1.0f);
    DrawText(label, button.x + (button.width - textSize.x) / 2, button.y + (button.height - textSize.y) / 2, 20, DARKGREEN);
}

bool IsButtonPressed(Rectangle button) {
    // Check if the mouse is over the button and is clicked
    return CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}


        void afficherUserInfoScreen() {
            // Set up the input variables
            int player1NameLength = 0;  // Length of the entered name
            int player2NameLength = 0;  // Length of the entered name
            int maxNameLength = 100;  // Maximum name length
            bool keyBackspace = false;  // Flag to check if backspace is pressed
            bool isEnterKeyPressed = false;  // Flag to track Enter key press
            bool isInputActive = true;  // Flag to track if input is still active

            // Main game loop
            while (!WindowShouldClose() && isInputActive) {
                BeginDrawing();
                DrawTexturePro(
                    player_background,
                    (Rectangle){ 0, 0, (float)player_background.width, (float)player_background.height },
                    (Rectangle){ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
                    (Vector2){ 0, 0 },
                    0.0f,
                    WHITE
                );
                


                // Display the "Choose Player Mode" prompt
                        int textSize = 50;
                        Color textColor = BLACK ; // Or a slightly warmer white
                        Color shadowColor = WHITE;
                        Color buttonColor = Fade(shadowColor, 0.3f);

                        // Shadow offset and blur parameters
                        int shadowOffset = 2;
                        float shadowBlur = 2.0f;
                        int spacing = 10; // Adjust the spacing value as needed
                if (playerChoice == 0) {
                        // Text settings

                        // Enable anti-aliasing
                        SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_BILINEAR);

                        // Draw shadow for the first text
                        DrawTextEx(font, "Do you want one player or two?", Vector2{(float)182 + shadowOffset, (float)252 + shadowOffset}, textSize, spacing, shadowColor);

                        // Draw a small rectangle behind the first text for better visibility
                        DrawRectangle(180, 250, MeasureTextEx(font, "Do you want one player or two?", textSize, spacing).x, textSize, Fade(shadowColor, 0.3f));  // Adjust the transparency for the rectangle if needed

                        // Draw first text
                        DrawTextEx(font, "Do you want one player or two?", Vector2{180, 250}, textSize, spacing, textColor);

                        // Draw shadow for the second text

                        // Draw a small rectangle behind the second text for better visibility

                        // Draw second text
                       


        DrawRectangleRec(onePlayerButton, buttonColor);  // Button background color set to green
        // Draw the button text in green
       DrawTextEx(
    font,  // Your custom font
    "One Player",  // The text to draw
    Vector2{ onePlayerButton.x + 100, onePlayerButton.y + 20 },  // Position
    50,  // Font size
    1.0f,  // Spacing between characters
    textColor  // Color of the text
);

        DrawRectangleRec(twoPlayerButton, buttonColor);  // Button background color set to green
        // Draw the button text in green
      DrawTextEx(
    font,  // Your custom font
    "Two Players",  // The text to draw
    Vector2{ twoPlayerButton.x + 100, twoPlayerButton.y + 20 },  // Position
    50,  // Font size
    1.0f,  // Spacing between characters
    textColor  // Color of the text
);

           




                    // Handle player choice input
                    if (IsButtonPressed(onePlayerButton)) {
                playerChoice = 1;  // One player chosen
            }
            if (IsButtonPressed(twoPlayerButton)) {
                playerChoice = 2;  // Two players chosen
            }
                }

                // Player 1 input
                if (playerChoice == 1) {
                 DrawRectangle(180, 250, MeasureTextEx(font, "Enter Player 1's name:", textSize, spacing).x, textSize, Fade(shadowColor, 0.3f));

                DrawTextEx(font, "Enter Player 1's name:", Vector2{(float)202 + shadowOffset, (float)252 + shadowOffset}, textSize, spacing, shadowColor);
                   
                    DrawTextEx(font, "Enter Player 1's name:", Vector2{200, 250}, textSize, spacing, textColor);

                    // Call takeinput to get Player 1's name
                    string name1 = takeinput(player1Name, player1NameLength, maxNameLength, isEnterKeyPressed, isInputActive);

                    // Display Player 1's entered name
                    DrawTextEx(font, player1Name, Vector2{(float)202 + shadowOffset, (float)302 + shadowOffset}, textSize, spacing, shadowColor);
                    DrawRectangle(200, 300, MeasureTextEx(font,player1Name , textSize, spacing).x, textSize, Fade(shadowColor, 0.3f));

                    DrawTextEx(font, player1Name, Vector2{200, 300}, textSize, spacing, textColor);

                }
                // Player 2 input (if two players)
                else if (playerChoice == 2) {
                    // Player 1 input
                    if (!isEnterKeyPressed) {

                   DrawRectangle(180, 250, MeasureTextEx(font, "Enter Player 1's name:", textSize, spacing).x, textSize, Fade(shadowColor, 0.3f));

                DrawTextEx(font, "Enter Player 1's name:", Vector2{(float)202 + shadowOffset, (float)252 + shadowOffset}, textSize, spacing, shadowColor);
                   
                    DrawTextEx(font, "Enter Player 1's name:", Vector2{200, 250}, textSize, spacing, textColor);




                       

                        string name1 = takeinput(player1Name, player1NameLength, maxNameLength, isEnterKeyPressed, isInputActive);

                         DrawTextEx(font, player1Name, Vector2{(float)202 + shadowOffset, (float)302 + shadowOffset}, textSize, spacing, shadowColor);
                    DrawRectangle(200, 300, MeasureTextEx(font,player1Name , textSize, spacing).x, textSize, Fade(shadowColor, 0.3f));

                    DrawTextEx(font, player1Name, Vector2{200, 300}, textSize, spacing, textColor);
                    }

                    // Player 2 input
                    else if (isEnterKeyPressed && isInputActive) {
                    DrawRectangle(180, 250, MeasureTextEx(font, "Enter Player 2's name:", textSize, spacing).x, textSize, Fade(shadowColor, 0.3f));

                        DrawTextEx(font, "Enter Player 2's name:", Vector2{(float)202 + shadowOffset, (float)252 + shadowOffset}, textSize, spacing, shadowColor);
                        DrawTextEx(font, "Enter Player 2's name:", Vector2{200, 250}, textSize, spacing, textColor);
                        string name2 = takeinput(player2Name, player2NameLength, maxNameLength, isEnterKeyPressed, isInputActive);
                         DrawTextEx(font, player2Name, Vector2{(float)202 + shadowOffset, (float)352 + shadowOffset}, textSize, spacing, shadowColor);
                                             DrawRectangle(200, 350, MeasureTextEx(font,player2Name , textSize, spacing).x, textSize, Fade(shadowColor, 0.3f));

                        DrawTextEx(font, player2Name, Vector2{200, 350}, textSize, spacing, textColor);
                    }
                }

                EndDrawing();
            }
        }

        int afficherMenuDifficulte() {
            afficherSplashScreen(); // Show the splash screen first;
            afficherUserInfoScreen(); // show the screen where user chooses how many players he wants

            int choixDifficulte = 0;
            while (!WindowShouldClose() && !jeuCommence) {
                BeginDrawing();
                ClearBackground(RAYWHITE);

                // Draw the background image for the difficulty screen
                DrawTexturePro(
                    difficultyBackgroundImage,
                    (Rectangle){0, 0, (float)difficultyBackgroundImage.width, (float)difficultyBackgroundImage.height},
                    (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                    (Vector2){0, 0}, 
                    0.0f, 
                    WHITE
                );

                int textSize = 60;
                Color textColor = WHITE;
                Color shadowColor = DARKGRAY;  // Shadow color
                int shadowOffset = 3; // Shadow offset for better visibility
                float spacing = 2.0f;  // Set appropriate spacing between text characters

                float centerX = WIDTH / 2 - 180; // X position for centering the options

               DrawTextEx(font, "Select Difficulty:", Vector2{centerX + 25 + shadowOffset, (float)300 + shadowOffset}, 60, spacing, shadowColor);
DrawTextEx(font, "Select Difficulty:", Vector2{centerX + 25, 300}, textSize, spacing, textColor);

// Difficulty Option 1 - Easy
DrawTextEx(font, "1 - Easy (30s)", Vector2{centerX + shadowOffset, (float)HEIGHT / 2 + 150 + shadowOffset}, 60, spacing, shadowColor);
DrawTextEx(font, "1 - Easy (30s)", Vector2{centerX, (float)HEIGHT / 2 + 150}, textSize, spacing, textColor);

// Difficulty Option 2 - Medium
DrawTextEx(font, "2 - Medium (70s)", Vector2{centerX + shadowOffset, (float)HEIGHT / 2 + 100 + shadowOffset}, 60, spacing, shadowColor);
DrawTextEx(font, "2 - Medium (70s)", Vector2{centerX, (float)HEIGHT / 2 + 100}, textSize, spacing, textColor);

// Difficulty Option 3 - Hard
DrawTextEx(font, "3 - Hard (150s)", Vector2{centerX + shadowOffset, (float)HEIGHT / 2 + 50 + shadowOffset}, 60, spacing, shadowColor);
DrawTextEx(font, "3 - Hard (150s)", Vector2{centerX, (float)HEIGHT / 2 + 50}, textSize, spacing, textColor);

                // Handle difficulty selection via keypress
                if (IsKeyPressed(KEY_ONE)) {
                    choixDifficulte = 1;
                    demarrer(choixDifficulte);
                } else if (IsKeyPressed(KEY_TWO)) {
                    choixDifficulte = 2;
                    demarrer(choixDifficulte);
                } else if (IsKeyPressed(KEY_THREE)) {
                    choixDifficulte = 3;
                    demarrer(choixDifficulte);
                }

                EndDrawing();
            }

            return choixDifficulte;
        }


        void demarrer(int difficulte) {
            switch (difficulte) {
                case 1: tempsInitial = 30.0f; break;
                case 2: tempsInitial = 70.0f; break;  // Medium
                case 3: tempsInitial = 150.0f; break;  // Hard
                default: tempsInitial = 45.0f; break; // Default to easy
            }


            enCours = true;
            jeuCommence = true;
        }

        void arreter() {
            enCours = false;
        }

        void miseAJour(bool &jeuTermine) {
            if (enCours) {

                if(condition){
                    delay_time = GetTime();
                    condition=false;
                }
                float tempsEcoule = GetTime();
                tempsRestant = tempsInitial - tempsEcoule + delay_time ;

                if (tempsRestant <= 0) {
                    tempsRestant = 0;
                    arreter();
                    jeuTermine = true;
                }
            }

        }

       void afficherCompteARebours(float x, float y, int tailleTexte, Color couleur) const {
    if (enCours) {
        // Adjust y to position the countdown just above the buttons
        float adjustedY = HEIGHT - 20  ;  // Adjusted height, just above the buttons
        
        // Draw the sand clock image at the far left
        DrawTextureEx(sand_clock, 
            (Vector2){ 0, adjustedY },  // Set x to 0 to position it at the far left
            0.0f, 
            0.15f, 
            WHITE);

        // Draw the countdown timer text at the far left with a little padding
        DrawText(TextFormat("Time Left: %d s", (int)tempsRestant), 70, adjustedY + 15, tailleTexte,WHITE );  // Add padding to the text position
    }
}

        void reset() {
            tempsRestant = tempsInitial;
            enCours = false;
            jeuCommence = false;
            introShown = false;
        }

            // Function to load leaderboard scores from a file
        std::vector<Score> loadLeaderboardScores() {
            std::vector<Score> leaderboard;
            std::ifstream file(leaderboardFilePath);
            std::string line;

            while (std::getline(file, line)) {
                std::istringstream ss(line);
                Score s;
                ss >> s.playerName >> s.score;
                leaderboard.push_back(s);
            }

            return leaderboard;
        }

        // Function to save a score to the leaderboard file
        void saveScore(const std::string& playerName, float score) {
            std::ofstream file(leaderboardFilePath, std::ios::app);  // Append to file
            if (file.is_open()) {
            file << playerName << ": " << static_cast<int>(score) << "\n";  // Cast score to int if needed
            }
        }

        // Function to display leaderboard

        int calculate_score(int difficulte){
            return tempsRestant * difficulte;
        }
};

#endif // GameController_H