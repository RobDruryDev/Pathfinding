#include "Maze.h"
#include "MathDefines.h"
#include "ConsoleUtil.h"

#include <ctime>
#include <stack>
#include <vector>
#include <unordered_set>

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

static _STD stack<_STD pair<int, int>> debug_path;

bool Maze::IsBlocked(const Vector2I& src, const Vector2I& dst) const
{
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

void Maze::Generate()
{
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
			debug_path.emplace(_STD make_pair(current, wall_id));

			uint8_t data = GetData(TO_VEC(current)) & ~(1 << (wall_id + 2 % 4));
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
}

void Maze::RenderRow(int row, bool forceBottom)
{
	for (int i = 0; i < _width; i++)
	{
		if (i == 0) _STD cout << " ";
		if ((GetData(i, row) & Maze::TOP_WALL) == Maze::TOP_WALL)
			_STD cout << "--- ";
		else
			_STD cout << "    ";
	}

	_STD cout << "\n";

	for (int i = 0; i < _width; i++)
	{
		if (i == 0)
		{
			if ((GetData(i, row) & Maze::LEFT_WALL) == Maze::LEFT_WALL)
				_STD cout << "|";
			else
				_STD cout << " ";
		}

		_STD cout << "   ";

		if ((GetData(i, row) & Maze::RIGHT_WALL) == Maze::RIGHT_WALL)
			_STD cout << "|";
		else
			_STD cout << " ";
	}

	_STD cout << "\n";

	for (int i = 0; forceBottom && i < _width; i++)
	{
		if (i == 0) _STD cout << " ";
		if ((GetData(i, row) & Maze::BOTTOM_WALL) == Maze::BOTTOM_WALL)
			_STD cout << "--- ";
		else
			_STD cout << "    ";
	}
}


void Maze::RenderGrid()
{
	for (int i = _height - 1; i >= 0; i--)
	{
		RenderRow(i, i == 0);
	}

	//auto current_cell = debug_path.top();
	//RenderChar(ToConsolePos(current_cell.first % _width, current_cell.first / _width, _height), 'o');
	//debug_path.pop();

	//while (debug_path.size() > 1)
	//{
	//	/*current_cell = debug_path.top();
	//	RenderChar(ToConsolePos(current_cell.first % _width, current_cell.first / _width, _height), 
	//			   current_cell.second % 2 == 1 ? '|' : '-');*/
	//	debug_path.pop();
	//}

	//current_cell = debug_path.top();
	//RenderChar(ToConsolePos(current_cell.first % _width, current_cell.first / _width, _height), 'x');
	//debug_path.pop();

	//SetCursorPos({ 0, 0 });
}