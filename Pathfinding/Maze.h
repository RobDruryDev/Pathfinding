#pragma once

#include "Grid.h"

/// T0-DO: 
/// This shouldn't be inherited it should be passed as a parameter
/// Maybe have a "NavMesh" base calss that A* owns? or maybe 
/// there should be a pathfinder class that owns both the data and the pathing
/// algo. Then the pathing algo can take a data structure as a param? 

class Maze : public Grid<uint8_t>
{
public:
	using Grid::Grid;

	const static uint8_t LEFT_WALL = 0x1;
	const static uint8_t BOTTOM_WALL = 0x2;
	const static uint8_t RIGHT_WALL = 0x4;
	const static uint8_t TOP_WALL = 0x8;
	const static uint8_t CLOSED_CELL = 0xF;

	bool IsBlocked(const Vector2I& src, const Vector2I& dst) const override;

private: 
	void RenderRow(int row, bool forceBottom);

public:

	void RenderGrid();
	void Generate();
};

