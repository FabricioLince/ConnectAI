#include "ImageReader.h"

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>

#include "SDL/Texture.h"
#include "SDL/Surface.h"
#include "SDL/WindowHolder.h"
#include <iostream>
#include "Puzzle.h"

const char* nameColor(int hue)
{
    if (hue < 15) return "red";
    if (hue < 30) return "orange";
    if (hue < 75) return "yellow";
    if (hue < 140) return "green";
    if (hue < 180) return "cyan";
    if (hue < 215) return "blue";
    if (hue < 250) return "dark blue";
    if (hue < 322) return "pink";
    return "red";
}

std::ostream& operator<<(std::ostream&out, const SDL_Rect& rect)
{
    return out << "(" << rect.x << ", " << rect.y << " : " << rect.w << ", " << rect.h << ")";
}
std::ostream& operator<<(std::ostream&out, Color& color)
{
    return out << "(" << (int)(color.r) << "," << (int)color.g << "," << (int)color.b << "," << (int)color.a << ")";
}
std::ostream& operator<<(std::ostream&out, HSV hsv)
{
    return out << "(" << (int)hsv.hue << "\247," << hsv.sat << "," << hsv.val << ")";
}

bool isBlack(Color color)
{
    return fromRGB(color).val < 0.05;
}
bool isGray(Color& color)
{
    return fromRGB(color).sat < 0.2;
}
bool hasSimilarSaturation(HSV& hsv1, HSV& hsv2, double tolerance = 0.2)
{
    double sat = (hsv1.sat - hsv2.sat);
    if (sat < 0.0) sat*=(-1); // abs() ABS() doesn't work here for some reason
    return sat < tolerance;
}
bool hasSimilarValue(HSV& hsv1, HSV& hsv2, double tolerance = 0.2)
{
    double val = hsv1.val - hsv2.val;
    if (val < 0.0) val*=(-1); // neither does it work here
    return val < tolerance;
}
bool hasSimilarHue(int hue1, int hue2, int tolerance = 10)
{
    return (abs(hue1 - hue2) < tolerance
     || abs(hue1 + 360 - hue2) < tolerance);
}
bool hasSimilarHue(HSV& hsv1, HSV& hsv2, int tolerance = 10)
{
    return hasSimilarHue(hsv1.hue, hsv2.hue, tolerance);
}
bool isSimilar(HSV& hsv1, HSV& hsv2)
{
    return
        hasSimilarHue(hsv1.hue, hsv2.hue) &&
        hasSimilarSaturation(hsv1, hsv2) &&
        hasSimilarValue(hsv1, hsv2) ;
}

LL_AI::LL_AI(const char* imagePath)
{
    if (imagePath == NULL)
        imagePath = "imgs/Connect_Tests/wp_ss_20160714_0048.png";
    imgPath = imagePath;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        throw std::runtime_error("Couldn't Init SDL");
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        throw std::runtime_error(IMG_GetError());
    }

    if (surface.load(imagePath) == false)
    {
        char err[32];
        sprintf(err, "Couldn't load %s", imagePath);
        throw std::runtime_error(err);
    }

    // Talvez seja necessaria uma inteligencia pra descobrir esses valores p/ printscreens com outras resoluções
    puzzleRect.x = 40;
    puzzleRect.y = 320;
    puzzleRect.w = 679 - 40;
    puzzleRect.h = 959 - 320;

    count = 0;
    squareWidth = 0;

    quitOnFinish = false;
}

LL_AI::~LL_AI()
{
    SDL_Quit();
    IMG_Quit();
}

int LL_AI::run()
{
    scanSquares();

    if (makePuzzle()) return 1;

    bool solved = puzzle.solve();
    if (solved) std::cout << "Solved!\n";

    std::cout << puzzle << "\n";

    return showResult();
}

