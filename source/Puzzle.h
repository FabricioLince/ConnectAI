#ifndef PUZZLE_H
#define PUZZLE_H

#include <vector>
#include "PathFinder/GridGraph.h"

class PuzzleCoord : public Coord
{
    public:
        PuzzleCoord(int x=0, int y=0, int lv = 0)
        : Coord(x, y), level(lv)
        {}
        PuzzleCoord(Coord&c, int lv = 0)
        : Coord(c), level(lv)
        {}
        bool isNeighbor(PuzzleCoord c);

        int level;
};
bool operator == (const PuzzleCoord c1, const PuzzleCoord c2);
bool operator < (const PuzzleCoord c1, const PuzzleCoord c2);
bool operator != (const PuzzleCoord c1, const PuzzleCoord c2);
std::ostream& operator<<(std::ostream& out, const PuzzleCoord& coord);

class PuzzleGraph : public GraphWithCostAndHeuristicInterface< PuzzleCoord >
{
    public:
        int size;
        std::map< PuzzleCoord , bool > passable;
        std::map< Coord, bool > nodeIsBridge;
        enum DIR
        {
            UP, DOWN, LEFT, RIGHT
        };

        std::vector<Node> getNeighborsFor(Node node);

        void pushNeighborOf(std::vector<Node>& rt, Node node, DIR dir);

        inline bool isWalkable(Node node) {
            return (passable.count(node) && passable[node]);
        }
        inline bool isWalkable(int x, int y) { return isWalkable(Node(x, y)); }

        inline bool isBridge(Node node) { return isBridge(node.x, node.y); }
        bool isBridge(int x, int y);

        inline int costTo(Node from, Node to) { return 1; }

        inline int heuristic(Node from, Node to) {
            return abs(from.x - to.x) + abs(from.y - to.y);
        }

        Coord calculateGridDimensions();
};

typedef std::vector<PuzzleCoord> Path;

class MatchingPair
{
    public:
        MatchingPair(char l = '\0')
        : letter(l){}

        PuzzleCoord a1;
        PuzzleCoord a2;
        char letter;
        Path path;
        inline bool hasPath() {return path.size()!=0;}
};

class Puzzle
{
    public:
        PuzzleGraph grid;
        std::vector<MatchingPair> pairList;
        std::vector<PuzzleCoord> blockList;
        std::vector<PuzzleCoord> bridgeList;

        void setGridSize(unsigned width, unsigned height);
        void addPair(Coord a1, Coord a2);
        void addBlock(Coord coord);
        inline void addBlock(int x, int y) {addBlock(Coord(x, y));}
        void addBridge(Coord coord);
        inline void addBridge(int x, int y) {addBridge(Coord(x, y));}

        bool search(unsigned pairIndex, bool invert=false, bool consume=false);

        bool solve();

        bool checkCompleteness();

};

std::ostream& operator << (std::ostream& out, Puzzle& puzzle);

template <typename T>
inline std::ostream& operator<<(std::ostream& out, std::vector<T>& vec)
{
    for (unsigned i = 0; i < vec.size(); i++) out << vec.at(i) << ", ";
    return out;
}

#endif
