// Pathfinding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <set>
#include <vector>
#include <boost/timer/timer.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

#include "ConsoleUtil.h"
#include "Maze.h"
#include "AStar.h"
#include "Button.h"

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

    SDL_SetRenderDrawBlendMode(*r, SDL_BLENDMODE_BLEND);

    return true;
}

bool TryGenEndPointTex(SDL_Renderer* r, SDL_Texture** out_tex)
{
    constexpr int tex_width = 32;
    constexpr int tex_height = 32;
    *out_tex = SDL_CreateTexture
    (
        r, 
        SDL_PIXELFORMAT_ARGB1555, 
        SDL_TEXTUREACCESS_STATIC, 
        tex_width, tex_height
    );

    if (!*out_tex)
    {
        _STD cout << "SDL Create Tex Error: " << SDL_GetError() << _STD endl;
        return false;
    }

    SDL_SetTextureBlendMode(*out_tex, SDL_BLENDMODE_BLEND);

    constexpr int padding = 7;
    int left = padding, right = tex_width - padding - 1;
    uint16_t* pixels = new uint16_t[tex_width * tex_height];    
    memset(pixels, 0, sizeof(uint16_t) * tex_width * tex_height);

    for (int i = padding; i < tex_height - padding; ++i)
    {
        pixels[left + i * tex_width] = 0b1000000000011111;
        pixels[right + i * tex_width] = 0b1000000000011111;

        ++left;
        --right;
    }

    bool success = SDL_UpdateTexture(*out_tex, NULL, pixels, sizeof(uint16_t) * tex_width) == 0;
    if (!success)
    {
        _STD cout << "SDL Update Tex Error: " << SDL_GetError() << _STD endl;
    }

    delete[] pixels;
    return success;
}

typedef uint8_t PathState;
constexpr PathState PATH_STATE_IDLE = 0;
constexpr PathState PATH_STATE_HAS_START = 0x1;
constexpr PathState PATH_STATE_HAS_END = 0x2;

int main()
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect viewport;
    if (!TryInitSDL(&window, &renderer, &viewport))
    {
        return 1;
    }

    if (TTF_Init() != 0)
    {
        return 1;
    }

    SDL_Texture* end_point_tex;
    if (!TryGenEndPointTex(renderer, &end_point_tex))
    {
        return 1;
    }
    
    static boost::timer::cpu_times last;
    static boost::timer::cpu_timer timer;
    static _STD shared_ptr<Maze> maze(new Maze(GRID_WIDTH, GRID_HEIGHT));
    static _STD unique_ptr<AStar> pathfinder(new AStar(maze));
    static Button test(SDL_FRect{ 700, 50, 100, 100 }, ".\\resources\\arial.ttf", "This is a test!");
    test.SetPadding(10, 10, 7, 7);
    
    bool ctrl_down = false;
    PathState state = PATH_STATE_IDLE;
    Vector2F start, dst;
    vector<SDL_FPoint> path; 

    maze->Generate(renderer);
    while (true)
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
                    ctrl_down = (key_ev->keysym.mod & SDL_KMOD_CTRL) != 0 && key_ev->state;
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                case SDL_EVENT_MOUSE_BUTTON_DOWN: 
                {
                    SDL_MouseButtonEvent* mb_ev = reinterpret_cast<SDL_MouseButtonEvent*>(&ev);
                    if (mb_ev->button == 1 && !mb_ev->state)
                    {
                        if ((state & PATH_STATE_HAS_END) != 0)
                        {
                            state = ctrl_down ? state & ~PATH_STATE_HAS_END : PATH_STATE_IDLE;
                        }

                        if ((state & PATH_STATE_HAS_START) != 0)
                        {
                            dst = maze->WindowToGridCoords(mb_ev->x, mb_ev->y);
                            if (maze->IsValid(dst))
                            {
                                state |= PATH_STATE_HAS_END;
                                Path p = pathfinder->FindPath(maze->WindowToGridCoords(start.x, start.y), dst);
                                dst = maze->GridToWindowCoords(dst.x, dst.y);

                                path.clear();
                                for (const Vector2F& coord : p)
                                {
                                    Vector2F win_coord = maze->GridToWindowCoords(coord.x, coord.y);
                                    path.emplace_back(SDL_FPoint
                                    { 
                                        win_coord.x + 16.f, 
                                        win_coord.y  + 16.f
                                    });
                                }
                            }
                        }
                        else
                        {
                            start = maze->WindowToGridCoords(mb_ev->x, mb_ev->y);
                            if (maze->IsValid(start))
                            {
                                state |= PATH_STATE_HAS_START;
                                start = maze->GridToWindowCoords(start.x, start.y);
                            }
                        }
                    }
                    break;
                }
                case SDL_EVENT_MOUSE_MOTION:
                {
                    SDL_MouseMotionEvent* mm_ev = reinterpret_cast<SDL_MouseMotionEvent*>(&ev);
                    test.SetHovered(test.Overlaps(Vector2F{ mm_ev->x, mm_ev->y }));
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
            SDL_RenderClear(renderer);

            maze->RenderGrid(renderer);
            if ((state & PATH_STATE_HAS_START) != 0)
            {
                SDL_FRect start_rect{ start.x, start.y, 32, 32 };
                SDL_RenderTexture(renderer, end_point_tex, NULL, &start_rect);
            }

            if ((state & PATH_STATE_HAS_END) != 0)
            {
                SDL_FRect dst_rect{ dst.x, dst.y, 32, 32 };
                SDL_RenderTexture(renderer, end_point_tex, NULL, &dst_rect);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderLines(renderer, path.data(), path.size());
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            }            

            test.Render(renderer);
            SDL_RenderPresent(renderer);
        }
        timer.stop();

       /* 
        _STD cout << _STD endl << "Pathfinding: ";
        _STD cin >> input;

        if (input == "size")
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