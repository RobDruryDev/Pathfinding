// Pathfinding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES

#include <Windows.h>
#include <memory>
#include <iostream>
#include <math.h>
#include <set>

#include "Grid.h"
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

constexpr int GRID_WIDTH = 4;
constexpr int GRID_HEIGHT = 4;
void RenderRow(bool forceRowBottom)
{
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        cout << " --- ";
    }

    cout << "\n";

    for (int i = 0; i < GRID_WIDTH; i++)
    {
        cout << "|   |";
    }

    cout << "\n";

    for (int i = 0; forceRowBottom && i < GRID_WIDTH; i++)
    {
        cout << " --- ";
    }
}

void RenderGrid()
{
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        RenderRow(GRID_HEIGHT - i == 1);
    }
}

int main()
{
    static string input; 
    static shared_ptr<Grid<uint8_t>> grid(new Grid<uint8_t>(GRID_WIDTH, GRID_HEIGHT));
    static unique_ptr<AStar> pathfinder(new AStar(grid));

    while (ExitCommands.find(input) == ExitCommands.end())
    {
        RenderGrid();
        cin >> input;
        ToLower(input);
    }

    return 0;
}