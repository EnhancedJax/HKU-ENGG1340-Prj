#ifndef GAMELOOP_HPP_
#define GAMELOOP_HPP_

#include "maze2D.hpp"
#include "loader.hpp"
#include <thread>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Timer
{
    thread timer;
    int timer_row;
    int timer_col;
    bool is_running;
    double paused_time;
    void job();
public:
    double time;
    bool* is_writing;
    Timer(bool* thePis_writing, int theRow, int theCol, double thePausedTime = 0.0);
    ~Timer();
    void start();
    void pause();
    string stop();
};

class Gameloop
{

    unordered_map<char, array<int,2>> lookup; 
    unordered_map<char, string> effect; 
    vector<int> resolvelist;
    vector<int> checkwall(char playerinput, int* position, int** themap, int width, int height, string detector);
    bool intersection(int pos_x, int pos_y, int **themap, bool the_first_move);
    int getintoMinigame();
public:
    State gameState;
    Loader loader; // initialize loader for writing to file
    Gameloop();
    ~Gameloop();
    int run(State loadedGameState);
    
};

#endif 