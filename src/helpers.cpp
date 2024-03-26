#include "helpers.hpp"
#include <iostream>
#include <string>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
using namespace std;

void cursorHide() {
    cout << "\033[?25l";
}

void cursorShow() {
    cout << "\033[?25h";
}

/*
    Print a string at a specific position on the terminal
    x: x coordinate
    y: y coordinate
    s: string to print
*/
void printAt(int x, int y, string s)
{
    // Save current cursor position
    cout << "\033[s";
    // Move cursor to x, y
    cout << "\033[" << y +1 << ";" << x +1 << "H";
    // Print character
    cout << s;
    // Restore saved cursor position
    cout << "\033[u" << flush;
}

void clearScreen()
{
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    cout << "\033[2J\033[H";
}

/*
    Get a single character from stdin without waiting for enter, by changing the terminal settings
    Returns the character read
*/
char getch()
{
    // get stdin file descriptor
    int file_desc = STDIN_FILENO;
    // get stdin file settings
    struct termios old_settings, new_settings;
    tcgetattr(file_desc, &old_settings);
    new_settings = old_settings;
    // modifies settings
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(file_desc, TCSANOW, &new_settings);
    // read a single byte
    char ch;
    read(file_desc, &ch, 1);
    // set the stdin settings back to before raw modification
    tcsetattr(file_desc, TCSANOW, &old_settings);
    return ch;
}

/*
    Toggle the echo of the terminal
*/
void toggleEcho()
{
    // get stdin file descriptor
    int file_desc = STDIN_FILENO;
    // get stdin file settings
    struct termios old_settings, new_settings;
    tcgetattr(file_desc, &old_settings);
    new_settings = old_settings;
    // modifies settings
    new_settings.c_lflag ^= ECHO;
    tcsetattr(file_desc, TCSANOW, &new_settings);
}

/*
    Get the number of rows in the terminal
    Returns the number of rows
*/
int getWinRows() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_row;
}

/*
    Get the number of columns in the terminal
    Returns the number of columns
*/
int getWinCols() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

void frame(int winCols, int winRows) {

    cout << "╔";
    for (int i = 0; i < winCols - 2; i++) 
    {
        cout << "═";
    }
    cout << "╗" << endl;
    
    for (int i = 1; i < winRows - 2; i++) {
        cout << "║" ;
        for (int j = 1; j < winCols - 1; j++){
            cout << " ";
        }
        cout << "║" << endl;
    }

    cout << "╚";
    for (int i = 0; i < winCols - 2; i++) 
    {
        cout << "═";
    }
    cout << "╝" << endl;
}
void botton(int x, int y, string colors, string s )
{
    string top = R"(╭──────╮)";
    string mid = R"(│      │)";
    string bot = R"(╰──────╯)";

    printAt(x-2,y-1,colors + top);
    printAt(x-2,y,colors + mid);
    printAt(x,y,colors + s);
    printAt(x-2,y+1, colors + bot);    
}