#include "pagedmenu.hpp"
#include "helpers.hpp"
#include "colors.hpp"
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

int PagedMenu::displayMenu()
{
    int response = 0;           // response is defulted to 0 (Play)
    int winCols = getWinCols(); // console width
    int winRows = getWinRows(); // console height

    using namespace std;
    string top = R"(╭──────╮)";
    string mid = R"(│      │)";
    string bot = R"(╰──────╯)";

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

    int offsetY = -4;
    int bottonH = 3;

    botton(60, (winRows / 2) + offsetY, COLOR_GREEN, play);                 // botton is a function in helers.cpp which helps
    botton(60, (winRows / 2) + offsetY + bottonH, COLOR_DEFAULT, load);     // building a thin frame around the words
    botton(60, (winRows / 2 + offsetY + bottonH * 2), COLOR_DEFAULT, quit); // botton(x_coor of the word, y_coor of the word, colour , word to be showed)

    char userinput; // detect user input

    do
    {
        userinput = getch();
        switch (userinput)
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
        botton(60, (winRows / 2) + offsetY, (response == 0) ? COLOR_GREEN : COLOR_DEFAULT, play);
        botton(60, (winRows / 2) + offsetY + bottonH, (response == 1) ? COLOR_YELLOW : COLOR_DEFAULT, load);
        botton(60, (winRows / 2 + offsetY + bottonH * 2), (response == 2) ? COLOR_RED : COLOR_DEFAULT, quit);
    } while (userinput != '\n');

    return response; // outputing the choice of player
}
