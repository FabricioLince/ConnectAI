#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

template < typename N >
class GraphInterface /// For BreadthFirst
{
    public:
        typedef N Node;

        virtual std::vector<Node> getNeighborsFor(Node node)=0;

        virtual ~GraphInterface(){}
};

template < typename N >
class GraphWithCostInterface /// For Dijkstra
: public GraphInterface<N>
{
    public:
        typedef N Node;
        virtual int costTo(Node from, Node to)=0;
};

template < typename N >
class GraphWithHeuristicInterface /// For GreedyBestFirst
: public GraphInterface<N>
{
    public:
        typedef N Node;
        virtual int heuristic(Node from, Node to)=0;
};

template < typename N >
class GraphWithCostAndHeuristicInterface /// For A*
: public GraphWithCostInterface<N>,
         GraphWithHeuristicInterface<N>
{
    public:
        typedef N Node;
};



#endif // GRAPH_H