void LL_AI::test()
{
    std::vector<PuzzleCoord> emptyCoordList;

    std::map<PuzzleCoord, bool>::iterator it = puzzle.grid.passable.begin();
    for (; it != puzzle.grid.passable.end(); it++)
    {
        if (it->second)
        {
            emptyCoordList.push_back(it->first);
        }
    }
    if (emptyCoordList.empty()) return;
    std::cout << "empty: " << emptyCoordList << "\n";

    std::vector<int> pathToChangeList;

    repeat(i, emptyCoordList.size())
    {
        PuzzleCoord emptyCoord = emptyCoordList.at(i);
        std::cout << emptyCoord << " is\n";
        repeat(j, puzzle.pairList.size())
        {
            MatchingPair& pair = puzzle.pairList.at(j);
            Path& path = pair.path;
            range(k, 1, path.size()-1)
            {
                if (emptyCoord.isNeighbor(path.at(k)))
                {
                    std::cout << "\tclose to " << pair.letter << "'s path";
                    std::cout << " at " << path.at(k);
                    std::cout << "\n";
                    pathToChangeList.push_back(j);
                    break;
                }
            }
        }
    }
    std::sort(pathToChangeList.begin(), pathToChangeList.end());
    std::vector<int>::iterator fin = std::unique(pathToChangeList.begin(), pathToChangeList.end());
    pathToChangeList.resize(std::distance(pathToChangeList.begin(), fin));
    std::cout << pathToChangeList << " needs change\n";

    Path prevPath = puzzle.pairList.at(0).path;

}

int LL_AI::showResult()
{
    bool solved = puzzle.checkCompleteness();

    SDL_Rect imageRect;
    imageRect.x = -(puzzleRect.x);
    imageRect.y = -(puzzleRect.y);
    I_Vector puzzleImageOffset(puzzleRect.x + imageRect.x, puzzleRect.y + imageRect.y);

    WindowHolder window;
    window.createWindow();
    window.setScreenResolution(puzzleRect.w, puzzleRect.h);

    Texture texture(window.getRenderer(), surface.get());
    texture.queryInfo(NULL, NULL, &imageRect.w, &imageRect.h);
    texture.render(NULL, &imageRect);

    SDL_Renderer* renderer = window.getRenderer()->getSDL_Renderer();
    if (!solved)
    {
        //mostrar circulos nos nós que foram detectados
        repeat(i, puzzle.pairList.size())
        {
            Color color = fromHSV(hsvList[i]);
            window.getRenderer()->setDrawColor(color);

            Coord& c = puzzle.pairList.at(i).a1;
            I_Vector vec(c.x*squareWidth + squareWidth/2 + puzzleImageOffset.x,
                         c.y*squareWidth + squareWidth/2 + puzzleImageOffset.y);
            filledCircleRGBA(renderer, vec.x, vec.y, 14, 0, 0, 0, 255);
            filledCircleRGBA(renderer, vec.x, vec.y, 10, color.r, color.g, color.b, 255);

            c = puzzle.pairList.at(i).a2;
            vec = I_Vector( c.x*squareWidth + squareWidth/2 + puzzleImageOffset.x,
                            c.y*squareWidth + squareWidth/2 + puzzleImageOffset.y);
            filledCircleRGBA(renderer, vec.x, vec.y, 14, 0, 0, 0, 255);
            filledCircleRGBA(renderer, vec.x, vec.y, 10, color.r, color.g, color.b, 255);
        }
    }

    SDL_Event event;
    bool running = true;
    int i = 0, j = 0;
    int iMax = puzzle.pairList.size(), jMax = puzzle.pairList.at(i).path.size();
    Color color = fromHSV(hsvList[i]);
    window.getRenderer()->setDrawColor(color);
    int timer = 0;
    I_Vector lastVec;
    while (running)
    {
        if (timer > (quitOnFinish?0:1) && i < iMax)
        {
            timer = 0;

            if (j >= 0)
            {
                if (j < jMax)
                {
                    Coord& c = puzzle.pairList.at(i).path.at(j);
                    I_Vector vec(c.x*squareWidth + squareWidth/2,
                                 c.y*squareWidth + squareWidth/2);
                    vec += puzzleImageOffset;
                    vec += I_Vector(c.x+2, c.y+2); // pequena correção para levar em conta as divisões entre quadrados
                    filledCircleRGBA(renderer, vec.x, vec.y, 10, color.r, color.g, color.b, 255);
                    if (j>0) thickLineRGBA(renderer, vec.x, vec.y, lastVec.x, lastVec.y,
                                        20, color.r, color.g, color.b, 255);
                    lastVec = vec;
                }
                else
                {
                    i++;
                    j=-2;
                }
                j++;
            }
            else if(i < iMax)
            {
                color = fromHSV(hsvList[i]);
                window.getRenderer()->setDrawColor(color);
                j=0;
                jMax = puzzle.pairList.at(i).path.size();
            }
        }
        else if (timer > 25)
        {
            if (quitOnFinish) running = false;
        }
        timer ++;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
        }
        window.getRenderer()->present();

    }

    if (quitOnFinish)
        return solved?0:1;
    return 0;
}

