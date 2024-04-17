#include "gameloop.hpp"
#include "helpers.hpp"
#include "maze2D.hpp"
#include "colors.hpp"
#include "minigame.hpp"
#include "loader.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;

Timer::Timer(bool *thePis_writing, int theRow, int theCol, double thePausedTime)
{
    time = 0;
    paused_time = thePausedTime;
    is_writing = thePis_writing;
    timer_row = theRow;
    timer_col = theCol;
}

void Timer::start()
{
    is_running = true;
    timer = thread(&Timer::job, this);
}

void Timer::pause()
{
    is_running = false;
    paused_time = time;
    stop();
}

string Timer::stop()
{
    is_running = false;
    if (timer.joinable())
    {
        timer.join();
    }
    return to_string(time).substr(0, 6);
}

Timer::~Timer()
{
    time = 0;
    paused_time = 0;
    stop();
}

void Timer::job()
{
    auto start_time = chrono::system_clock::now() - chrono::duration<double>(paused_time);
    while (is_running)
    {
        if (!*is_writing)
        {
            chrono::duration<float> time_elapsed_seconds = chrono::system_clock::now() - start_time;
            if (time_elapsed_seconds.count() - time > 0.01)
            {
                time = time_elapsed_seconds.count();
                cout << "\033[" << timer_row << ";" << timer_col << "H[" << fixed << setprecision(2) << time << "s]" << flush;
            }
        }
    }
    // auto time_elapsed = chrono::system_clock::now() - start_time;
    // time = time_elapsed.count();
}

Gameloop::Gameloop()
/**
 * lookup: to find the direction of the player movement
 *      w = move upward with one row ;
 *      a = move left with one col;
 *      s = move downward with one row;
 *      d = move right with one col;

 * effect: do the animation while breaking the wall
 *      w = effect of "||" while breaking upward;
 *      a = effect of "--" while breaking left;
 *      s = effect of "||" while breaking downward;
 *      d = effect of "--" while breaking right;
*/
{
    // lookup: the allowed player movement
    lookup = {
        {char('w'), {0, -1}},
        {char('a'), {-1, 0}},
        {char('s'), {0, 1}},
        {char('d'), {1, 0}},
    };
    // effect: the effect of breaking wall
    effect = {
        {char('w'), {"||"}},
        {char('a'), {"--"}},
        {char('s'), {"||"}},
        {char('d'), {"--"}},
    };
}

Gameloop::~Gameloop()
{
}


bool Gameloop::intersection(int pos_x, int pos_y, int **themap, bool the_first_move)
/**
 * intersection: used to check whether there are road [0] around the user location
 *      if there is/are [0] around the location 
 *      counting [num] + 1;
 *      if the num >= 2 return 0 
*/
{
    int num = 0;
    if (themap[pos_y + 1][pos_x] == 0)
        num++;
    if (themap[pos_y - 1][pos_x] == 0)
        num++;
    if (themap[pos_y][pos_x + 1] == 0)
        num++;
    if (themap[pos_y][pos_x - 1] == 0)
        num++;

    if (num <= 1 && the_first_move == false)
        return 0;
    return 1;
}


