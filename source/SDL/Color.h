#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>
#include <math.h>
#define MAX(a, b) (a>b?a:b)
#define MAX3(a, b, c) (MAX(a, MAX(b, c)))
#define MIN(a, b) (a<b?a:b)
#define MIN3(a, b, c) (MIN(a, MIN(b, c)))
#define ABS(a) (a<0?(-a):(a))

#define GRAY(value) (0x##value##value##value)

class Color : public SDL_Color
{
    SDL_PixelFormat* format;
    public:
        Color()
        : format(NULL)
        {a=0xff;}
        Color(Uint8 r_, Uint8 g_, Uint8 b_, Uint8 a_ = 0xff, SDL_PixelFormat* format_=NULL)
        : format(format_)
        {
            r = r_;
            g = g_;
            b = b_;
            a = a_;
        }
        Color(const SDL_Color& color)
        : format(NULL)
        {
            r = color.r;
            g = color.g;
            b = color.b;
            a = color.a;
        }

        void setFormat(SDL_PixelFormat* format_)
        {
            format = format_;
        }
        Uint32 makeUint32()
        {
            return SDL_MapRGBA(format, r, g, b, a);
        }

        static const Color WHITE;
        static const Color W_TRANSPARENT;
        static const Color BLACK;

        bool operator==(Color c);

        static Color getColor(SDL_PixelFormat *fmt, Uint32 pixel);
        static int getColor(int color);

    #if SDL_BYTEORDER == SDL_LIL_ENDIAN
        static const int rmask = 0x000000ff;
        static const int gmask = 0x0000ff00;
        static const int bmask = 0x00ff0000;
        static const int amask = 0xff000000;
    #else
        static const int rmask = 0x00ff0000;
        static const int gmask = 0x0000ff00;
        static const int bmask = 0x000000ff;
        static const int amask = 0xff000000;
    #endif
};

struct HSV
{
    int hue;
    double sat;
    double val;
};

typedef Color RGB;

HSV fromRGB(int r, int g, int b);
HSV fromRGB(RGB rgb);// {return fromRGB(rgb.r, rgb.g, rgb.b);}
RGB fromHSV(int h, double s, double v);
RGB fromHSV(HSV hsv);// {return fromHSV(hsv.hue, hsv.sat, hsv.val);}

extern SDL_Color createColor(SDL_PixelFormat* fmt, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
extern SDL_Color createColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

extern Uint32 createUint32Color(SDL_PixelFormat* fmt, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

#endif // COLOR_H
