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

bool TryInitSDL(SDL_Window** w, SDL_Renderer** r, SDL_Rect* vp)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
    {
        cout << "SDL Init Error: " << SDL_GetError() << endl;
        return false;
    }

    *w = SDL_CreateWindow("Pathfinding", 1280, 720,
        SDL_WINDOW_INPUT_FOCUS |
        SDL_WINDOW_MOUSE_FOCUS |
        SDL_WINDOW_RESIZABLE);

    if (!w)
    {
        cout << "SDL Window Error: " << SDL_GetError() << endl;
        return false;
    }

    SDL_RendererInfo info;
    *r = SDL_CreateRenderer(*w, NULL, 0);
    if (!r)
    {
        cout << "SDL Render Error: " << SDL_GetError() << endl;
        return false;
    }
    else if (SDL_GetRendererInfo(*r, &info) == 0)
    {
        _STD cout << "Current Renderer: " << info.name << _STD endl;
    }

    if (SDL_GetRenderViewport(*r, vp))
    {
        _STD cout << "SDL Viewport Error: " << SDL_GetError() << _STD endl;
        return false;
    }

    return true;
}

int main()
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect viewport;
    if (!TryInitSDL(&window, &renderer, &viewport))
    {
        return 1;
    }
    
    static boost::timer::cpu_times last;
    static boost::timer::cpu_timer timer;
    static _STD shared_ptr<Maze> maze(new Maze(GRID_WIDTH, GRID_HEIGHT));
    static _STD unique_ptr<AStar> pathfinder(new AStar(maze));
    Vector2F start, dst;

    maze->Generate(renderer);

    while (ExitCommands.find(input) == ExitCommands.end())
    {
        SDL_Event ev;
        if (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_EventType::SDL_EVENT_QUIT)
                break;

            switch (ev.type)
            {
                case SDL_EVENT_KEY_UP:
                case SDL_EVENT_KEY_DOWN:
                {
                    SDL_KeyboardEvent* key_ev = reinterpret_cast<SDL_KeyboardEvent*>(&ev);
                    bool toggle_full_screen = key_ev->keysym.scancode == SDL_SCANCODE_F &&
                        (key_ev->keysym.mod & SDL_KMOD_CTRL) != 0 &&
                        key_ev->state;
                    if (toggle_full_screen)
                    {
                        SDL_SetWindowFullscreen(window, (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) == 0);
                    }

                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                case SDL_EVENT_MOUSE_BUTTON_DOWN: 
                {
                    SDL_MouseButtonEvent* mb_ev = reinterpret_cast<SDL_MouseButtonEvent*>(&ev);
                    _STD cout << "fuck" << _STD endl;
                    break;
                }
            }
        }

        auto elapsed = timer.elapsed();
        float dt = (elapsed.wall - last.wall) / 1000000000.f;
        timer.resume();
        if (dt >= 0.016f)
        {
            last = elapsed;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            /*SDL_RenderLine(renderer, 0.f, .5f * viewport.h, .5f * viewport.w, .5f * viewport.h);
            SDL_RenderLine(renderer, .5f * viewport.w, .5f * viewport.h, .5f * viewport.w, .1f);*/
            /*SDL_RenderLine(renderer, .1f, 0.f, .1f, 40.f);
            SDL_RenderLine(renderer, .1f, 0.f, 40.1f, 0.f);
            SDL_RenderLine(renderer, 40.1f, 0.f, 40.1f, 40.f);
            SDL_RenderLine(renderer, 40.1f, 40.f, .1f, 40.f);*/
            maze->RenderGrid(renderer);
            SDL_RenderPresent(renderer);
        }
        timer.stop();

       /* maze->RenderGrid();
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
        }*/
    }

    return 0;
}