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
    // check terminal size
    int winCols = getWinCols();
    int winRows = getWinRows();
    if (winRows < 20 || winCols < 70)
    {
        cout << "Terminal size too small. Please resize to at least WxH: 70x20." << endl;
        return 1;
    }

    cursorHide();

    toggleEcho();

    Gameloop game;
    State defaultGame;
    Loader loader;
    int game_result = -1;
    while (true)
    {
        PagedMenu menu;                    //Linkage with pagedmenu setting requried
        int response = menu.displayMenu();
        clearScreen();
        if (response == 2)               // System mechanism for handling user's input
        {
            return 1; // Quit the game
        }
        if (response == 1)
        {
            State loadedState = loader.loadState();
            if (loadedState.Map2D == nullptr)
            {
                string m = "Loaded file is not a state. Try saving a game by hitting P in-game.";
                printAt(winCols/2 - m.length()/2, winRows/2, m);
                getch();
                continue;
            }
            if (loadedState.winCols == winCols && loadedState.winRows == winRows)
            {
                game_result = game.run(loadedState);
            } else {
                string m = "Loadde game's map size does not match terminal. Please resize terminal to " + to_string(loadedState.winCols) + "x" + to_string(loadedState.winRows);
                printAt(winCols/2 - m.length()/2, winRows/2, m);
                getch();
            }
        }
        else 
        {
            game_result = game.run(defaultGame);
        }

        //Handling different game results
        if (game_result == 0)
        {
            // Handle lose state
            // Print lose statement in the middle of the screen with ASCII art

            string loseStatement = R"(
             __      __  ______   __    __  __  _______   ________        __         ______    ______   ________  __ 
            /  \    /  |/      \ /  |  /  |/  |/       \ /        |      /  |       /      \  /      \ /        |/  |
            $$  \  /$$//$$$$$$  |$$ |  $$ |$$/ $$$$$$$  |$$$$$$$$/       $$ |      /$$$$$$  |/$$$$$$  |$$$$$$$$/ $$ |
             $$  \/$$/ $$ |  $$ |$$ |  $$ |$/  $$ |__$$ |$$ |__          $$ |      $$ |  $$ |$$ \__$$/    $$ |   $$ |
              $$  $$/  $$ |  $$ |$$ |  $$ |    $$    $$< $$    |         $$ |      $$ |  $$ |$$      \    $$ |   $$ |
               $$$$/   $$ |  $$ |$$ |  $$ |    $$$$$$$  |$$$$$/          $$ |      $$ |  $$ | $$$$$$  |   $$ |   $$/ 
                $$ |   $$ \__$$ |$$ \__$$ |    $$ |  $$ |$$ |_____       $$ |_____ $$ \__$$ |/  \__$$ |   $$ |    __ 
                $$ |   $$    $$/ $$    $$/     $$ |  $$ |$$       |      $$       |$$    $$/ $$    $$/    $$ |   /  |
                $$/     $$$$$$/   $$$$$$/      $$/   $$/ $$$$$$$$/       $$$$$$$$/  $$$$$$/   $$$$$$/     $$/    $$/ 
            )";

            int loseStatementX = winCols / 2 - loseStatement.find('\n') / 2;
            int loseStatementY = winRows / 2;

            printAt(loseStatementX, loseStatementY, loseStatement);
            // Clear the lose statement
            clearScreen();
            // Wait for 2 seconds
            this_thread::sleep_for(chrono::seconds(2));

            // Return to the main menu
            continue;
        }
        
        else if (game_result == 1)
        {   
            // Handle win state
            // Print win statement in the middle of the screen with ASCII art
            string winStatement = R"(
                _______   _______   ________   ______   __    __         ______   __    __  ________  __ 
            /       \ /       \ /        | /      \ /  |  /  |       /      \ /  |  /  |/        |/  |
            $$$$$$$  |$$$$$$$  |$$$$$$$$/ /$$$$$$  |$$ | /$$/       /$$$$$$  |$$ |  $$ |$$$$$$$$/ $$ |
            $$ |__$$ |$$ |__$$ |$$ |__    $$ |__$$ |$$ |/$$/        $$ |  $$ |$$ |  $$ |   $$ |   $$ |
            $$    $$< $$    $$< $$    |   $$    $$ |$$  $$<         $$ |  $$ |$$ |  $$ |   $$ |   $$ |
            $$$$$$$  |$$$$$$$  |$$$$$/    $$$$$$$$ |$$$$$  \        $$ |  $$ |$$ |  $$ |   $$ |   $$/ 
            $$ |__$$ |$$ |  $$ |$$ |_____ $$ |  $$ |$$ |$$  \       $$ \__$$ |$$ \__$$ |   $$ |    __ 
            $$    $$/ $$ |  $$ |$$       |$$ |  $$ |$$ | $$  |      $$    $$/ $$    $$/    $$ |   /  |
            $$$$$$$/  $$/   $$/ $$$$$$$$/ $$/   $$/ $$/   $$/        $$$$$$/   $$$$$$/     $$/    $$/ 
            )";

            int winStatementX = winCols / 2 - winStatement.find('\n')/ 2;
            int winStatementY = winRows / 2;

            printAt(winStatementX, winStatementY, winStatement);
            // Clear the win statement
            clearScreen();
            // Wait for 2 seconds
            this_thread::sleep_for(chrono::seconds(2));

            // Return to the main menu
            continue;
        }
    cursorShow();

    return 0;
    }
}

