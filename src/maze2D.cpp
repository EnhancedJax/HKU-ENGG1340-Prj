/**
 * @file maze2D.cpp
 * @brief Implementation file for the Maze2D class.
 * 
 * This file contains the implementation of the Maze2D class, which represents a 2D maze.
 * The class provides methods for generating and printing the maze, as well as editing the maze map.
 * It also includes a destructor to clean up dynamically allocated memory.
 * 
 * Attribution: https://github.com/138paulmiller/PyMaze/tree/master
 * Sampled logic for maze generation (only) and converted to C++ for use.
 */

#include "maze2D.hpp"
#include "disjoint.hpp"
#include "colors.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include <sstream>

using namespace std;

/* ----------- Constructor ---------- */

/**
 * @brief Constructs a Maze2D object with the specified width, height, seed, and map.
 * 
 * This constructor initializes the Maze2D object with the given width, height, seed, and map.
 * If a map is not provided, it generates a new maze using the Kruskal's algorithm.
 * The constructor also sets up the glyphs for different elements in the maze.
 * 
 * @param theWidth The width of the maze.
 * @param theHeight The height of the maze.
 * @param theSeed The seed for the random number generator.
 * @param theMap2D The 2D map of the maze (optional).
 */
Maze2D::Maze2D(int theWidth, int theHeight, int theSeed, int **theMap2D = nullptr)
{
    // Initialize member variables
    width = theWidth / 2;
    height = theHeight / 2;
    seed = theSeed;
    result_width = (theWidth % 2 == 0 ? theWidth + 1 : theWidth);
    result_height = theHeight;

    // Set up glyphs for different elements in the maze
    glyphs = {
        {9, COLOR_BLUE + "██" + COLOR_DEFAULT},                              // player
        {8, COLOR_BRIGHT_GARY + "██" + COLOR_DEFAULT},                       // tail
        {1, "  "},                                                           // space
        {0, "██"},                                                           // wall
        {2, COLOR_YELLOW + "⟋⟋" + COLOR_DEFAULT},                           // end
        {3, COLOR_BG_BRIGHT_MAGENTA + COLOR_WHITE + "❂❂" + COLOR_DEFAULT}, // portal
        {5, COLOR_RED + "██"},
    };

    // If the map is provided, use it
    if (theMap2D != nullptr)
    {
        isMapProvided = true;
        map2D = theMap2D;
    }
    else
    {
        // Create the 2D array for the MST algorithm
        grid2D = new int *[height];
        for (int i = 0; i < height; i++)
        {
            grid2D[i] = new int[width];
        }

        // Initialize the array
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                grid2D[i][j] = width * i + j;
            }
        }

        map2D = new int *[result_height];
        for (int i = 0; i < result_height; i++)
        {
            map2D[i] = new int[result_width];
        }

        // Generate the maze using Kruskal's algorithm
        kruskalize();
        generateMaze();
    }
}

/* -------------- Utils ------------- */

/**
 * @brief Prints the map of the maze.
 * 
 * This method prints the map of the maze to the console.
 */
void Maze2D::printMap()
{
    for (int r = 0; r < result_height; ++r)
    {
        for (int c = 0; c < result_width; ++c)
        {
            cout << glyphs[map2D[r][c]];
        }
        cout << endl;
    }
}

/**
 * @brief Edits the map of the maze at the specified position.
 * 
 * This method allows editing the map of the maze at the specified row and column position.
 * 
 * @param r The row position.
 * @param c The column position.
 * @param value The new value to be set at the specified position.
 */
void Maze2D::editMap(int r, int c, int value)
{
    map2D[r][c] = value;
}

/**
 * @brief Prints the transfers between cells in the maze.
 * 
 * This method prints the transfers between cells in the maze to the console.
 */
void Maze2D::printTransfers()
{
    for (auto const &x : transfers)
    {
        cout << x.first << ": ";
        for (auto const &y : x.second)
        {
            cout << y.first << " ";
        }
        cout << endl;
    }
}

/* ----------- Destructor ----------- */

/**
 * @brief Destructor for the Maze2D object.
 * 
 * This destructor cleans up dynamically allocated memory for the maze map and grid.
 */
Maze2D::~Maze2D()
{
    for (int c = 0; c < height; ++c)
    {
        delete[] map2D[c];
        if (!isMapProvided)
        {
            delete[] grid2D[c];
        }
    }
    delete[] map2D;
    if (!isMapProvided)
    {
        delete[] grid2D;
    }
}

/* ----------- Generators ----------- */

/**
 * @brief Generates the maze.
 * 
 * This method generates the maze by translating the generated "transfers" into a 2D array.
 * It loops through the grid and transfers, creating 2 rows based on the data in those variables per iteration of the loop.
 * It also assigns random end points and minigame portals to the 2D array.
 */
