
#include "PathFinder/GridGraph.h"
#include "PathFinder/PathFinder/AStar.h"
#include "PathFinder/PathFinder/BreadthFirst.h"
#include "SDL/defines.h"
#include "Puzzle.h"

bool PuzzleCoord::isNeighbor(PuzzleCoord c)
{
    int dx = (x - c.x);
    int dy = (y - c.y);
    return ((dx==0 && abs(dy)==1) | (dy==0 && abs(dx)==1) );
}
inline PuzzleCoord operator- (const PuzzleCoord c1, const PuzzleCoord c2)
{
    return PuzzleCoord(c1.x-c2.x, c1.y-c2.y);
}
std::vector<PuzzleGraph::Node> PuzzleGraph::getNeighborsFor(PuzzleGraph::Node node)
{
    std::vector<Node> rt;

    if (!isWalkable(node)) return rt;

    if (isBridge(node))
    {
        if (node.level == 1) // upper bridge LEFT & RIGHT
        {
            pushNeighborOf(rt, node, RIGHT);
            pushNeighborOf(rt, node, LEFT);
        }
        else // below passage UP & DOWN
        {
            pushNeighborOf(rt, node, UP);
            pushNeighborOf(rt, node, DOWN);
        }

        return rt;
    }
    else ;// Not a bridge

    pushNeighborOf(rt, node, LEFT);

    pushNeighborOf(rt, node, RIGHT);

    pushNeighborOf(rt, node, DOWN);

    pushNeighborOf(rt, node, UP);

    return rt;
}

void PuzzleGraph::pushNeighborOf(std::vector<Node>& rt, Node node, DIR dir)
{
    int dx = dir == LEFT? -1 :dir == RIGHT? +1  :0;
    int dy = dir == UP?   -1 :dir == DOWN?  +1  :0;

    Node neighbor(node.x+dx, node.y+dy,
                  (isBridge(node.x+dx, node.y+dy) && dx)?1:0);
    if (isWalkable(neighbor))
    {
        rt.push_back(neighbor);
    }
}

bool PuzzleGraph::isBridge(int x, int y)
{
    Coord coord(x, y);
    std::map<Coord, bool>::iterator it = nodeIsBridge.find(coord);
    if (it == nodeIsBridge.end()) return false;
    return it->second;
}

Coord PuzzleGraph::calculateGridDimensions()
{
    Coord rt;
    std::map<PuzzleGraph::Node, bool>::iterator it = passable.begin();
    for (; it != passable.end(); it++)
    {
        if (it->first.x+1 > rt.x) rt.x = it->first.x+1;
        if (it->first.y+1 > rt.y) rt.y = it->first.y+1;
    }
    return rt;
}


void printPathNumbers(Path& path, int mapWidth)
{
    repeat(i, path.size())
        std::cout << path.at(i).x + (path.at(i).y * mapWidth) << ", ";
}

void Puzzle::setGridSize(unsigned width, unsigned height)
{
    grid.passable.clear();
    repeat(x, width)
    repeat(y, height)
        grid.passable[PuzzleCoord(x, y)] = true;
}

void Puzzle::addPair(Coord a1, Coord a2)
{
    MatchingPair A(pairList.size()+'A');
    A.a1 = a1;
    A.a2 = a2;
    pairList.push_back(A);
}
void Puzzle::addBlock(Coord coord)
{
    blockList.push_back(coord);
    grid.passable.erase(grid.passable.find(coord));
    //grid.passable[coord] = false;
}
void Puzzle::addBridge(Coord coord)
{
    bridgeList.push_back(coord);
    grid.passable.erase(grid.passable.find(coord));
    PuzzleCoord c(coord, 0);
    grid.passable[c] = true;
    c.level = 1;
    grid.passable[c] = true;

    grid.nodeIsBridge[c] = true;
}

bool Puzzle::search(unsigned pairIndex, bool invert, bool consume)
{
    if (pairIndex >= pairList.size()) return false;

    // reseting grid to blank
    std::map<PuzzleGraph::Node, bool>::iterator it = grid.passable.begin();
    for (; it != grid.passable.end(); it++)
        it->second = true;

    repeat(i, pairList.size())
    {
        MatchingPair& A = pairList.at(i);

        grid.passable[A.a1] = false;
        grid.passable[A.a2] = false;

        repeat(j, A.path.size())
        {
            grid.passable[A.path.at(j)] = false;
        }
    }

    MatchingPair& pair = pairList.at(pairIndex);
    grid.passable[pair.a1] = true;
    grid.passable[pair.a2] = true;

    if (invert)
        return BreadthFirst::search(grid, pair.a2, pair.a1, pair.path);
    else
        return BreadthFirst::search(grid, pair.a1, pair.a2, pair.path);
}

