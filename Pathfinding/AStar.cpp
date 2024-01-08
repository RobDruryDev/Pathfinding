
#include<queue>
#include "AStar.h"
#include "Maze.h"

float AStar::CalculateHeuristic(const AStarNode& node, const AStarNode& dest) const
{
    return Distance(node.pos, dest.pos);
}

Path AStar::BuildPath(const Vector2F& start, const Vector2F& dest, const vector<AStarNode>& nodes) const
{
    deque<Vector2F> pathQueue;
    pathQueue.emplace_front(dest);

    AStarNode current = nodes[dest.x + dest.y * _maze->GetWidth()];
    while (current.pos != current.parentPos)
    {
        current = nodes[current.parentPos.x + current.parentPos.y * _maze->GetWidth()];
        pathQueue.emplace_front(current.pos);
    }
    
    pathQueue.emplace_front(start);
    return Path(pathQueue.cbegin(), pathQueue.cend());
}

Path AStar::FindPath(const Vector2F& startPos, const Vector2F& destPos)
{
    if (!_maze->IsValid(startPos) || !_maze->IsValid(destPos) || startPos == destPos)
    {
        return {};
    }

    vector<AStarNode> nodes(_maze->GetSize());
    std::fill(nodes.begin(), nodes.end() - 1, AStarNode::Default());

    vector<bool> closedSet(_maze->GetSize());
    priority_queue<AStarNode> openSet;
    std::fill(closedSet.begin(), closedSet.end() - 1, false);

    AStarNode& dest = nodes[destPos.x + destPos.y * _maze->GetWidth()];
    AStarNode& start = nodes[startPos.x + startPos.y * _maze->GetWidth()];
    start = { 0, 0, startPos, startPos};

    openSet.push(start);
    while (!openSet.empty())
    {
        const AStarNode& current = openSet.top();
        openSet.pop();

        closedSet[current.pos.x + current.pos.y * _maze->GetWidth()] = true;

        for (int i = -1; i <= 1; i++)
        {
            if (i == 0)
                continue;

            for (int j = -1; j <= 1; j++)
            {
                if (j == 0)
                    continue;

                Vector2I nextPos(current.pos.x + j, current.pos.y + i);
                if (_maze->IsBlocked(current.pos, nextPos) || closedSet[nextPos.x + nextPos.y * _maze->GetWidth()])
                    continue;

                AStarNode& neighbor = nodes[nextPos.x + nextPos.y * _maze->GetWidth()];
                if (dest.pos == nextPos)
                {
                    neighbor.parentPos = current.pos;
                    return BuildPath(startPos, destPos, nodes);
                }

                int neighborDist = current.distance + Distance(current.pos, nextPos);
                int neighborCost = neighborDist + CalculateHeuristic(current, dest);
                if (neighbor.cost > neighborCost)
                {
                    neighbor.cost = neighborCost;
                    neighbor.distance = neighborDist;
                    neighbor.parentPos = current.pos;
                    openSet.push(neighbor);
                }
            }
        }
    }

    return {};
}