void Maze2D::generateMaze()
{
    int bC, bR;
    bR = 0;

    for (int r = 0; r < height; ++r)
    {
        ++bR;
        bC = 0;

        map2D[bR][bC] = 0;
        ++bC;
        if (r == 0)
        {
            map2D[bR][bC] = 9;
        }
        else
        {
            map2D[bR][bC] = 1;
        }
        ++bC;

        for (int c = 1; c < width; ++c)
        {
            if (transfers[grid2D[r][c]].find(grid2D[r][c - 1]) != transfers[grid2D[r][c]].end())
            {
                map2D[bR][bC] = 1;
            }
            else
            {
                map2D[bR][bC] = 0;
            }
            ++bC;
            map2D[bR][bC] = 1;
            ++bC;
        }
        map2D[bR][bC] = 0;

        ++bR;
        bC = 0;

        for (int c = 0; c < width; ++c)
        {
            map2D[bR][bC] = 0;
            ++bC;
            int key = grid2D[r][c];
            if (r + 1 < height && transfers[key].find(grid2D[r + 1][c]) != transfers[key].end())
            {
                map2D[bR][bC] = 1;
            }
            else
            {
                map2D[bR][bC] = 0;
            }
            ++bC;
        }
        map2D[bR][bC] = 0;
    }

    /*
    Then we assign random end points as well as
    minigame portals to the 2D array.
    */

    random_device rd;
    mt19937 g(rd()); // g is a random number generator, not a random number

    int exit_col, exit_row;
    while (true)
    {
        exit_col = g() % (width / 2 + 1) + width / 2; // generate numbers
        exit_row = g() % (height) + 1;
        if (g() % 2 + 1 == 1)
        { // flip a coin to decide if the exit should be at the bottom or right edge
            if (map2D[exit_row][result_width - 2] == 1)
            { // checks if the position is a valid exit position
                map2D[exit_row][result_width - 1] = 2;
                break;
            }
            else if (map2D[result_height - 2][exit_col] == 1)
            {
                map2D[result_height - 1][exit_col] = 2;
                break;
            }
        }
    }

    int portal_edge_margin = 2;
    float portal_rate = 0.4;

    vector<pair<int, int>> portal_candidates;
    for (int r = portal_edge_margin; r < result_height - portal_edge_margin; ++r)
    {
        for (int c = portal_edge_margin; c < result_width - portal_edge_margin; ++c)
        {
            int checking = map2D[r][c];
            if (checking == 1)
            {
                int wall_count = 0;
                vector<int> neighbors = {map2D[r][c + 1], map2D[r][c - 1], map2D[r + 1][c], map2D[r - 1][c]};
                for (int v : neighbors)
                {
                    if (v == 0)
                    {
                        ++wall_count;
                    }
                }
                if (wall_count >= 3)
                {
                    portal_candidates.push_back(make_pair(r, c));
                    // cout << r << " " << c << endl;
                }
            }
        }
    }

    int num_portals = static_cast<int>(portal_candidates.size() * portal_rate);

    // Note: shuffle takes a random number generator as a third argument
    shuffle(portal_candidates.begin(), portal_candidates.end(), g);                                     // shuffle the candidates
    vector<pair<int, int>> portals(portal_candidates.begin(), portal_candidates.begin() + num_portals); // // g is a random number generator, not a random number first n elements of the candidiates

    for (auto pos : portals)
    {
        map2D[pos.first][pos.second] = 3;
    }
}

/**
 * @brief Applies Kruskal's algorithm to generate the minimum spanning tree (MST) of the maze.
 * 
 * This method applies Kruskal's algorithm to generate the minimum spanning tree (MST) of the maze.
 * It creates a disjoint set for each cell in the maze and initializes the transfer dictionary for each key.
 * It then randomly selects edges and adds them to the MST until the number of edges reaches the number of cells minus one.
 */
void Maze2D::kruskalize()
{
    // edge = ((r1, c1), (r2, c2) // flip a coin to decide if the exit should be at the bottom or right edge) such that grid[r][c] = key
    vector<pair<pair<int, int>, pair<int, int>>> edges_ordered;

    // First add all neighboring edges into a list
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < width; ++c)
        {
            pair<int, int> cell = make_pair(c, r);
            pair<int, int> left_cell = make_pair(c - 1, r);
            pair<int, int> down_cell = make_pair(c, r - 1);
            vector<pair<pair<int, int>, pair<int, int>>> near;

            // if not a boundary cell, add edge, else ignore
            if (c > 0)
            {
                near.push_back(make_pair(left_cell, cell));
            }
            if (r > 0)
            {
                near.push_back(make_pair(down_cell, cell));
            }
            edges_ordered.insert(edges_ordered.end(), near.begin(), near.end());
        }
    }

    // Seed the random value
    srand(seed);
    vector<pair<pair<int, int>, pair<int, int>>> edges;

    // Shuffle the ordered edges randomly into a new list
    while (!edges_ordered.empty())
    {
        int index = rand() % edges_ordered.size();
        edges.push_back(edges_ordered[index]);
        edges_ordered.erase(edges_ordered.begin() + index);
    }

    DisjointSet disjoint_set;
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < width; ++c)
        {
            // the key is the cell                                     // shuffle the candidatess unique id
            int key = grid2D[r][c];
            // create the singleton
            disjoint_set.make_set(key);
            // initialize the keys transfer dict
            transfers[key] = unordered_map<int, bool>();
        }
    }

    int edge_count = 0;

    int key_count = grid2D[height - 1][width - 1]; // last key
    while (edge_count < key_count)
    {
        // get next edge ((r1, c1), (r2,c2))
        auto edge = edges.back();
        edges.pop_back();
        // edge = ((r1, c1), (r2, c2) // flip a coin to decide if the exit should be at the bottom or right edge) such that grid[r][c] = key
        // get the sets for each vertex in the edge
        int key_a = grid2D[edge.first.second][edge.first.first];
        int key_b = grid2D[edge.second.second][edge.second.first];
        int set_a = disjoint_set.find(key_a);
        int set_b = disjoint_set.find(key_b);
        // if they are not in the same set they are not in the
        // same region in the maze
        if (set_a != set_b)
        {
            // add the transfer between the cells,
            // graph is undirected and will search
            // [a][b] or [b][a]
            edge_count++;
            transfers[key_a][key_b] = true;
            transfers[key_b][key_a] = true;
            disjoint_set.union_sets(set_a, set_b);
        }
    }
}