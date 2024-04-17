#include "minigame.hpp"
#include "helpers.hpp"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

Minigame::Minigame()
{
}

Minigame::~Minigame()
{
}

/* ▲ ▼ ◀ ▶ */

bool Minigame::countdown(int col, int row)
{ // countdown timer with a bar shown //

        bool time_out = true;

        int duration = 4;
        int divisor = 20;
        int prog = duration;
        string bar_symbol = "/", timer, bar;

        int midcol_timer = (col - 6) / 2;
        int midcol_bar = (col - divisor) / 2;

        this_thread::sleep_for(chrono::milliseconds(500));

        for (int i = 0; i <= divisor; i++)
        {
                prog = duration - i * duration / divisor;
                timer = "> " + to_string(prog) + " <";
                printAt(midcol_timer, 2, timer);

                bar = "";
                for (int j = 0; j <= divisor; j++)
                {
                        if (j <= i)
                        {
                                bar += " ";
                        }
                        else
                        {
                                bar += bar_symbol;
                        }
                }
                printAt(midcol_bar, 1, bar);

                this_thread::sleep_for(chrono::milliseconds(1000 * duration / divisor));

                if (stopCountdown == true)
                {
                        time_out = false;
                        return true;
                        break;
                }
                printAt(midcol_bar, 1, "           ");
        }

        if (time_out == true)
        {
                string quote = "           Oops! Time's out           ";

                int midrow_quote = (row - 5) / 2;
                int midcol_quote = (col - quote.size()) / 2;
                printAt(midcol_quote, midrow_quote + 4, quote);

                this_thread::sleep_for(chrono::seconds(1));

                stop_direction = true;
                return false;
        }
}

bool Minigame::direction()
{
        char answer[6];
        int arr[6];

        int col = getWinCols();
        int row = getWinRows();

        int midcol = (col - 41) / 2;
        int midrow = (row - 5) / 2;

        string arrows[4] = {"▲", "▼", "◀", "▶"}; // randomly generate the output //
        srand(time(NULL));
        for (int i = 0; i < 6; i++)
        {
                int number = rand() % 4;
                arr[i] = number;
        };

        printAt(midcol, midrow, "╭───╮  ╭───╮  ╭───╮  ╭───╮  ╭───╮  ╭───╮");
        this_thread::sleep_for(chrono::milliseconds(100));
        printAt(midcol, midrow + 1, "│ " + arrows[arr[0]] + " │  │ " + arrows[arr[1]] + " │  │ " + arrows[arr[2]] + " │  │ " + arrows[arr[3]] + " │  │ " + arrows[arr[4]] + " │  │ " + arrows[arr[5]] + " │");
        this_thread::sleep_for(chrono::milliseconds(100));
        printAt(midcol, midrow + 2, "╰───╯  ╰───╯  ╰───╯  ╰───╯  ╰───╯  ╰───╯");

        for (int i = 0; i < 6; ++i)
        { // turn the output into an answer list //

                char letter1 = 'w';
                char letter2 = 's';
                char letter3 = 'a';
                char letter4 = 'd';

                if (arrows[arr[i]] == "▲")
                {
                        answer[i] = letter1;
                }
                else if (arrows[arr[i]] == "▼")
                {
                        answer[i] = letter2;
                }
                else if (arrows[arr[i]] == "◀")
                {
                        answer[i] = letter3;
                }
                else if (arrows[arr[i]] == "▶")
                {
                        answer[i] = letter4;
                }
                else
                {
                        printAt(3, 3, "wrong");
                }
        }

        string output[10] = {arrows[arr[0]], arrows[arr[1]], arrows[arr[2]], arrows[arr[3]], arrows[arr[4]], arrows[arr[5]]};

        for (int i = 0; i < 6; i++)
        { // check user's answer one by one //   
                char input = getch_emptyinput();
                
                if (input != '\0') 
                {
                        continue;
                } else if (input == '\0')
                {
                        stop_direction=true;
                        return false;
                }


                if (input != answer[i] && stop_direction == false)
                { // if user's input is wrong //
                        stopCountdown = true;
                        output[i] = "✘";
                        printAt(midcol, midrow, "╭───╮  ╭───╮  ╭───╮  ╭───╮  ╭───╮  ╭───╮");
                        printAt(midcol, midrow + 1, "│ " + output[0] + " │  │ " + output[1] + " │  │ " + output[2] + " │  │ " + output[3] + " │  │ " + output[4] + " │  │ " + output[5] + " │");
                        printAt(midcol, midrow + 2, "╰───╯  ╰───╯  ╰───╯  ╰───╯  ╰───╯  ╰───╯");
                        
                        return false;
                }
                else if (input == answer[i] && stop_direction == false)
                { // if user's input is correct //
                        output[i] = "✔";
                        printAt(midcol, midrow, "╭───╮  ╭───╮  ╭───╮  ╭───╮  ╭───╮  ╭───╮");
                        printAt(midcol, midrow + 1, "│ " + output[0] + " │  │ " + output[1] + " │  │ " + output[2] + " │  │ " + output[3] + " │  │ " + output[4] + " │  │ " + output[5] + " │");
                        printAt(midcol, midrow + 2, "╰───╯  ╰───╯  ╰───╯  ╰───╯  ╰───╯  ╰───╯");
                        if (i == 5)
                        {
                                stopCountdown = true;
                                return true;
                        }
                } else if (stop_direction == true) {
                        return false;
                }

        }
        return false;
}

