#ifndef IMG_READER_H
#define IMG_READER_H

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>

#include "SDL/Texture.h"
#include "SDL/Surface.h"
#include "SDL/WindowHolder.h"
#include "Puzzle.h"

class LL_AI
{
    int count;
    int squareWidth;
    std::string imgPath;
    Surface surface;
    SDL_Rect puzzleRect;
    std::vector<HSV> hsvList;
    int identifiedColors;
    Puzzle puzzle;

    Surface nodeTemplate;
    Surface bridgeTemplate;

    bool quitOnFinish;

    public:
        LL_AI(const char* imagePath);

        ~LL_AI();

        int run();

        void test();

        void setQuitOnFinish(bool b) { quitOnFinish = b; }

    private:

        int showResult();

        void scanSquares();

        int makePuzzle();

        void checkSquareType(int squareX, int squareY);

        void makeNode(int squareX, int squareY);

        int loadTemplates();
};


#endif