/* Win Screen
 _______   _______   ________   ______   __    __         ______   __    __  ________  __ 
/       \ /       \ /        | /      \ /  |  /  |       /      \ /  |  /  |/        |/  |
$$$$$$$  |$$$$$$$  |$$$$$$$$/ /$$$$$$  |$$ | /$$/       /$$$$$$  |$$ |  $$ |$$$$$$$$/ $$ |
$$ |__$$ |$$ |__$$ |$$ |__    $$ |__$$ |$$ |/$$/        $$ |  $$ |$$ |  $$ |   $$ |   $$ |
$$    $$< $$    $$< $$    |   $$    $$ |$$  $$<         $$ |  $$ |$$ |  $$ |   $$ |   $$ |
$$$$$$$  |$$$$$$$  |$$$$$/    $$$$$$$$ |$$$$$  \        $$ |  $$ |$$ |  $$ |   $$ |   $$/ 
$$ |__$$ |$$ |  $$ |$$ |_____ $$ |  $$ |$$ |$$  \       $$ \__$$ |$$ \__$$ |   $$ |    __ 
$$    $$/ $$ |  $$ |$$       |$$ |  $$ |$$ | $$  |      $$    $$/ $$    $$/    $$ |   /  |
$$$$$$$/  $$/   $$/ $$$$$$$$/ $$/   $$/ $$/   $$/        $$$$$$/   $$$$$$/     $$/    $$/ 

*/

/* Lose Screen
 __      __  ______   __    __  __  _______   ________        __         ______    ______   ________  __ 
/  \    /  |/      \ /  |  /  |/  |/       \ /        |      /  |       /      \  /      \ /        |/  |
$$  \  /$$//$$$$$$  |$$ |  $$ |$$/ $$$$$$$  |$$$$$$$$/       $$ |      /$$$$$$  |/$$$$$$  |$$$$$$$$/ $$ |
 $$  \/$$/ $$ |  $$ |$$ |  $$ |$/  $$ |__$$ |$$ |__          $$ |      $$ |  $$ |$$ \__$$/    $$ |   $$ |
  $$  $$/  $$ |  $$ |$$ |  $$ |    $$    $$< $$    |         $$ |      $$ |  $$ |$$      \    $$ |   $$ |
   $$$$/   $$ |  $$ |$$ |  $$ |    $$$$$$$  |$$$$$/          $$ |      $$ |  $$ | $$$$$$  |   $$ |   $$/ 
    $$ |   $$ \__$$ |$$ \__$$ |    $$ |  $$ |$$ |_____       $$ |_____ $$ \__$$ |/  \__$$ |   $$ |    __ 
    $$ |   $$    $$/ $$    $$/     $$ |  $$ |$$       |      $$       |$$    $$/ $$    $$/    $$ |   /  |
    $$/     $$$$$$/   $$$$$$/      $$/   $$/ $$$$$$$$/       $$$$$$$$/  $$$$$$/   $$$$$$/     $$/    $$/ 
                                                                                                         
                                                                                                         
*/

/*
 _______   ________  __    __  _______   ______  __    __   ______             
/       \ /        |/  \  /  |/       \ /      |/  \  /  | /      \            
$$$$$$$  |$$$$$$$$/ $$  \ $$ |$$$$$$$  |$$$$$$/ $$  \ $$ |/$$$$$$  |           
$$ |__$$ |$$ |__    $$$  \$$ |$$ |  $$ |  $$ |  $$$  \$$ |$$ | _$$/            
$$    $$/ $$    |   $$$$  $$ |$$ |  $$ |  $$ |  $$$$  $$ |$$ |/    |           
$$$$$$$/  $$$$$/    $$ $$ $$ |$$ |  $$ |  $$ |  $$ $$ $$ |$$ |$$$$ |           
$$ |      $$ |_____ $$ |$$$$ |$$ |__$$ | _$$ |_ $$ |$$$$ |$$ \__$$ |__  __  __ 
$$ |      $$       |$$ | $$$ |$$    $$/ / $$   |$$ | $$$ |$$    $$//  |/  |/  |
$$/       $$$$$$$$/ $$/   $$/ $$$$$$$/  $$$$$$/ $$/   $$/  $$$$$$/ $$/ $$/ $$/
*/