bool Puzzle::solve()
{
    std::vector<int> pairs;
    repeat(i, pairList.size()) pairs.push_back(i);

    bool invert = false;

    while (true)
    {
        repeat(i, pairList.size())
        {
            pairList.at(i).path.clear();
        }

        repeat(i, pairs.size())
        {
            if (!search(pairs.at(i), invert)) break;
        }

        bool ok = true;
        repeat(i, pairList.size())
            if (pairList.at(i).hasPath() == false)
                ok = false;
        if (ok) break;

        ok = std::next_permutation(pairs.begin(), pairs.end());
        if (!ok)
        {
            if (invert) break;
            else invert = true;
        }
    }

    return checkCompleteness();
}

bool Puzzle::checkCompleteness()
{
    bool ok = true;
    repeat(i, pairList.size())
        if (pairList.at(i).hasPath() == false)
        {
            std::cout << "Pair " << pairList.at(i).letter << " has no path\n";
            ok = false;
        }
    if (!ok) return false;

    repeat(i, pairList.size())
    {
        MatchingPair& A = pairList.at(i);

        grid.passable[A.a1] = false;
        grid.passable[A.a2] = false;

        repeat(j, A.path.size())
        {
            grid.passable[A.path.at(j)] = false;
        }
    }

    repeat(i, blockList.size())
    {
        grid.passable[blockList.at(i)] = false;
    }

    std::map<PuzzleGraph::Node, bool>::iterator it = grid.passable.begin();
    for (; it != grid.passable.end(); it++)
        if (it->second)
        {
            std::cout << "node at " << it->first << " no closed\n";
            return false;
        }

    return true;
}

inline bool operator == (const PuzzleCoord c1, const PuzzleCoord c2)
{
    return c1.x == c2.x && c1.y == c2.y && c1.level==c2.level;
}
inline bool operator < (const PuzzleCoord c1, const PuzzleCoord c2)
{
    if (c1.y == c2.y)
    {
        if (c1.x == c2.x)
        {
            return c1.level < c2.level;
        }
        return c1.x < c2.x;
    }
    return c1.y < c2.y;
}
inline bool operator != (const PuzzleCoord c1, const PuzzleCoord c2)
{
    return !(c1==c2);
}

inline std::ostream& operator<<(std::ostream& out, const PuzzleCoord& coord)
{
    return out << "(" << coord.x << ", " << coord.y << ": " << coord.level << ")";
}

std::ostream& operator << (std::ostream& out, Puzzle& puzzle)
{
    PuzzleGraph& graph = puzzle.grid;
    Coord dim = graph.calculateGridDimensions();
    if (dim.x > 70) return out << "infeaseble: grid width bigger than screen\n";
    out << "   ";
    for(int i = 0; i < dim.x; i++) out << i%10;
    out << "\n";
    for (int y = 0; y < dim.y; y++)
    {
        out << y%10 << ": ";
        for (int x = 0; x < dim.x; x++)
        {
            PuzzleCoord c(x, y);
            bool printed = false;

            repeat(i, puzzle.pairList.size())
            {
                MatchingPair& pair = puzzle.pairList.at(i);
                if (pair.a1 == c || pair.a2 == c)
                {
                    out << pair.letter;
                    printed = true;
                    break;
                }
                repeat(j, pair.path.size())
                {
                    if (pair.path.at(j) == c)
                    {
                        out << (char)(pair.letter - 'A' + 'a');
                        printed = true;
                        i = puzzle.pairList.size();
                        break;
                    }
                }
            }
            if (!printed) // try to print Block
            {
                //out << (char)((graph.isWalkable(c))?' ':'X');
                repeat(i, puzzle.blockList.size())
                {
                    if (puzzle.blockList.at(i) == c)
                    {
                        out << "X";
                        printed = true;
                        break;
                    }
                }
            }

            if (!printed) // try to print bridge
            {
                repeat(i, puzzle.bridgeList.size())
                {
                    if (puzzle.bridgeList.at(i) == c)
                    {
                        out << "Z";
                        printed = true;
                    }
                }
            }

            if (!printed) out << " ";

        }
        out << "\n";
    }
    return out;
}

