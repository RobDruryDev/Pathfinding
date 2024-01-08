// Pathfinding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <set>
#include <iostream>
#include <string>

#include "ConsoleUtil.h"
#include "Maze.h"
#include "AStar.h"

using namespace std;

static const set<string> ExitCommands
{
    "q",
    "quit",
    "exit",
    "done"
};

static void ToLower(string& in)
{
    for (auto& c : in)
    {
        c = tolower(c);
    }
}

constexpr int GRID_WIDTH = 15;
constexpr int GRID_HEIGHT = 15;

static string input;

int main()
{
    static shared_ptr<Maze> maze(new Maze(GRID_WIDTH, GRID_HEIGHT));
    static unique_ptr<AStar> pathfinder(new AStar(maze));
    Vector2F start, dst;

    maze->Generate();
    while (ExitCommands.find(input) == ExitCommands.end())
    {
        maze->RenderGrid();
        _STD cout << _STD endl << "Pathfinding: ";
        _STD cin >> input;

        if (input == "s")
        {
            bool valid = getline(_STD cin, input, ',') && isdigit(input[1]);
            if (valid)
            {
                start.x = stoi(input, 0);
            }

            valid = valid && getline(_STD  cin, input) && isdigit(input[1]);
            if (valid)
            {
                start.y = stoi(input, 0);
            }
        }
        else if (input == "e")
        {
            Path p;
            bool valid = getline(_STD cin, input, ',') && isdigit(input[1]);
            if (valid)
            {
                dst.x = stoi(input, 0);
            }

            valid = valid && getline(_STD  cin, input) && isdigit(input[1]);
            if (valid)
            {
                dst.y = stoi(input, 0);
                p = pathfinder->FindPath(start, dst);
            }
        }
    }

    return 0;
}