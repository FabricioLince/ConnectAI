#ifndef ASTAR_H
#define ASTAR_H

#include "../CollectionWrapper/PriorityQueue.h"

#include <map>
#include <algorithm>

//#define COUT
#ifdef COUT
#include <iostream>
#endif

class AStar
{
    public:
        template <typename Graph>
        static bool search
        (Graph& graph, typename Graph::Node start, typename Graph::Node goal,
         std::vector<typename Graph::Node>& path)
        {
            typedef typename Graph::Node Node;
            typedef std::vector<typename Graph::Node> Path;

            PriorityQueue<Node> frontier;
            frontier.put(start, 0);

            std::map<Node, Node> cameFrom;
            cameFrom[start] = start;

            std::map<Node, int> costSoFar;
            costSoFar[start] = 0;
            #ifdef COUT
            int visited = 0;
            #endif

            while (!frontier.empty())
            {
                Node current = frontier.pop();

                if (current == goal) break;

                #ifdef COUT
                visited++;
                #endif

                std::vector<Node> neighborList = graph.getNeighborsFor(current);
                for (unsigned i = 0; i < neighborList.size(); i++)
                {
                    const Node& next = neighborList.at(i);

                    int newCost = costSoFar[current] + graph.costTo(current, next);
                    if ((costSoFar.count(next) == 0) || (newCost < costSoFar[next]))
                    {
                        costSoFar[next] = newCost;
                        int priority = newCost + graph.heuristic(next, goal);
                        frontier.put(next, priority);
                        cameFrom[next] = current;
                    }
                }
            }

            if (cameFrom.count(goal) == 0) return false;

            Node current = goal;
            path.push_back(current);
            while (current != start)
            {
                current = cameFrom[current];
                path.push_back(current);
            }

            std::reverse(path.begin(), path.end());

            #ifdef COUT
            std::cout << "cost: " << costSoFar[goal] << "\n";
            std::cout << visited << " nodes visited\n";
            #endif

            return true;
        }
        template <typename Graph>
        static std::vector<typename Graph::Node> search
        (Graph& graph, typename Graph::Node start, typename Graph::Node goal)
        {
            std::vector<typename Graph::Node> path;
            AStar::search(graph, start, goal, path);
            return path;
        }
};

#endif // ASTAR_H


