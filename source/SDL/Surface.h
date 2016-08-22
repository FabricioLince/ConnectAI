#ifndef IMAGE_H
#define IMAGE_H
#include <SDL.h>
#include <SDL_image.h>
#include "SDL2_gfx-1.0.1/SDL2_gfxPrimitives.h"
#include "SDL2_gfx-1.0.1/SDL2_rotozoom.h"
#include <stdio.h>

#include "Color.h"
#include "LVector.h"

#define pause(miliseconds) {int start=GetTickCount();while(GetTickCount()-start<miliseconds);}

class Surface
{
    void hack();

    public:

        Surface(int w = 0, int h = 0, int color = 0);
        Surface(const char* path);
        Surface(Surface&);
        Surface(SDL_Surface* src)
        {
            this->image = src;
        }
        ~Surface();

        Surface& operator=(Surface&);

        void destroy();
        bool load(const char* path);
        bool create(int w, int h, int color = 0);
        void save(const char* path);

        void setColorKey(bool enabled, int color = 0xFF00FF);

        void clear(int color = 0);
        void clear(Color color);
        void draw(Surface& src, int x = 0, int y = 0, float scaleFactor = 1, int rotateFactor = 0);
        void draw(Surface& src, SDL_Rect& pos, float scaleFactor = 1, int rotateFactor = 0);
        template < typename C >
        void Surface::draw(Surface& src, LVector<C>& pos, float scaleFactor = 1, int rotateFactor=0)
        {
            draw(src, int(pos.x), int(pos.y), scaleFactor, rotateFactor);
        }

        void drawBlit(Surface& source, Surface& dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);
        void blit(Surface& source, SDL_Rect& rectSource, int x, int y);

        SDL_Surface* get();
        SDL_Surface* image;

        int getWidth();
        int getHeight();

        void rect_(int x, int y, int w, int h, int lineColor, int fillColor = -1);
        void rect_(SDL_Rect& rect, int lineColor, int fillColor = -1);
        void circle_(int x, int y, int radius, int lineColor, int fillColor = -1);
        void copy(Surface& origin);

        void putPixelColor(int x, int y, Color color);
        void putPixel(int x, int y, Uint32 color);
        int getPixel(int x, int y);
        Color getPixelColor(int x, int y);

        void line(I_Vector p1, I_Vector p2, Uint32 color);
        void line(int x1, int y1, int x2, int y2, Uint32 color);
        void hline(int x1, int y, int x2, Uint32 color);
        void vline(int x,  int y1, int y2, Uint32 color);
        void filterToColor(int originColor, int resultColor);
        void floodFill(int x, int y, int color);

        void savePNG(const char* path);

    protected:

        void low_level_putPixelColor(int x, int y, Uint32 color);

};

#endif // IMAGE_H
