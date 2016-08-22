#ifndef GRID_GRAP_H
#define GRID_GRAP_H

#include "Graph.h"
#include <iostream>
#include <map>

struct Coord
{
    Coord(int x_=0, int y_=0){x=x_;y=y_;}
    Coord(std::pair<int, int> pair)
    {
        x = pair.first;
        y = pair.second;
    }
    int x;
    int y;
};
inline std::ostream& operator << (std::ostream& out, const Coord c)
{
    return out << "(" << c.x << ", " << c.y << ")";
}
inline bool operator < (const Coord c1, const Coord c2)
{
    if (c1.y == c2.y) return c1.x < c2.x;
    else return c1.y < c2.y;
}
inline bool operator == (const Coord c1, const Coord c2)
{
    return c1.x == c2.x && c1.y == c2.y;
}
inline bool operator != (const Coord c1, const Coord c2)
{
    return !(c1 == c2);
}

class GridGraph : public GraphWithCostAndHeuristicInterface< Coord >
{
    public:
        std::map< Coord , bool > passable;
        std::map< Coord, int > cost;

        std::vector<Node> getNeighborsFor(Node node)
        {
            std::vector<Node> rt;

            if (!isWalkable(node)) return rt;

            if (        isWalkable(node.x+1, node.y))
                rt.push_back(Coord(node.x+1, node.y));

            if (        isWalkable(node.x-1, node.y))
                rt.push_back(Coord(node.x-1, node.y));

            if (        isWalkable(node.x, node.y+1))
                rt.push_back(Coord(node.x, node.y+1));

            if (        isWalkable(node.x, node.y-1))
                rt.push_back(Coord(node.x, node.y-1));

            return rt;
        }

        bool isWalkable(Node node)
        {
            return (passable.count(node) && passable[node]);
        }
        bool isWalkable(int x, int y)
        {
            return isWalkable(Coord(x, y));
        }

        int costTo(Node from, Node to)
        {
            if (cost.count(to) == 0)
            {
                cost[to] = 1;
            }
            return cost[to];
        }

        int heuristic(Node from, Node to)
        {
            return abs(from.x - to.x) + abs(from.y - to.y);
        }

        Coord calculateGridDimensions()
        {
            Coord rt;
            std::map<GridGraph::Node, bool>::iterator it = passable.begin();
            for (; it != passable.end(); it++)
            {
                if (it->first.x+1 > rt.x) rt.x = it->first.x+1;
                if (it->first.y+1 > rt.y) rt.y = it->first.y+1;
            }
            return rt;
        }
};
inline std::ostream& operator << (std::ostream& out, GridGraph& graph)
{
    Coord dim = graph.calculateGridDimensions();
    if (dim.x > 79) return out << "infeaseble: grid width bigger than screen\n";
    //out << " [" << dim.x << ", " << dim.y << "]\n";
    out << "   ";
    for(int i = 0; i < dim.x; i++) out << i%10;
    out << "\n";
    for (int y = 0; y < dim.y; y++)
    {
        out << y%10 << ": ";
        for (int x = 0; x < dim.x; x++)
        {
            Coord c(x, y);
            out << (char)((graph.isWalkable(c))?graph.costTo(c, c)+'0':'X');
            //out << (char)((graph.isWalkable(c))?' ':'X');
        }
        out << "\n";
    }
    return out;
}
inline GridGraph& operator << (GridGraph& graph, const char* s)
{
    char walk = ' ';
    char block = 'X';
    int x = 0, y = 0;
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == walk)
        {
            graph.passable[Coord(x, y)] = true;
        }
        else if (s[i] == block)
        {
            graph.passable[Coord(x, y)] = false;
            graph.passable.erase(Coord(x, y));
        }
        else if (s[i] == '\n')
        {
            y++;
            x=-1;
        }
        x++;
    }
    return graph;
}

#endif
