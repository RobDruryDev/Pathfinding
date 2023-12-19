#include "Maze.h"
#include <ctime>
#include <unordered_map>
#include <iostream>

#define SHUFFLE(ARR, LEN)	  \
for (int i = 0; i < LEN; i++) \
{							  \
	int j = rand() % LEN;	  \
	auto swap = ARR[i];		  \
	ARR[i] = ARR[j];		  \
	ARR[j] = swap;			  \
} 

void Maze::Generate()
{
	std::fill(_data.begin(), _data.end(), Maze::CLOSED_CELL);

	static std::unordered_map<int, Vector2I> maze;
	static int indices[4] = { 0, 1, 2, 3 };
	static Vector2I neighbors[4]
	{
		Vector2I(-1, 0),
		Vector2I(0, -1),
		Vector2I(1, 0),
		Vector2I(0, 1),
	};

	srand((unsigned)time(0));
	Vector2I start(rand() % (_width - 1), rand() % (_height - 1));
	maze.emplace(start.x + start.y * _width, start);
	
	auto walk_graph = [this]()
	{
		Vector2I current;
		do
		{
			current = Vector2I(rand() % (_width - 1), (rand() % (_height - 1)));
		} while (maze.find(current.x + current.y * _width) != maze.end());

		std::unordered_map<int, Vector2I> path;
		std::unordered_map<int, Vector2I> blocked;
		path.emplace(current.x + current.y * _width, current);

		while (maze.find(current.x + current.y * _width) == maze.end())
		{
			SHUFFLE(indices, 4);
			for (int i = 0; i < 4; i++)
			{
				Vector2I next = current + neighbors[indices[i]];
				if (IsValid(next) && blocked.find(next.x + next.y * _width) == blocked.end() && path.find(next.x + next.y * _width) == path.end())
				{
					uint8_t data = GetData(current) & ~(1 << indices[i]);
					SetData(current, data);

					data = GetData(next) & ~(1 << ((indices[i] - 2) % 4));
					SetData(next, data);

					current = next;
					break;
				}
			}
			
			if (!path.emplace(current.x + current.y * _width, current).second)
			{
				// this doesn't quite work we need to change current to point to the previous cell
				// Maybe just track path in a list? 
				blocked.emplace(current.x + current.y * _width, current);
				path.erase(current.x + current.y * _width);
			}

			
		}

		maze.insert(path.begin(), path.end());
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
		if (GetData(i, row) & Maze::TOP_WALL == Maze::TOP_WALL)
			std::cout << " --- ";
		else
			std::cout << "     ";
	}

	std::cout << "\n";

	for (int i = 0; i < _width; i++)
	{
		if (GetData(i, row) & Maze::LEFT_WALL == Maze::LEFT_WALL)
			std::cout << "|";
		else
			std::cout << " ";

		std::cout << "   ";

		if (GetData(i, row) & Maze::RIGHT_WALL == Maze::RIGHT_WALL)
			std::cout << "|";
		else
			std::cout << " ";
	}

	std::cout << "\n";

	for (int i = 0; forceBottom && i < _width; i++)
	{
		if (GetData(i, row) & Maze::BOTTOM_WALL == Maze::BOTTOM_WALL)
			std::cout << " --- ";
		else
			std::cout << "     ";
	}
}

void Maze::RenderGrid()
{
	for (int i = 0; i < _height; i++)
	{
		RenderRow(i, _height - i == 1);
	}
}