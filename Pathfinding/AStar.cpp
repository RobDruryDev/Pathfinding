
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

        static auto try_find_dest = [&](const Vector2I&& nextPos)
        {
            if (_maze->IsBlocked(current.pos, nextPos) || closedSet[nextPos.x + nextPos.y * _maze->GetWidth()])
                return false;

            AStarNode& neighbor = nodes[nextPos.x + nextPos.y * _maze->GetWidth()];
            if (dest.pos == nextPos)
            {
                neighbor.parentPos = current.pos;
                return true;
            }

            int neighborDist = current.distance + Distance(current.pos, nextPos);
            int neighborCost = neighborDist + CalculateHeuristic(current, dest);
            if (neighbor.cost > neighborCost)
            {
                neighbor.cost = neighborCost;
                neighbor.distance = neighborDist;
                neighbor.parentPos = current.pos;
                neighbor.pos = nextPos;
                openSet.push(neighbor);
            }

            return false;
        };

        for (int i = -1; i <= 1; i++)
        {
            if (try_find_dest(Vector2I(current.pos.x, current.pos.y + i)))
            {
                return BuildPath(startPos, destPos, nodes);
            }
        }

        for (int i = -1; i <= 1; i++)
        {
            if (try_find_dest(Vector2I(current.pos.x + i, current.pos.y)))
            {
                return BuildPath(startPos, destPos, nodes);
            }
        }
    }

    return {};
}