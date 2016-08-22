#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include "Color.h"
#include "LVector.h"
#include <string>

class Texture;

class LWindow
{
    public:
        LWindow();
        LWindow(std::string title_, I_Vector position_, I_Vector resolution_, bool fullscreen_);
        ~LWindow();

        bool create(std::string title_, I_Vector position_, I_Vector resolution_, bool fullscreen_);

        void changeResolution(I_Vector newResolution);
        void changePosition(I_Vector newPosition);

        void destroy();

        SDL_Window* getSDL_Window();

        bool isFullscreen();
        I_Vector getResolution();
        const char* getTitle();

    protected:
    public: /// temporário até resolver o ECS.cpp
        std::string title;
        I_Vector position;
        I_Vector resolution;
        bool fullscreen;
        SDL_Window* window;
};



class Renderer
{
    public:
        Renderer();
        virtual ~Renderer();

        bool create(LWindow* window, int index, Uint32 flags);
        void destroy();

        bool setDrawColor(Color color);
        bool setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

        void drawRect(const SDL_Rect* rect);
        void drawRect(const SDL_Rect* rect, Color color);
        void drawRect(int x, int y, int w, int h);
        void drawRect(int x, int y, int w, int h, Color color);

        void fillRect(const SDL_Rect* rect);
        void fillRect(const SDL_Rect* rect, Color color);
        void fillRect(int x, int y, int w, int h);
        void fillRect(int x, int y, int w, int h, Color color);

        void drawLine(const I_Vector& p1, const I_Vector& p2);
        void drawLine(const I_Vector& p1, const I_Vector& p2, Color color);

        void drawLines(SDL_Point* points, int count);

        void drawCircle(const I_Vector& centre, double radius);
        void drawCircle(const I_Vector& centre, double radius, Color color);


        void present();
        void clear(Color color);
        void clear();

        SDL_Renderer* getSDL_Renderer();
        SDL_Window* getSDL_Window();
        LWindow* getWindow();

        Texture* getRenderTarget();
        bool setRenderTarget(Texture*t);

    protected:
    private:
        SDL_Renderer* renderer;
        static SDL_Rect rect;
        LWindow* window;
        Texture* renderTarget;
};

#endif // RENDERER_H
