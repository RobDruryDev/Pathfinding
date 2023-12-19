#pragma once

#include "Grid.h"

class Maze : Grid<uint8_t>
{
public:
	using Grid::Grid;

	const static uint8_t LEFT_WALL = 0x1;
	const static uint8_t BOTTOM_WALL = 0x2;
	const static uint8_t RIGHT_WALL = 0x4;
	const static uint8_t TOP_WALL = 0x8;
	const static uint8_t CLOSED_CELL = 0xF;

private: 
	void RenderRow(int row, bool forceBottom);

public:
	void RenderGrid();
	void Generate();
};

