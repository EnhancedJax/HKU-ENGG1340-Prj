#include "helpers.hpp"
#include <iostream>
#include <string>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

using namespace std;

void cursorHide()
{
    cout << "\033[?25l";
}

void cursorShow()
{
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
    cout << "\033[" << y + 1 << ";" << x + 1 << "H";
    // Print character
    cout << s;
    // Restore saved cursor position
    cout << "\033[u" << flush;
}

void moveCursorTo(int x, int y)
{
    cout << "\033[" << y + 1 << ";" << x + 1 << "H";
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

char getch_emptyinput()
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

    // set up the file descriptor set for select()
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(file_desc, &readfds);

    // set up the timeout struct for select() 
    struct timeval timeout;
    timeout.tv_sec = 4;
    timeout.tv_usec = 0;

    // wait until input is available or timeout occurs
    int ready = select(file_desc + 1, &readfds, NULL, NULL, &timeout);

    // check if input is avaiable
    if (ready == 1 && FD_ISSET(file_desc, &readfds)) 
    {
        char ch;
        read(file_desc, &ch, 1);

        tcsetattr(file_desc, TCSANOW, &old_settings);
        return ch;
    } else 
    {
        tcsetattr(file_desc, TCSANOW, &old_settings);
        return '\0';
    }

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
int getWinRows()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_row;
}

/*
    Get the number of columns in the terminal
    Returns the number of columns
*/
int getWinCols()
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}
/*
    build a UI frame based on the size of console
*/
void frame(int winCols, int winRows) // input current size of console 
{
    // print the top of the UI frame
    cout << "╔";
    for (int i = 0; i < winCols - 2; i++)
    {
        cout << "═";
    }
    cout << "╗" << endl;
    //

    // print the UI pillars of two sides 
    for (int i = 1; i < winRows - 2; i++)
    {
        cout << "║";
        for (int j = 1; j < winCols - 1; j++)
        {
            cout << " ";
        }
        cout << "║" << endl;
    }
    //

    // print the bottom of the UI frame
    cout << "╚";
    for (int i = 0; i < winCols - 2; i++)
    {
        cout << "═";
    }
    cout << "╝" << endl;
    //
}
/*
    build a thin frame for the word inputed (used only for the menu botton) (only suitable for words in 4)
*/
void botton(int x, int y, string colors, string s)
{
    string top = R"(╭──────╮)";
    string mid = R"(│      │)";
    string bot = R"(╰──────╯)";

    printAt(x - 2, y - 1, colors + top);
    printAt(x - 2, y, colors + mid);
    printAt(x, y, colors + s);
    printAt(x - 2, y + 1, colors + bot);
}

    /*
    Example:
            ╭──────╮
            │ PLAY │
            ╰──────╯
    */