void LL_AI::scanSquares()
{
    bool inSquare = false;
    int y = 3;
    range(x, puzzleRect.x, puzzleRect.x + puzzleRect.w)
    {
        Color color = surface.getPixelColor(x, puzzleRect.y + y);
        if (isBlack(color))
        {
            inSquare = true;
        }
        else if (inSquare)
        {
            squareWidth = x - puzzleRect.x;
            count = puzzleRect.w/squareWidth;
            if (count < 4 || count > 20)
            {
                x = puzzleRect.x;
                y++;
            }
            else
                break;
        }
    }

    std::cout << "count " << count << "\nsquare width " << squareWidth << "\n";
}

int LL_AI::makePuzzle()
{
    identifiedColors = 0;
    hsvList.clear();
    puzzle.setGridSize(count, count);

    if (loadTemplates()) return 1;

    repeat(squareY, count)
    {
        repeat(squareX, count)
        {
            checkSquareType(squareX, squareY);
        }
    }

    std::cout << "\n\ngot " << identifiedColors << " identified colors\n\n";

    std::cout << puzzle << "\n";

    return 0;
}

void LL_AI::checkSquareType(int squareX, int squareY)
{
    int isNode = 0;
    int isBridge = 0;

    int initialX = puzzleRect.x + squareX*squareWidth + squareX;
    int initialY = puzzleRect.y + squareY*squareWidth + squareY;

    int blackPercent = 0;

    range(x, initialX, initialX+squareWidth)
    range(y, initialY, initialY+squareWidth)
    {
        Color color = surface.getPixelColor(x, y);
        int x_ = x - (initialX);
        int y_ = y - (initialY);
        HSV hsv1 = fromRGB(color);
        HSV hsv2;
        hsv2 = fromRGB(nodeTemplate.getPixelColor(x_, y_));
        if (hasSimilarValue(hsv1, hsv2))
        {
           isNode++;
        }

        hsv2 = fromRGB(bridgeTemplate.getPixelColor(x_, y_));
        if (hasSimilarValue(hsv1, hsv2))
        {
           isBridge++;
        }

        if (isBlack(color)) blackPercent++;
    }

    blackPercent = blackPercent*100/(squareWidth*squareWidth);
    static const char BLANK = ' ', BLOCK = 'X', NODE = 'A', BRIDGE = 'Z';
    char type = 0;
    int certainty = 100;

    if (blackPercent > 90) type = BLANK;
    else if (blackPercent < 5) type = BLOCK;
    else if (isNode > isBridge)
    {
        type = NODE;
        certainty = int(double(isNode)/double(squareWidth*squareWidth)*100);
    }
    else
    {
        type = BRIDGE;
        certainty = int(double(isBridge)/double(squareWidth*squareWidth)*100);
    }

    if (certainty < 50) /// Verify error
    {
        if (0) // but not right now
        {
            if (0)
            surface.rect_(initialX, initialY,
                          squareWidth, squareWidth,
                          0xff00);
            Surface s(squareWidth, squareWidth);
            s.draw(surface, -(initialX), -(initialY));
            char name[64];
            sprintf(name, "err_%d_%d.png", squareX, squareY);
            s.savePNG(name);
        }
    }

    if (type == NODE || type == BRIDGE) // logging
    {
        std::cout << "\n\nsquare " << squareX << ", " << squareY << " ";

        std::cout << "blackPercent: " << blackPercent << "% and ";

        if (1)
        std::cout
            << int(double(isNode)/double(squareWidth*squareWidth)*100)   << "% Node; "
            << int(double(isBridge)/double(squareWidth*squareWidth)*100) << "% Bridge; ";
    }

    switch (type)
    {
        case BLANK:
            //std::cout << "is blank ";
            break;
        case BLOCK:
            //std::cout << "is block ";
            puzzle.addBlock(Coord(squareX, squareY));
            break;
        case NODE:
            std::cout << "\nis node ";
            makeNode(squareX, squareY);
            break;
        case BRIDGE:
            std::cout << "is bridge ";
            puzzle.addBridge(Coord(squareX, squareY));
            break;
    }
}

