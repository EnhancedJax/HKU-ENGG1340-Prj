#include <iostream>
#include <string>
#include "helpers.hpp"
#include "gameloop.hpp"
#include "pagedmenu.hpp"
#include "loader.hpp"
#include <chrono>

using namespace std;

int main()
{
    // Retrieve terminal dimensions
    int winCols = getWinCols(); // Retrieve console width
    int winRows = getWinRows(); // Retrieve console height

    // Validate terminal size
    if (winRows < 20 || winCols < 95) // Terminal size is insufficient
    {
        cout << "Terminal size too small. Please resize." << endl;
        cout << "Minimum size: 95x20" << endl;  // Display minimum terminal size
        cout << "Current size: " << winCols << "x" << winRows << endl; // Display current terminal size
        return 1;
    }

    // Prepare for game launch
    cursorHide();           // Make cursor invisible
    toggleEcho();           // Suppress input echo

    // Initialize game components
    Gameloop game;          // Create game loop
    State defaultGame;      // Create default game state
    Loader loader;          // Create game state loader
    int game_result = -1;   // Initialize game result
    
    // Main menu loop
    while (true)
    {
        PagedMenu menu;         // Create menu
        int response = menu.displayMenu(); // Display menu and capture user selection
        clearScreen();  // Clear screen for next display

        // Handle user selection
        if (response == 2) // User chose to quit
        {
            return 1; // Exit game
        }

        if (response == 1) // User chose to load game
        {
            State loadedState = loader.loadState(); // Load saved game state
            if (loadedState.Map2D == nullptr) // Invalid game state loaded
            {
                string m = "Loaded file is not a state. Try saving a game by hitting P in-game.";
                printAt(winCols / 2 - m.length() / 2, winRows / 2, m);
                getch();
                continue;
            }

            // Validate terminal size for loaded game
            if (loadedState.winCols == winCols && loadedState.winRows == winRows)
            {
                game_result = game.run(loadedState); // Run loaded game
            } 
            else // Terminal size mismatch
            {
                string m = "Loaded game's map size does not match terminal. Please resize terminal to " + to_string(loadedState.winCols) + "x" + to_string(loadedState.winRows);
                printAt(winCols / 2 - m.length() / 2, winRows / 2, m);
                getch();
            }
        }
        else // User chose to start new game
        {
            game_result = game.run(defaultGame); // Run default game
        }

        // Handle game result
        if (game_result == 1) // Game completed
        {
            menu.WinningScreen();   // Display victory screen
            getch();                // Wait for user input 

            // Return to main menu
            continue;
        }
    }
    cursorShow(); // Make cursor visible again

    return 0;
}
