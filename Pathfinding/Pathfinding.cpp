// Pathfinding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES

#include <Windows.h>
#include <memory>
#include <iostream>
#include <math.h>
#include <set>

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

constexpr int GRID_WIDTH = 6;
constexpr int GRID_HEIGHT = 6;

int main()
{
    static string input; 
    static shared_ptr<Maze> grid(new Maze(GRID_WIDTH, GRID_HEIGHT));
    grid->Generate();
    /*static unique_ptr<AStar> pathfinder(new AStar(grid));*/

    while (ExitCommands.find(input) == ExitCommands.end())
    {
        grid->RenderGrid();
        cin >> input;
        ToLower(input);
    }

    return 0;
}