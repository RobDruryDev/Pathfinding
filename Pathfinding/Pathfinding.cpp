// Pathfinding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <set>
#include <boost/timer/timer.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

#include "ConsoleUtil.h"
#include "Maze.h"
#include "AStar.h"

static const _STD set<_STD string> ExitCommands
{
    "q",
    "quit",
    "exit",
    "done"
};

static void ToLower(_STD string& in)
{
    for (auto& c : in)
    {
        c = tolower(c);
    }
}

template<class T>
static bool TryParseXY(_STD string& input, T& x_out, T& y_out)
{
    bool valid = getline(_STD cin, input, ',') && isdigit(input[1]);
    if (valid)
    {
        x_out = stoi(input, 0);
    }

    valid = valid && getline(_STD  cin, input) && isdigit(input[1]);
    if (valid)
    {
        y_out = stoi(input, 0);
    }

    return valid;
}

constexpr int GRID_WIDTH = 15;
constexpr int GRID_HEIGHT = 15;

static _STD string input;

int main()
{
    /*if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
    {
        cout << "SDL Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Pathfinding", 1280, 720,
        SDL_WINDOW_MOUSE_GRABBED |
        SDL_WINDOW_INPUT_FOCUS |
        SDL_WINDOW_MOUSE_FOCUS |
        SDL_WINDOW_KEYBOARD_GRABBED |
        SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        cout << "SDL Window Error: " << SDL_GetError() << endl;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL, 0);
    if (!renderer)
    {
        cout << "SDL Render Error: " << SDL_GetError() << endl;
    }*/

    //static boost::timer::cpu_times last;
    static _STD shared_ptr<Maze> maze(new Maze(GRID_WIDTH, GRID_HEIGHT));
    static _STD unique_ptr<AStar> pathfinder(new AStar(maze));
    Vector2F start, dst;

    maze->Generate();
    while (ExitCommands.find(input) == ExitCommands.end())
    {
        maze->RenderGrid();
        _STD cout << _STD endl << "Pathfinding: ";
        _STD cin >> input;

        if (input == "s")
        {
            TryParseXY(input, start.x, start.y);
        }
        else if (input == "e")
        {
            Path p;
            if (TryParseXY(input, dst.x, dst.y))
            {
                p = pathfinder->FindPath(start, dst);
                _STD cout << "Size: " << p.size() << _STD endl;
            }
        }
        else if (input == "size")
        {
            int width, height;
            TryParseXY(input, width, height);
            maze->Reset(width, height);
            maze->Generate();
        }
        else if (input == "save")
        {
            _STD ofstream save("save.dat", _STD ios::out | _STD ios::binary);
            save.clear();
            save << *maze;
            save.close();
        }
        else if (input == "load")
        {
            _STD ifstream load("save.dat", _STD ios::in | _STD ios::binary);
            load >> *maze;
            load.close();
        }
        else if (input == "gen" || input == "g")
        {
            maze->Generate();
        }
        /*SDL_Event ev;
        if (SDL_PollEvent(&ev))
        {

        }*/
        // this isnt the way to do this.. just update dynamic characters as needed. 
        /*auto elapsed = timer.elapsed();
        float dt = (elapsed.wall - last.wall) / 1000000000.f;
        timer.resume();
        if (dt >= 0.066f)
        {
            last = elapsed;
        }
        timer.stop();*/
    }

    return 0;
}