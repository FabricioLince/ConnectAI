#include "Renderer.h"
#include "Texture.h"
//#include "CommonFunctions.h"

LWindow::LWindow()
{
    window = NULL;
}
LWindow::LWindow(std::string title_, I_Vector position_, I_Vector resolution_, bool fullscreen_)
: title(title_), position(position_), resolution(resolution_), fullscreen(fullscreen_)
{
    if (!create(title, position, resolution, fullscreen))
    {
        //throw std::runtime_error("Couldn't Create Window: %s", SDL_GetError() );
        printf("Couldn't Create Window: %s", SDL_GetError());
        exit(1);
    }
}
bool LWindow::create(std::string title_, I_Vector position_, I_Vector resolution_, bool fullscreen_)
{
    title = title_;
    position = position_;
    resolution = resolution_;
    fullscreen = fullscreen_;
    window = SDL_CreateWindow(title.c_str(),
                                  position.x, position.y,
                                  resolution.x, resolution.y,
                                  (fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0));
    return (window);
}
SDL_Window* LWindow::getSDL_Window()
{
    return window;
}
void LWindow::destroy()
{
    if (window)
        SDL_DestroyWindow(window);
    window = NULL;
}
LWindow::~LWindow()
{
    destroy();
}
bool LWindow::isFullscreen()
{
    return fullscreen;
}
I_Vector LWindow::getResolution()
{
    return resolution;
}
const char* LWindow::getTitle()
{
    return title.c_str();
}
void LWindow::changeResolution(I_Vector newResolution)
{
    resolution = newResolution;
    SDL_SetWindowSize(getSDL_Window(), newResolution.x, newResolution.y);
}
void LWindow::changePosition(I_Vector newPosition)
{
    SDL_SetWindowPosition(window, newPosition.x, newPosition.y);
}
SDL_Rect Renderer::rect;

Renderer::Renderer()
{
    renderer = NULL;
}

Renderer::~Renderer()
{
    destroy();
}

bool Renderer::create(LWindow* window_, int index, Uint32 flags)
{
    destroy();
    window = window_;
    return ((renderer = SDL_CreateRenderer(window_->getSDL_Window(), index, flags))!=NULL);
}

void Renderer::destroy()
{
    if (renderer) SDL_DestroyRenderer(renderer);
    renderer = NULL;
}

bool Renderer::setDrawColor(Color color)
{
    return (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a));
}
bool Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return (!SDL_SetRenderDrawColor(renderer, r, g, b, a));
}

void Renderer::fillRect(const SDL_Rect* rect)
{
    if (renderer) /// Testar o que acontece caso eu use SDL_RenderFillRect(NULL, ...
        SDL_RenderFillRect(renderer, rect);
}
void Renderer::fillRect(const SDL_Rect* rect, Color color)
{
    if (!renderer) return;/// Testar o que acontece caso eu use SDL_RenderFillRect(NULL, ...

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    if (setDrawColor(color))
        SDL_RenderFillRect(renderer, rect);
}
void Renderer::fillRect(int x, int y, int w, int h)
{
    if (!renderer) return;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_RenderFillRect(renderer, &rect);
}
void Renderer::fillRect(int x, int y, int w, int h, Color color)
{
    if (!renderer) return;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Renderer::drawRect(const SDL_Rect* rect)
{
    if (renderer) /// Testar o que acontece caso eu use SDL_RenderFillRect(NULL, ...
        SDL_RenderDrawRect(renderer, rect);
}
void Renderer::drawRect(const SDL_Rect* rect, Color color)
{
    if (!renderer) return;/// Testar o que acontece caso eu use SDL_RenderFillRect(NULL, ...

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    if (setDrawColor(color))
        SDL_RenderDrawRect(renderer, rect);
}
void Renderer::drawRect(int x, int y, int w, int h)
{
    if (!renderer) return;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_RenderDrawRect(renderer, &rect);
}
void Renderer::drawRect(int x, int y, int w, int h, Color color)
{
    if (!renderer) return;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
}


void Renderer::drawLine(const I_Vector& p1, const I_Vector& p2)
{
    SDL_RenderDrawLine(renderer,p1.x, p1.y, p2.x, p2.y);
}
void Renderer::drawLine(const I_Vector& p1, const I_Vector& p2, Color color)
{
    setDrawColor(color);
    drawLine(p1, p2);
}
void Renderer::drawLines(SDL_Point* points, int count)
{
    SDL_RenderDrawLines(renderer, points, count);
}

void Renderer::drawCircle(const I_Vector& centre, double radius)
{
    #define total 6.4
    #define rate DEG_TO_RAD(10)
    #define p_size (int(total/rate)+1)
    static SDL_Point p[p_size];
    int counter = 0;
    for (double t = 0.0; t < total; t += rate, counter++)
    {
        p[counter].x = int(centre.x+(radius*std::cos(t)));
        p[counter].y = int(centre.y+(radius*std::sin(t)));
    }
    SDL_RenderDrawLines(renderer, p, p_size);

    #undef total
    #undef rate
    #undef p_size
}
void Renderer::drawCircle(const I_Vector& centre, double radius, Color color)
{
    setDrawColor(color);
    drawCircle(centre, radius);
}

void Renderer::clear(Color color)
{
    setDrawColor(color);
    clear();
}
void Renderer::clear()
{
    SDL_RenderClear(renderer);
}

void Renderer::present()
{
    if (renderer)
        SDL_RenderPresent(renderer);
}
SDL_Renderer* Renderer::getSDL_Renderer()
{
    return renderer;
}
SDL_Window* Renderer::getSDL_Window()
{
    if (!window) return NULL;
    return window->getSDL_Window();
}
LWindow* Renderer::getWindow()
{
    return window;
}


bool Renderer::setRenderTarget(Texture*t)
{
    renderTarget = t;
    if (t)
        return (!SDL_SetRenderTarget(renderer, *t));
    return (!SDL_SetRenderTarget(renderer, NULL));
}
Texture* Renderer::getRenderTarget()
{
    return renderTarget;
}