void LL_AI::makeNode(int squareX, int squareY)
{
    int x = puzzleRect.x + squareX*squareWidth + squareWidth/2;
    int y = puzzleRect.y + squareY*squareWidth + squareWidth/2;
    HSV hsv[2];
    Color color;
    do // try to eliminate white line between double colors
    {
        color = surface.getPixelColor(x + squareWidth/5, y);
        hsv[0] = fromRGB(color);
        y--;
    }
    while (hsv[0].sat < 0.05);

    hsv[1] = hsv[0];
    bool has2 = false;

    int x_;
    int y_;
    bool foundColor = false;

    for(int m = -1; m <= 1; m++)
    for(int n = -1; n <= 1; n++)
    {
        x_ = x + m*(squareWidth/5);
        y_ = y + n*(squareWidth/5);
        //std::cout << "x_y: " << x_ << ", " << y_ << " ";
        hsv[1] = fromRGB(surface.getPixelColor(x_, y_));
        if (hsv[1].sat < 0.2) continue;
        //std::cout << "hue: " << hsv2.hue << "\n";
        if (hasSimilarHue(hsv[0], hsv[1], 15) == false)
        {
            has2 = true;
            m=2;
            n=2;
        }
    }

    if (has2 && hsv[0].sat < 0.5)
    {
        std::cout << "<" << hsv[0] << x_ << ", " << y_ << ">";
        x = puzzleRect.x + squareX*squareWidth + squareWidth/2;
        y = puzzleRect.y + squareY*squareWidth + squareWidth/2;
        int dx = x_ - x;
        int dy = y_ - y;
        x -= dx;
        y -= dy;
        std::cout << "<" << hsv[0] << x << ", " << y << ">";
        hsv[0] = fromRGB(surface.getPixelColor(x, y));
    }

    if (has2)
        std::cout << "\nHAS TWO "
        << (hsv[0]) << "(" << nameColor(hsv[0].hue) << ") and "
        << (hsv[1]) << "(" << nameColor(hsv[1].hue) << ")\n";
    else std::cout << hsv[0] << "(" << nameColor(hsv[0].hue) << ")";

    int nodeAmount = has2?2:1;

    repeat(nodeNo, nodeAmount)
    {
        repeat(i, hsvList.size())
        {
           if (isSimilar(hsv[nodeNo], hsvList[i])) // Same color
           {
               //std::cout << " on list ";
               std::cout << (char)(i+'A');
               foundColor = true;
               puzzle.pairList.at(i).a2 = PuzzleCoord(squareX, squareY);
               break;
           }
        }
        if (!foundColor)
        {
            hsvList.push_back(hsv[nodeNo]);
            std::cout << (char)(identifiedColors+'A');
            puzzle.addPair(Coord(squareX, squareY), Coord(0, 0));
            identifiedColors++;
        }
        foundColor = false;
        std::cout << " ";
    }
}

int LL_AI::loadTemplates()
{
    I_Vector templateSize(squareWidth, squareWidth);
    nodeTemplate.create(squareWidth, squareWidth);
    bridgeTemplate.create(squareWidth, squareWidth);
    {
        double z = squareWidth/126.0;
        Surface temp;
        temp.load("imgs/node_template.png");
        if (!temp.get()) return 1;
        nodeTemplate.draw(temp, 0, 0, z);
        temp.load("imgs/bridge_template.png");
        if (!temp.get()) return 1;
        bridgeTemplate.draw(temp, 0, 0, z);
    }
    return 0;
}








