

#include "Puzzle.h"
#include "SDL/defines.h"
#include "ImageReader.h"

int main(int argc, char** argv)
{
    try
    {
        bool qof = false; //quit on finish flag
        if (argc == 3 && argv[2][0] == '1') qof = true;

        LL_AI ai(argv[1]);
        ai.setQuitOnFinish(qof);

        return ai.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Error\n" << e.what() << "\n";
        system("pause");
    }

    return 0;
}


/// The "main" functions from down here were tests done during development that I didn't really feel like removing after shipping

int main__()
{
    Puzzle puzzle;

    puzzle.setGridSize(5, 5);

    repeat(x, 2)
    repeat(y, 2)
    {
        puzzle.addBlock(x, y);
        puzzle.addBlock(x+3, y);
        puzzle.addBlock(x+3, y+3);
        puzzle.addBlock(x, y+3);
    }
    puzzle.addBridge(Coord(2, 2));
    puzzle.addPair(Coord(2, 0), Coord(2, 4));
    puzzle.addPair(Coord(0, 2), Coord(4, 2));

    std::cout << puzzle << "\n";

    /*
    std::cout << puzzle.grid.isBridge(PuzzleCoord(2, 2)) << " is Bridge 2, 2\n";
    PuzzleCoord node(2, 2, 1);
    std::vector<PuzzleCoord> vetor;
    vetor = puzzle.grid.getNeighborsFor(node);
    std::cout << vetor << " vector\n";
    repeat(i, vetor.size())
    {
        std::cout << vetor.at(i) << " is " << puzzle.grid.isWalkable(vetor.at(i)) << " passable\n";
    }
    */

    int toSearch = 1;

    std::cout << puzzle.search(toSearch, false) << " search " << (char)(toSearch+'A') << "\n";

    Path path = puzzle.pairList.at(toSearch).path;

    std::cout << path << " path\n";

    std::map< PuzzleCoord , bool >::iterator it = puzzle.grid.passable.begin();

    for (; it != puzzle.grid.passable.end(); it++)
    {
        if (it->second)
            std::cout << it->first << " is " << (it->second?"passable":"not") << "\n";
    }

    it = puzzle.grid.passable.find(PuzzleCoord(2, 2, 1));
    if (it != puzzle.grid.passable.end())
    {
        std::cout << "there it is: " << it->first << "\n";
    }
    else std::cout << "nope\n";

    if (puzzle.solve()) std::cout << "Solved!\n";

    std::cout << puzzle << "\n";

    return 0;
}

int main1()
{
    Puzzle puzzle;

    puzzle.setGridSize(5, 5);

    if (0)
    {
        puzzle.addPair(Coord(2, 0), Coord(0, 1)); //A
        puzzle.addPair(Coord(3, 0), Coord(4, 3)); //B
        puzzle.addPair(Coord(0, 2), Coord(2, 2)); //C
        puzzle.addPair(Coord(3, 2), Coord(1, 3)); //D
        puzzle.addPair(Coord(3, 3), Coord(4, 4)); //E
    }
    else if (0)
    {
        puzzle.addPair(Coord(4, 0), Coord(0, 1)); //A
        puzzle.addPair(Coord(1, 1), Coord(3, 1)); //B
        puzzle.addPair(Coord(4, 1), Coord(1, 4)); //C
        puzzle.addPair(Coord(2, 2), Coord(0, 4)); //D
        puzzle.addPair(Coord(3, 2), Coord(2, 3)); //E
        puzzle.addBlock(Coord(1, 3));
    }
    else if (0)
    {
        puzzle.addPair(Coord(3, 0), Coord(0, 1)); //A
        puzzle.addPair(Coord(4, 0), Coord(2, 1)); //B
        puzzle.addPair(Coord(1, 1), Coord(3, 3)); //C
        puzzle.addPair(Coord(0, 2), Coord(3, 4)); //D
        puzzle.addPair(Coord(2, 2), Coord(4, 4)); //E
    }
    else if (0)
    {
        puzzle.setGridSize(6, 6);
        puzzle.addPair(Coord(2, 0), Coord(0, 2)); //A
        puzzle.addPair(Coord(3, 0), Coord(5, 3)); //B
        puzzle.addPair(Coord(1, 1), Coord(3, 5)); //C
        puzzle.addPair(Coord(3, 2), Coord(1, 4)); //D
        puzzle.addPair(Coord(2, 3), Coord(4, 4)); //E
        puzzle.addPair(Coord(5, 4), Coord(4, 5)); //F
        puzzle.search(2, true);
        puzzle.search(0, true);
        puzzle.search(3, true);
        puzzle.search(4, true);
        puzzle.search(1, true);
        puzzle.search(5, true);
    }
    else if (0)
    {
        puzzle.setGridSize(6, 6);
        puzzle.addPair(Coord(2, 0), Coord(1, 5)); //A
        puzzle.addPair(Coord(3, 0), Coord(4, 1)); //B
        puzzle.addPair(Coord(5, 0), Coord(4, 4)); //C
        puzzle.addPair(Coord(2, 1), Coord(1, 4)); //D
        puzzle.addPair(Coord(3, 1), Coord(2, 5)); //E
        puzzle.addPair(Coord(4, 2), Coord(5, 5)); //F
    }
    else if (1)
    {
        puzzle.setGridSize(6, 6);
        puzzle.addPair(Coord(2, 0), Coord(1, 5)); // A
        puzzle.addPair(Coord(3, 0), Coord(4, 1)); // B
        puzzle.addPair(Coord(5, 0), Coord(4, 4)); // C
        puzzle.addPair(Coord(2, 1), Coord(1, 4)); // D
        puzzle.addPair(Coord(3, 1), Coord(2, 5)); // E
        puzzle.addPair(Coord(4, 2), Coord(5, 5)); // F
    }

    std::cout << puzzle << "\n";
    system("pause");

    std::vector<int> pairs;
    repeat(i, puzzle.pairList.size()) pairs.push_back(i);

    bool invert = false;

    while (true)
    {
        repeat(i, puzzle.pairList.size())
        {
            puzzle.pairList.at(i).path.clear();
        }

        repeat(i, pairs.size()) std::cout << char(pairs.at(i) + 'A') << ", ";
        if (invert) puts(" (i)"); else puts("");

        repeat(i, pairs.size())
        {
            if (!puzzle.search(pairs.at(i), true)) break;
        }
        //std::cout << puzzle << "\n";

        bool ok = true;
        repeat(i, puzzle.pairList.size())
            if (puzzle.pairList.at(i).hasPath() == false)
                ok = false;
        if (ok) break;

        ok = std::next_permutation(pairs.begin(), pairs.end());
        if (!ok)
        {
            if (invert)
            {
                printf("NO DICE \n");
                break;
            }
            else invert = true;
        }

    }

    std::cout << puzzle << "\n\n";

    puzzle.checkCompleteness();

    puts("");

    return 0;
}

int main2()
{
    Puzzle puzzle;
    puzzle.setGridSize(4, 4);

    puzzle.addPair(Coord(3, 0), Coord(0, 2));

    puzzle.addPair(Coord(3, 1), Coord(2, 3));

    puzzle.addPair(Coord(3, 2), Coord(3, 3));

    puzzle.addPair(Coord(1, 1), Coord(0, 3));

    std::cout << puzzle << "\n";
    puzzle.search(0);
    puzzle.search(1);
    puzzle.search(2);
    puzzle.search(3);
    std::cout << puzzle << "\n";

    puts("");

    return 0;

}



