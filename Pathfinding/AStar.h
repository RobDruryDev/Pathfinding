#pragma once

#include <memory>
#include <vector>

#include "Vector2.h"

using namespace std;

template<class GridT> class Grid;
typedef vector<Vector2F> Path;

class AStar
{
public:
    AStar(shared_ptr<Grid<uint8_t>> g) { _grid = g; }

private:

    struct AStarNode
    {
        float cost;
        float distance;
        Vector2I pos;
        Vector2I parentPos;

        bool operator<(const AStarNode& rhs) const
        {
            return cost < rhs.cost;
        }

        static AStarNode Default()
        {
            return
            {
                FLT_MAX,
            };
        }
    };

    shared_ptr<Grid<uint8_t>> _grid;

    float CalculateHeuristic(const AStarNode& node, const AStarNode& dest) const;
    Path BuildPath(const Vector2F& start, const Vector2F& dest, const vector<AStarNode>& nodes) const;

public:
    Path FindPath(const Vector2F& start, const Vector2F& dest);
};