bool Minigame::run()
{
        int winCols = getWinCols();
        int winRows = getWinRows();

        frame(winCols, winRows);

        bool result;

        bool countdown_result = true; // allow countdown function and the game run at the same time using thread //
        thread countdown_thread([&]()
                                { countdown_result = countdown(winCols, winRows); });
        result = direction();

        countdown_thread.join();

        string quote;
        int retrn = 0;
        if (result == 1) // win
        { 
                quote = "            Wohoooo nice!            ";
                retrn = 1;
        } else if (stop_direction == true) {
                
        } 
        else if (result == 0) { 
                quote = "         Oops! Wrong input :(         ";
        }
        else if (countdown_result == false) // wrong key
        { 
                quote = "           Oops! Time's out           ";
        }

        int midrow = (winRows - 5) / 2;
        int midcol_quote = (winCols - quote.size()) / 2;
        printAt(midcol_quote, midrow + 4, quote);
        this_thread::sleep_for(chrono::seconds(1));

        return retrn;
}

/*
     __
    '. \
     '- \
      / /_         .---.
     / | \\,.\/--.//    )
     |  \//        )/  /
      \  ' ^ ^    /    )____.----..  6
       '.____.    .___/            \._)
          .\/.                      )
           '\                       /
           _/ \/    ).        )    (
          /#  .!    |        /\    /      -Morfina-
          \  C// #  /'-----''/ #  /
       .   'C/ |    |    |   |    |mrf  ,
       \), .. .'OOO-'. ..'OOO'OOO-'. ..\(,
*/

/*
string cartoon
string bubble = "follow my steps!"

*/

/*
             ___.-~"~-._   __....__
           .'    `    \ ~"~        ``-.
          /` _      )  `\              `\
         /`  a)    /     |               `\
        :`        /      |                 \
   <`-._|`  .-.  (      /   .            `;\\          _.-~"~-.
    `-. `--'_.'-.;\___/'   .      .       | \\       ;`a)   )  `\
 _     /:--`     |        /     /        .'  \\     /      /    |~=-=--.
("\   /`/        |       '     '         /    ;`'._/ ,;_  |    /        ".
`\'\_/`/         .\     /`~`=-.:        /     ``.,__/ ` `| `"~`           \
  `._.'          /`\    |      `\      /(            .--'   /      /      |\
                /  /\   |        `Y   /  \          /  ____/\     |      / `"
          jgs  J  /  Y  |         |  /`\  \        /__/  |  |`-...-\    |
              /  |   |  |         |  |  |  |       '""   |  |      |`\  |
             "---"  /___|        /___|  /__|             /__|     /__/__|
                    '"""         '"""  '"""              '"""    '"""'""
*/

/*
                      /^--^\     /^--^\     /^--^\
                      \____/     \____/     \____/
                     /      \   /      \   /      \
KAT                 |        | |        | |        |
                     \__  __/   \__  __/   \__  __/
|^|^|^|^|^|^|^|^|^|^|^|^\ \^|^|^|^/ /^|^|^|^|^\ \^|^|^|^|^|^|^|^|^|^|^|^|
| | | | | | | | | | | | |\ \| | |/ /| | | | | | \ \ | | | | | | | | | | |
########################/ /######\ \###########/ /#######################
| | | | | | | | | | | | \/| | | | \/| | | | | |\/ | | | | | | | | | | | |
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
*/

/*
  ,-.       _,---._ __  / \
 /  )    .-'       `./ /   \
(  (   ,'            `/    /|
 \  `-"             \'\   / |
  `.              ,  \ \ /  |
   /`.          ,'-`----Y   |
  (            ;        |   '
  |  ,-.    ,-'         |  /
  |  | (   |        hjw | /
  )  |  \  `.___________|/
  `--'   `--'
*/