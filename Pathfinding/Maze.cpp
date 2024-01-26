#include "Maze.h"
#include "MathDefines.h"
//#include "ConsoleUtil.h"

#include <ctime>
#include <stack>
#include <vector>
#include <unordered_set>
#include <SDL3/SDL_render.h>

#define SHUFFLE(ARR, LEN)	  \
for (int i = 0; i < LEN; i++) \
{							  \
	int j = rand() % LEN;	  \
	auto swap = ARR[i];		  \
	ARR[i] = ARR[j];		  \
	ARR[j] = swap;			  \
} 

#define FILL_RANGE(CONTAINER, START_VAL, RANGE)		\
for (int i = START_VAL; i < START_VAL + RANGE; i++) { CONTAINER.emplace_back(i); } 

#define FAST_REMOVE(CONTAINER, IDX)				  \
CONTAINER[IDX] = CONTAINER[CONTAINER.size() - 1]; \
CONTAINER.pop_back();							  \

#define TO_IDX(VEC) VEC.x + VEC.y * _width
#define TO_VEC(IDX) Vector2I(IDX % _width, IDX / _width)

#if _DEBUG
static _STD stack<_STD pair<Vector2I, int>> debug_path;
#endif

Maze::~Maze()
{
	if (_tex)
	{
		SDL_DestroyTexture(_tex);
		_tex = nullptr;
	}
}

bool Maze::IsBlocked(const Vector2I& src, const Vector2I& dst) const
{
	if (!IsValid(src) || !IsValid(dst))
		return true;

	static const float cosFortyFive = cosf(DegToRad(45.f));
	float angleBetween = Dot(Normalized<float>(dst - src), Vector2F::Up());
	if (abs(angleBetween) > cosFortyFive)
	{
		return angleBetween > 0 ? (GetData(src) & Maze::TOP_WALL) == Maze::TOP_WALL : 
								  (GetData(src) & Maze::BOTTOM_WALL) == Maze::BOTTOM_WALL;
	}

	angleBetween = Dot(Normalized<float>(dst - src), Vector2F::Right());
	return angleBetween > 0 ? (GetData(src) & Maze::RIGHT_WALL) == Maze::RIGHT_WALL : 
							  (GetData(src) & Maze::LEFT_WALL) == Maze::LEFT_WALL;
}

void Maze::Generate(SDL_Renderer* r)
{
#if _DEBUG
	debug_path = _STD stack<_STD pair<Vector2I, int>>();
#endif

	_STD fill(_data.begin(), _data.end(), Maze::CLOSED_CELL);
	static int indices[4] = { 0, 1, 2, 3 };
	static Vector2I neighbors[4]
	{
		Vector2I(-1, 0),
		Vector2I(0, -1),
		Vector2I(1, 0),
		Vector2I(0, 1),
	};

	srand((unsigned)time(0));
	static _STD vector<int> open_set;
	static _STD unordered_set<int> maze;

	open_set.clear();
	maze.clear();

	FILL_RANGE(open_set, 0, _width * _height);
	SHUFFLE(open_set, open_set.size());
	maze.emplace(open_set[0]);
	FAST_REMOVE(open_set, 0);
	
	auto walk_graph = [this]()
	{
		SHUFFLE(open_set, open_set.size());
		int current = open_set[0];
		FAST_REMOVE(open_set, 0);

		_STD stack<int> path;
		_STD unordered_set<int> visited;
		_STD unordered_set<int> blocked;

		visited.emplace(current);
		while (maze.find(current) == maze.end())
		{
			int i = 0;
			SHUFFLE(indices, 4);
			for (; i < 4; i++)
			{
				Vector2I next = TO_VEC(current) + neighbors[indices[i]];
				if (IsValid(next) && blocked.find(TO_IDX(next)) == blocked.end() && visited.find(TO_IDX(next)) == visited.end())
				{
					current = TO_IDX(next);
					break;
				}
			}
			
			if (visited.emplace(current).second)
			{
				path.emplace(indices[i]);
			}			
			else
			{
				visited.erase(current);
				blocked.emplace(current);
				current = TO_IDX((TO_VEC(current) - neighbors[path.top()]));
				path.pop();
			}
		}

		maze.insert(visited.begin(), visited.end());
		while (!path.empty())
		{
			int wall_id = path.top();
#if _DEBUG
			debug_path.emplace(_STD make_pair(TO_VEC(current), wall_id));
#endif

			uint8_t data = GetData(TO_VEC(current)) & ~(1 << ((wall_id + 2) % 4));
			SetData(TO_VEC(current), data);

			current = TO_IDX((TO_VEC(current) - neighbors[path.top()]));
			path.pop();

			// this is redundant refactor to reduce duplicated data
			// store edges instead of nodes???
			data = GetData(TO_VEC(current)) & ~(1 << wall_id);
			SetData(TO_VEC(current), data);
		}
	};

	while (maze.size() < _width * _height)
	{
		walk_graph();
	}

	GenTexture(r);
}

