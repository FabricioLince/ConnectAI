#ifndef BREADTHFIRST_H
#define BREADTHFIRST_H

#include "../CollectionWrapper/Queue.h"
#include <vector>
#include <map>
#include <algorithm>

class BreadthFirst
{
    public:
        template <typename Graph>
        static void getDirectionalMap(Graph& graph,
                                      typename Graph::Node start,
                                      std::map< typename Graph::Node,
                                                typename Graph::Node >& map,
                                      typename Graph::Node* goalToEarlyExit = NULL)
        {
            typedef typename Graph::Node Node;

            std::vector<Node> list;

            Queue<Node> frontier;
            frontier.put(start);
            std::map<Node, Node>& cameFrom = map;
            cameFrom[start] = start;

            while (!frontier.empty())
            {
                Node current = frontier.pop();

                if (goalToEarlyExit && current == *goalToEarlyExit) break;

                std::vector<Node> neighborList = graph.getNeighborsFor(current);
                for (unsigned i = 0; i < neighborList.size(); i++)
                {
                    const Node& next = neighborList.at(i);
                    if (cameFrom.count(next) == 0)
                    {
                        frontier.put(next);
                        cameFrom[next] = current;
                    }
                }
            }
        }
        template <typename Graph>
        static bool search
        (Graph& graph, typename Graph::Node start, typename Graph::Node goal,
         std::vector<typename Graph::Node>& path)
        {
            typedef typename Graph::Node Node;
            typedef std::vector<typename Graph::Node> Path;

            Queue<Node> frontier;
            frontier.put(start);
            std::map<Node, Node> cameFrom;

            getDirectionalMap(graph, start, cameFrom, &goal);

            if (cameFrom.find(goal) == cameFrom.end()) return false;

            Node current = goal;
            path.push_back(current);
            while (current != start)
            {
                current = cameFrom[current];
                path.push_back(current);
            }

            std::reverse(path.begin(), path.end());

            return true;
        }

        template <typename Graph>
        static std::vector<typename Graph::Node> search
        (Graph& graph, typename Graph::Node start, typename Graph::Node goal)
        {
            std::vector<typename Graph::Node> path;
            search(graph, start, goal, path);
            return path;
        }


};

#endif // BREADTHFIRST_H
