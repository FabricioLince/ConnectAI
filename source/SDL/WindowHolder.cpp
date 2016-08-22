#include "WindowHolder.h"
#include "Windows.h"
#include "Surface.h"

WindowHolder::WindowHolder()
{
    screenResolution.x = 800;
    screenResolution.y = 600;
    fullscreen = false;
    displayNumber = 0;
}
WindowHolder::~WindowHolder()
{
    destroyWindow();
}
SDL_Window* WindowHolder::getWindow()
{
    return window.getSDL_Window();
}
bool WindowHolder::createWindow()
{
    if (window.getSDL_Window())
    {
        printf("Window already exists\n");
        return false;
    }

    window.create("Connect_AI",
                  I_Vector(
                           SDL_WINDOWPOS_CENTERED_DISPLAY(displayNumber),
                           SDL_WINDOWPOS_CENTERED_DISPLAY(displayNumber)
                           ),
                  screenResolution,
                  fullscreen);
    if (!window.getSDL_Window())
    {
        printf("Couldn't Create Window: %s\n", SDL_GetError());
        return false;
    }

    return createRenderer();
}
bool WindowHolder::createRenderer()
{
    if (!renderer.create(&window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
    {
        printf("Couldn't create Renderer: %s\n", SDL_GetError());
        destroyWindow();

        return false;
        //throw std::runtime_error(SDL_GetError());
    }
    return true;
}
void WindowHolder::destroyWindow()
{
    window.destroy();
    destroyRenderer();
}
void WindowHolder::destroyRenderer()
{
    renderer.destroy();
}
bool WindowHolder::reloadWindow()
{
    destroyWindow();
    return createWindow();
}
bool WindowHolder::reloadRenderer()
{
    destroyRenderer();
    return createRenderer();
}
bool WindowHolder::setScreenResolution(int x, int y)
{
    if (screenResolution.x == x &&
        screenResolution.y == y) return false;
    if (!isValidScreenResolution(x, y)) return false;

    screenResolution.x = x;
    screenResolution.y = y;

    window.changeResolution(screenResolution);
    window.changePosition(I_Vector(
                                   SDL_WINDOWPOS_CENTERED_DISPLAY(displayNumber),
                                   SDL_WINDOWPOS_CENTERED_DISPLAY(displayNumber)
                                   )
                          );

    return reloadRenderer();
}
bool WindowHolder::isValidScreenResolution(int x, int y)
{
    return true;
}
bool WindowHolder::setFullscreen(bool state)
{
    if (fullscreen == state) return false;

    fullscreen = state;
    return reloadWindow();
}
bool WindowHolder::changeDisplay(int index)
{
    if (displayNumber == index) return false;
    displayNumber = index;
    return reloadWindow();
}
Renderer* WindowHolder::getRenderer()
{
    return &renderer;
}
