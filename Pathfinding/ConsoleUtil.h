#pragma once
#include <iostream>
#include <Windows.h>
#include "Vector2.h"

static Vector2I cell_size(4, 2);
static CONSOLE_SCREEN_BUFFER_INFO inputBuffInfo;
static COORD ToConsolePos(int x, int y, int height)
{
	return { (short)(cell_size.x / 2 + cell_size.x * x),
			 (short)max(cell_size.y / 2, cell_size.y * (height - y) - cell_size.y / 2) };
}

static void SetCursorPos(COORD&& pos)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

static void SetCursorPos(COORD& pos)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

static void RenderChar(COORD&& pos, char c)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	_STD cout << c;
}