vector<int> Gameloop::checkwall(char playerinput, int *pos, int **themap, int width, int height, string detector)
/**
 * checkwall : allow the player to knowing the wall they going to break down / and showing that with "detector"
 *      resolvelist : list all the wall which gonna to break;
 *      the block which will break will show in red
 *      the maximum number of block gonna to break is 6
*/
{

    // save the wall data into the list that allow the player to change their idea
    vector<int> resolvelist;
    resolvelist.clear();

    for (int i = 1; i < 7; i++)
    {
        if (themap[pos[0] + i * lookup[playerinput][1]][pos[1] + i * lookup[playerinput][0]] != 2)
        {
            if (pos[1] + i * lookup[playerinput][0] != 0 && pos[0] + i * lookup[playerinput][1] != 0 && pos[1] + i * lookup[playerinput][0] != width - 1 && pos[0] + i * lookup[playerinput][1] != height - 1)
            {
                this_thread::sleep_for(chrono::milliseconds(1));
                resolvelist.push_back(themap[pos[0] + i * lookup[playerinput][1]][pos[1] + i * lookup[playerinput][0]]);
                // print the wall which the player going to break down with the "detector"
                printAt(2 * (pos[1] + i * lookup[playerinput][0]), pos[0] + i * lookup[playerinput][1], detector);
            }
            else if (pos[1] + i * lookup[playerinput][0] == 0 || pos[0] + i * lookup[playerinput][1] == 0 || pos[1] + i * lookup[playerinput][0] == width - 1 || pos[0] + i * lookup[playerinput][1] == height - 1)
            {
                return resolvelist;
            }
        }
        else
        {
            return resolvelist;
        }
    }
    return resolvelist;
}
int Gameloop::getintoMinigame()
/**
 * getintoMinigame : to letting the player to know they gonna to start the minigame and prevent miss-clicking
*/
{

    clearScreen();
    int winCols = getWinCols();
    int winRows = getWinRows();
    printAt(winCols / 2 -36, winRows / 2 - 5, " _____                    _____            _____            _____        ");
    printAt(winCols / 2 -36, winRows / 2 - 4, "|  _  |___ ___ ___ ___   |  _  |___ _ _   |  |  |___ _ _   |_   _|___    ");
    printAt(winCols / 2 -36, winRows / 2 - 3, "|   __|  _| -_|_ -|_ -|  |     |   | | |  |    -| -_| | |    | | | . |   ");
    printAt(winCols / 2 -36, winRows / 2 - 2, "|__|  |_| |___|___|___|  |__|__|_|_|_  |  |__|__|___|_  |    |_| |___|   ");
    printAt(winCols / 2 -36, winRows / 2 - 1, "                                   |___|            |___|                ");
    printAt(winCols / 2 -36, winRows / 2, "                                                                         ");
    printAt(winCols / 2 -36, winRows / 2 + 1, " _____ _           _      _   _                _     _                   ");
    printAt(winCols / 2 -36, winRows / 2 + 2, "|   __| |_ ___ ___| |_   | |_| |_ ___    _____|_|___|_|___ ___ _____ ___ ");
    printAt(winCols / 2 -36, winRows / 2 + 3, "|__   |  _| .'|  _|  _|  |  _|   | -_|  |     | |   | | . | .'|     | -_|");
    printAt(winCols / 2 -36, winRows / 2 + 4, "|_____|_| |__,|_| |_|    |_| |_|_|___|  |_|_|_|_|_|_|_|_  |__,|_|_|_|___|");
    printAt(winCols / 2 -36, winRows / 2 + 5, "                                                      |___|              ");
    
    getch();
    int minigame_result = Minigame().run();
    return minigame_result;
}