constexpr SDL_Rect cell_size{ 0, 0, 32, 32 };
void Maze::GenTexture(SDL_Renderer* r)
{
	const int tex_width = _width * cell_size.w;
	const int tex_height = _height * cell_size.h;

	if (!_tex)
	{
		_tex = SDL_CreateTexture
		(
			r, 
			SDL_PIXELFORMAT_ARGB1555, 
			SDL_TEXTUREACCESS_STATIC,
			tex_width, tex_height
		);
	}

	SDL_SetTextureBlendMode(_tex, SDL_BLENDMODE_BLEND);

	uint16_t* pixels = new uint16_t[tex_width * tex_height];
	memset(pixels, 0, sizeof(uint16_t) * tex_width * tex_height);

	for (int row = _height - 1; row >= 0; row--)
	{
		for (int i = 0; i < _width; ++i)
		{
			if ((GetData(i, row) & Maze::TOP_WALL) == Maze::TOP_WALL)
			{
				for (int j = 0; j < cell_size.w; ++j)
				{
					pixels[i * cell_size.w + j + (_height - 1 - row) * cell_size.h * tex_width] = (uint16_t)0b1111110000000000;
				}
			}
		}

		for (int i = 0; i < _width; i++)
		{
			if (i == 0)
			{
				if ((GetData(i, row) & Maze::LEFT_WALL) == Maze::LEFT_WALL)
				{
					for (int j = 0; j < cell_size.h; ++j)
					{
						pixels[i * cell_size.w + (j + (_height - 1 - row) * cell_size.h) * tex_width] = (uint16_t)0b1111110000000000;
					}
				}
			}

			if ((GetData(i, row) & Maze::RIGHT_WALL) == Maze::RIGHT_WALL)
			{
				for (int j = 0; j < cell_size.h; ++j)
				{
					pixels[i * cell_size.w + cell_size.w - 1 + (j + (_height - 1 - row) * cell_size.h) * tex_width] = (uint16_t)0b1111110000000000;
				}
			}
		}

		for (int i = 0; row == 0 && i < _width; i++)
		{
			if ((GetData(i, row) & Maze::BOTTOM_WALL) == Maze::BOTTOM_WALL)
			{
				for (int j = 0; j < cell_size.w; ++j)
				{
					pixels[i * cell_size.w + j + ((_height - 1 - row) * cell_size.h + cell_size.h - 1) * tex_width] = static_cast<uint16_t>(0b1111110000000000);
				}
			}
		}
	}

	SDL_UpdateTexture(_tex, NULL, pixels, sizeof(uint16_t) * tex_width);
	delete[] pixels;
}

void Maze::RenderGrid(SDL_Renderer* r)
{
	SDL_FRect render_rect{ 10.f, 10.f, (float)cell_size.w * _width, (float)cell_size.h * _height };
	SDL_RenderTexture(r, _tex, NULL, &render_rect);
}

Vector2F Maze::WindowToGridCoords(float x, float y)
{
	Vector2F pos
	(
		static_cast<int>((x - 10.f) / cell_size.w), 
		static_cast<int>(ceil(_height - 1 - (y - 10.f) / cell_size.h))
	);

	return pos;
}

Vector2F Maze::GridToWindowCoords(float x, float y)
{
	Vector2F pos
	(
		static_cast<int>(x * cell_size.w + 10.f),
		static_cast<int>((_height - y - 1) * cell_size.h + 10.f)
	);

	return pos;
}
