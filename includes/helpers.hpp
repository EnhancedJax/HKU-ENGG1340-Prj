#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>

void cursorHide();
void cursorShow();
void printAt(int, int, std::string);
void moveCursorTo(int, int);
void clearScreen();
int getWinRows();
int getWinCols();
char getch();
char getch_emptyinput();
void toggleEcho();
void frame(int winCols, int winRows);
void botton(int x, int y, std::string, std::string );

#endif