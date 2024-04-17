#ifndef LOADER_HPP_
#define LOADER_HPP_

#include <string>
#include <sstream>

using namespace std;

/**
 * @brief Represents the state of the game.
 */
struct State {
    int position[2] = {1, 1}; // row / col
    int** Map2D = nullptr; // 2D map array
    int mapWidth = 0; // width of the map
    int mapHeight = 0; // height of the map
    int winCols = 0; // size of window cols
    int winRows = 0; // size of window rows
    string timerPausedTime = "0.0"; // paused time of the timer

    /**
     * @brief Overloaded stream insertion operator to output the state to a file.
     * @param outputFile The output file stream.
     * @param gameState The state object to be outputted.
     * @return The output file stream.
     */
    friend ostream& operator<<(ostream& outputFile, const State& gameState) {
        outputFile << gameState.position[0] << " " << gameState.position[1] << endl;
        outputFile << gameState.winCols << " " << gameState.winRows << endl;
        outputFile << gameState.mapHeight << " " << gameState.mapWidth << endl;
        for (int i = 0; i < gameState.mapHeight; i++) {
            for (int j = 0; j < gameState.mapWidth; j++) {
                outputFile << gameState.Map2D[i][j] << " ";
            }
            outputFile << endl;
        }
        outputFile << gameState.timerPausedTime << endl;
        return outputFile;
    }

    /**
     * @brief Overloaded assignment operator to assign the state from a string.
     * @param fileContents The string containing the state information.
     * @return The reference to the updated state object.
     */
    State& operator=(const string& fileContents) {
    stringstream ss(fileContents);
    ss >> position[0] >> position[1];
    ss >> winCols >> winRows;
    ss >> mapHeight >> mapWidth;
    Map2D = new int*[mapHeight];
    for (int i = 0; i < mapHeight; i++) {
        Map2D[i] = new int[mapWidth];
        for (int j = 0; j < mapWidth; j++) {
            ss >> Map2D[i][j];
        }
    }
    ss >> timerPausedTime;
    return *this;
}
};

class Loader
{
    string file;

    public:
        State loadState();
        string saveStateToFile(State gameState);
        Loader();
        ~Loader();
};

#endif