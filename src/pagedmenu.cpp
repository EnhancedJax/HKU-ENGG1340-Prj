#include "pagedmenu.hpp"
#include "helpers.hpp"
#include "colors.hpp"
#include "loader.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

PagedMenu::PagedMenu()
{
}

PagedMenu::~PagedMenu()
{
}


/*
@ displaymenu is a function to print the main menu for the game
@ user will input 'w' and 's' to choose between 'Play' , 'Load' and 'Quit'
*/
int PagedMenu::displayMenu()
{
    int response = 0;           // response is defulted to 0 (Play)
    int winCols = getWinCols(); // console width
    int winRows = getWinRows(); // console height

    using namespace std;

    string play = "PLAY"; // the pattern of play
    string load = "LOAD"; // the pattern of load
    string quit = "QUIT"; // the pattern of quit

    frame(winCols, winRows); // the UI frame
    printAt(0, (winRows - 11) /2, R"(x        _    __  __    _     ________ _   _  ____    
x       / \  |  \/  |  / \   |__  /_ _| \ | |/ ___|   
x      / _ \ | |\/| | / _ \    / / | ||  \| | |  _    
x     / ___ \| |  | |/ ___ \  / /_ | || |\  | |_| |   
x    /_/__ \_\_|  |_/_/   \_\/____|___|_| \_|\____|__ 
x    | __ )|  _ \| ____|  / \  | |/ / _ \| | | |_   _|
x    |  _ \| |_) |  _|   / _ \ | ' / | | | | | | | |  
x    | |_) |  _ <| |___ / ___ \| . \ |_| | |_| | | |  
x    |____/|_| \_\_____/_/   \_\_|\_\___/ \___/  |_|  )");                        // the title showed in UI

    int offsetY = -4;   // varibles for adjusting the botton to be parallel
    int bottonH = 3;    // varibles for adjusting the botton to be parallel

    botton(60, (winRows / 2) + offsetY, COLOR_GREEN, play);                 // botton is a function in helers.cpp which helps
    botton(60, (winRows / 2) + offsetY + bottonH, COLOR_DEFAULT, load);     // building a thin frame around the words
    botton(60, (winRows / 2 + offsetY + bottonH * 2), COLOR_DEFAULT, quit); // botton(x_coor of the word, y_coor of the word, colour , word to be showed)

    char userinput; // detect user input

    do
    {
        userinput = getch();    // obtain user input
        switch (userinput)      // different situation of userinput ( w & s )
        {
        case 'w':
            // Move up
            if (response > 0)
            {
                response--;
            }
            break;
        case 's':
            // Move down
            if (response < 2)
            {
                response++;
            }
            break;
        }
        // Update the buttons
        botton(60, (winRows / 2) + offsetY, (response == 0) ? COLOR_GREEN : COLOR_DEFAULT, play);                  // print a 'play' botton in green when the response is 0 
        botton(60, (winRows / 2) + offsetY + bottonH, (response == 1) ? COLOR_YELLOW : COLOR_DEFAULT, load);       // print a 'load' botton in yellow when the response is 1 
        botton(60, (winRows / 2 + offsetY + bottonH * 2), (response == 2) ? COLOR_RED : COLOR_DEFAULT, quit);      // print a 'quit' botton in red when the response is 2
    } while (userinput != '\n');    // stop the choosing process and confirm the player choice with "enter" being inputed

    return response; // outputing the choice of player
}


/*
@ WinningScreen is a function to print out the winning UI art 
@ user will only see this when they successfully get out from the maze
@ no input from user will be needed in this function
*/
void PagedMenu::WinningScreen()
{
    int splashSpeed = 100;      // 100 milliseconds          
    int winCols = getWinCols();
    int winRows = getWinRows();

    frame(winCols, winRows); // UI frame
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2, " _______   _______   ________   ______   __    __         ______   __    __  ________  __ ");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2 + 1, "/       \\ /       \\ /        | /      \\ /  |  /  |       /      \\ /  |  /  |/        |/  |");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2 + 2, "$$$$$$$  |$$$$$$$  |$$$$$$$$/ /$$$$$$  |$$ | /$$/       /$$$$$$  |$$ |  $$ |$$$$$$$$/ $$ |");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2 + 3, "$$ |__$$ |$$ |__$$ |$$ |__    $$ |__$$ |$$ |/$$/        $$ |  $$ |$$ |  $$ |   $$ |   $$ |");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2 + 4, "$$    $$< $$    $$< $$    |   $$    $$ |$$  $$<         $$ |  $$ |$$ |  $$ |   $$ |   $$ |");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2 + 5, "$$$$$$$  |$$$$$$$  |$$$$$/    $$$$$$$$ |$$$$$  \\        $$ |  $$ |$$ |  $$ |   $$ |   $$/ ");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2 + 6, "$$ |__$$ |$$ |  $$ |$$ |_____ $$ |  $$ |$$ |$$  \\       $$ \\__$$ |$$ \\__$$ |   $$ |    __ ");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2 + 7, "$$    $$/ $$ |  $$ |$$       |$$ |  $$ |$$ | $$  |      $$    $$/ $$    $$/    $$ |   /  |");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));
            printAt((winCols - 90) / 2, (winRows - 9) / 2 + 8, "$$$$$$$/  $$/   $$/ $$$$$$$$/ $$/   $$/ $$/   $$/        $$$$$$/   $$$$$$/     $$/    $$/ ");
            this_thread::sleep_for(chrono::milliseconds(splashSpeed));

    /* WINNING SCREEN ART 

    " _______    _______    ________   ______    __    __         ______   __    __  ________  __ "
    "/       \\ /       \\ /        | /      \\ /  |  /  |      /      \\ /  |  /  |/        |/  |"
    "$$$$$$$  |$$$$$$$  |$$$$$$$$/ /$$$$$$  |$$ | /$$/       /$$$$$$    |$$ |   $$ |$$$$$$$$/ $$ |"
    "$$ |__$$ |$$ |__$$ |$$ |__    $$ |__$$ |$$ |/$$/        $$ |   $$  |$$ |   $$ |   $$ |   $$ |"
    "$$    $$< $$    $$< $$    |   $$    $$ |$$  $$<         $$ |   $$  |$$ |   $$ |   $$ |   $$ |"
    "$$$$$$$  |$$$$$$$  |$$$$$/    $$$$$$$$ |$$$$$  \\       $$ |   $$  |$$ |   $$ |   $$ |   $$/ "
    "$$ |__$$ |$$ |  $$ |$$ |_____ $$ |  $$ |$$ |$$  \\      $$ \\__$$|  $$ \\__$$ |   $$ |    __ "
    "$$    $$/ $$ |  $$ |$$       |$$ |  $$ |$$ | $$  |      $$    $$/   $$    $$/     $$ |   /  |"
    "$$$$$$$/  $$/   $$/ $$$$$$$$/ $$/   $$/ $$/   $$/        $$$$$$/     $$$$$$/      $$/    $$/ "

    */

    //every line would be output one by one with some time stopped between
    // making a waterfall effect

}