int Gameloop::run(State loadedGameState)
{

    int winCols = getWinCols(); // size of window cols 
    int winRows = getWinRows(); // size of window rows
    bool the_first_move;     // check if player had moved since key pressed
    bool choice;             // check if player had choosen the direction of breaking wall
    bool is_writing = false; // check if the screen is writing

    srand(time(NULL)); // Seed the random number generator with the current time

    Maze2D maze(winCols / 2 - 2, (winRows % 2 == 0 ? winRows + 1 : winRows) - 2, rand(), loadedGameState.Map2D);
    Timer timer(&is_writing, 0, winCols / 2 - 7, stod(loadedGameState.timerPausedTime));
    gameState = loadedGameState;
    clearScreen();
    maze.printMap();

    timer.start();
    while (true)
    {
        choice = false;
        char input = getch();
        // check if the player is in the first move or not
        the_first_move = true;

        if (input == 'q')
        {
            timer.stop();
            clearScreen();
            return 3;
        } else if (input == 'p')
        {
            gameState.timerPausedTime = timer.stop();
            gameState.Map2D = maze.map2D;
            gameState.mapHeight = maze.result_height;
            gameState.mapWidth = maze.result_width;
            gameState.winCols = winCols;
            gameState.winRows = winRows;
            string savedTo = loader.saveStateToFile(gameState);
            clearScreen();
            frame(winCols, winRows);
            string m = "Game saved to " + savedTo + ". Press any key to continue.";
            printAt(winCols / 2 - m.length() / 2, winRows / 2, m);
            getch();
            return 2;
        } else if ( input != 'w' && input != 'a' && input != 's' && input != 'd' )
        {
            continue;
        }

        while (intersection(gameState.position[1], gameState.position[0], maze.map2D, the_first_move))
        {
            the_first_move = false;
            // player movement: only stop when there is intersection or wall
            if (maze.map2D[gameState.position[0] + lookup[input][1]][gameState.position[1] + lookup[input][0]] != 0)
            {
                is_writing = true;
                this_thread::sleep_for(chrono::milliseconds(25));
                // replace the moved place to glyph[8]
                printAt(2 * gameState.position[1], gameState.position[0], maze.glyphs[8]); // change the moved space
                maze.editMap(gameState.position[0], gameState.position[1], 8);             // edit the map

                gameState.position[0] += lookup[input][1];                                 // move the player
                gameState.position[1] += lookup[input][0];                                 // move the player
                printAt(2 * gameState.position[1], gameState.position[0], maze.glyphs[9]); // change the player location
                is_writing = false;

                // end the game when the player arrive to the "end"
                if (maze.map2D[gameState.position[0]][gameState.position[1]] == 2)
                {
                    timer.stop();
                    clearScreen();
                    return 1;
                }

                // start the minigame when the player arrive to the "function"
                if (maze.map2D[gameState.position[0]][gameState.position[1]] == 3)
                {
                    is_writing = true;
                    this_thread::sleep_for(chrono::milliseconds(1));
                    printAt(2 * gameState.position[1], gameState.position[0], maze.glyphs[9]); // change the player location
                    maze.editMap(gameState.position[0], gameState.position[1], 9);
                    is_writing = false;
                    // get into the game
                    timer.pause();
                    int minigame_result = getintoMinigame();
                    // end the game 
                    clearScreen();
                    is_writing = true;
                    maze.printMap();
                    is_writing = false;
                    if (minigame_result == 1)
                    {
                        // win can get ability to break the wall
                        char playerinput = getch(); // the direction of breaking
                        // chooce the direction of wall breaking
                        while (choice == false)
                        {
                            is_writing = true;
                            // showing the wall going to break
                            resolvelist = checkwall(playerinput, gameState.position, maze.map2D, maze.result_width, maze.result_height, maze.glyphs[5]);
                            is_writing = false;
                            // confirm the break
                            char playerinput2 = playerinput;
                            playerinput = getch();

                            if (playerinput == playerinput2)
                            {
                                // breaking wall
                                choice = true;
                                for (int i = 1; i < 7; i++)
                                {
                                    if (maze.map2D[gameState.position[0] + i * lookup[playerinput][1]][gameState.position[1] + i * lookup[playerinput][0]] != 2)
                                    {
                                        if (gameState.position[1] + i * lookup[playerinput][0] != 0 && gameState.position[0] + i * lookup[playerinput][1] != 0 && gameState.position[1] + i * lookup[playerinput][0] != maze.result_width - 1 && gameState.position[0] + i * lookup[playerinput][1] != maze.result_height - 1)
                                        {
                                            is_writing = true;
                                            this_thread::sleep_for(chrono::milliseconds(50));
                                            printAt(2 * (gameState.position[1] + i * lookup[playerinput][0]), gameState.position[0] + i * lookup[playerinput][1], effect[playerinput]); // effect of breaking wall

                                            printAt(2 * (gameState.position[1] + i * lookup[playerinput][0]), gameState.position[0] + i * lookup[playerinput][1], maze.glyphs[1]); // change the moved space
                                            maze.editMap(gameState.position[0] + i * lookup[playerinput][1], gameState.position[1] + i * lookup[playerinput][0], 1);               // edit the map
                                            is_writing = false;
                                        }
                                        else if (gameState.position[1] + i * lookup[playerinput][0] == 0 || gameState.position[0] + i * lookup[playerinput][1] == 0 || gameState.position[1] + i * lookup[playerinput][0] == maze.result_width - 1 || gameState.position[0] + i * lookup[playerinput][1] == maze.result_height - 1)
                                        {
                                            break;
                                        }
                                    }
                                    else if (gameState.position[1] + i * lookup[playerinput][0] == 0 || gameState.position[0] + i * lookup[playerinput][1] == 0 || gameState.position[1] + i * lookup[playerinput][0] == maze.result_width - 1 || gameState.position[0] + i * lookup[playerinput][1] == maze.result_height - 1)
                                    {
                                        break;
                                    }
                                }
                            }
                            else if (playerinput != playerinput2)
                            {
                                for (int i = 1; i <= resolvelist.size(); i++)
                                { 
                                    // refresh the map again
                                    is_writing = true;
                                    this_thread::sleep_for(chrono::milliseconds(1));
                                    printAt(2 * (gameState.position[1] + i * lookup[playerinput2][0]), gameState.position[0] + i * lookup[playerinput2][1], maze.glyphs[resolvelist[i - 1]]);
                                    is_writing = false;
                                }
                            }
                        }
                    }
                    timer.start();
                    break;
                }
                maze.editMap(gameState.position[0], gameState.position[1], 9);
            }
            else
                break;
        }
    }

    return 0;
}
