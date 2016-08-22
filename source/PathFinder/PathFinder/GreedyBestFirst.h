#ifndef GREEDY_BEST_FIRST_H
#define GREEDY_BEST_FIRST_H

#include "../CollectionWrapper/PriorityQueue.h"

#include <iostream>
#include <map>
#include <algorithm>

class GreedyBestFirst
{
    public:
        template <typename Graph>
        static bool search(Graph& graph,
                           typename Graph::Node start,
                           typename Graph::Node goal,
                           std::vector<typename Graph::Node>& path)
        {
            typedef typename Graph::Node Node;
            typedef std::vector<typename Graph::Node> Path;

            PriorityQueue<Node> frontier;
            frontier.put(start, 0);

            std::map<Node, Node> cameFrom;
            cameFrom[start] = 0;
            bool foundGoal = false;

            int visited = 0;

            while (!frontier.empty())
            {
                Node current = frontier.pop();

                if (current == goal)
                {
                    foundGoal = true;
                    break;
                }

                visited++;

                std::vector<Node> neighborList
                    = graph.getNeighborsFor(current);
                for (unsigned i = 0; i < neighborList.size(); i++)
                {
                    const Node& next = neighborList.at(i);

                    if (cameFrom.count(next) == 0)
                    {
                        int priority = graph.heuristic(next, goal);
                        frontier.put(next, priority);
                        cameFrom[next] = current;
                    }
                }
            }

            if (!foundGoal) return false;

            Node current = goal;
            path.push_back(current);
            while (current != start)
            {
                current = cameFrom[current];
                path.push_back(current);
            }

            std::cout << visited << " nodes visited\n";

            std::reverse(path.begin(), path.end());

            return true;
        }
        template <typename Graph>
        static std::vector<typename Graph::Node>
        search(Graph& graph,
               typename Graph::Node start,
               typename Graph::Node goal)
        {
            std::vector<typename Graph::Node> path;
            GreedyBestFirst::search(graph, start, goal, path);
            return path;
        }
};

#endif // GREEDY_BEST_FIRST_H


