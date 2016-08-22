#include "Surface.h"
#include "Color.h"
#include "defines.h"
//#include "Shape.h"
#include <stdexcept>
#include "SDL2_gfx-1.0.1/SDL2_gfxPrimitives.h"
#include "SDL2_gfx-1.0.1/SDL2_rotozoom.h"

Surface::Surface(int w, int h, int color)
{
    image = NULL;
    if (w > 0 && h > 0)
    {
        create(w, h, color);
    }
}

Surface::Surface(const char* path)
{
    image = NULL;
    load(path);
}

Surface::Surface(Surface& source)
{
    image = NULL;
    (*this) = source;
}

bool Surface::create(int w, int h, int color)
{
    destroy();
    image = SDL_CreateRGBSurface(0, w, h, 32,
                                 0xff0000,
                                 0xff00,
                                 0xff,
                                 0xff000000);
    if (image == NULL) return false;

    clear(color);

    return true;
}

bool Surface::load(const char* path)
{
    image = IMG_Load(path);

    return (image);
}

void Surface::destroy()
{
    if (!image) return;
    SDL_FreeSurface(image);
    image = NULL;
}

Surface::~Surface()
{
    destroy();
}

void Surface::draw(Surface& src, int x, int y, float scaleFactor, int rotateFactor)
{
    if (!image) return;
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;

    this->draw(src, dstrect, scaleFactor, rotateFactor);
}

void Surface::draw(Surface& src, SDL_Rect& pos, float scaleFactor, int rotateFactor)
{
    if (!image) return;
    SDL_Surface* src_ = src.get();
    if (scaleFactor != 1.0 || rotateFactor)
    {
        src_ = rotozoomSurface(src.get(), rotateFactor, scaleFactor, SMOOTHING_ON);
    }

    SDL_BlitSurface(
                    src_, NULL,
                    image, &pos
                    );

    if (scaleFactor != 1.0 || rotateFactor)
    {
        SDL_FreeSurface(src_);
    }
}

void Surface::blit(Surface& source, SDL_Rect& rectSource, int x, int y)
{
    if (!image) return;
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    SDL_BlitSurface(
                    source.get(), &rectSource,
                    image, &pos
                    );
}

SDL_Surface* Surface::get()
{
    return image;
}

void Surface::clear(int color)
{
    if (!image) return;
    //SDL_Color rgb = Color::getColor(image->format, color);
    //SDL_FillRect(image, NULL, SDL_MapRGB(image->format, rgb.r, rgb.g, rgb.b));

    SDL_FillRect(image, NULL,
                 SDL_MapRGB(image->format,
                    color%0x1000000/0x10000,
                    color%0x10000/0x100,
                    color%0x100));
}

void Surface::clear(Color color)
{
    if (!image) return;

    Uint32 pixel = createUint32Color(image->format, color.r, color.g, color.b, color.a);

    repeat(xA, (unsigned)getWidth())
    {
        repeat(yA, (unsigned)getHeight())
        {
            low_level_putPixelColor(xA, yA, pixel);
        }
    }
}

void Surface::rect_(int x, int y, int w, int h, int lineColor, int fillColor)
{
    static SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    rect_(rect, lineColor, fillColor);
    return;
}

void Surface::rect_(SDL_Rect& rect, int lineColor, int fillColor)
{
    if (!image) return;
    if (fillColor != -1)
    {
        SDL_Color fillRGB = Color::getColor(image->format, fillColor);
        SDL_FillRect(image, &rect, SDL_MapRGB(image->format, fillRGB.r, fillRGB.g, fillRGB.b));
    }
    if (fillColor != lineColor)
    {
        hline(rect.x,        rect.y,         rect.x+rect.w, lineColor);
        hline(rect.x,        rect.y+rect.h,  rect.x+rect.w, lineColor);
        vline(rect.x,        rect.y,         rect.y+rect.h, lineColor);
        vline(rect.x+rect.w, rect.y,         rect.y+rect.h, lineColor);
    }
}

void Surface::putPixelColor(int x, int y, Color color)
{
    if (!image) return;
    putPixel(x, y, SDL_MapRGBA(image->format, color.r, color.g, color.b, color.a));
}
void Surface::putPixel(int x, int y, Uint32 color)
{
    if (!image) return;

    if (x >= 0 && x < image->w && y >= 0 && y <= image->h)
//    SDL_LockSurface(image);
    ((Uint32*)image->pixels)[y * image->w + x] = color;
//    SDL_UnlockSurface(image);
}

void Surface::hline(int x1, int y, int x2, Uint32 color)
{
    if (!image) return;
    while (x1 < x2)
    {
        putPixel(x1, y, color);
        x1++;
    }
    putPixel(x1, y, color);
}

void Surface::vline(int x,  int y1, int y2, Uint32 color)
{
    if (!image) return;
    while (y1 < y2)
    {
        putPixel(x, y1, color);
        y1++;
    }
    putPixel(x, y1, color);
}

int Surface::getWidth()
{
    return (image?image->w:0);
}
int Surface::getHeight()
{
    return (image?image->h:0);
}

void Surface::setColorKey(bool enabled, int color)
{
    if (!image) return;
    SDL_SetColorKey(image, enabled, color);
}

Color Surface::getPixelColor(int x, int y)
{
    if (!image) return Color(0, 0, 0, 0);
    return Color::getColor(image->format, getPixel(x, y));
}

int Surface::getPixel(int x, int y)
{
    if (!image) return -1;
    return ((Uint32*)image->pixels)[y * image->w + x];
//    SDL_UnlockSurface(image);
}

void Surface::savePNG(const char* path)
{
    if (!image) return;
    IMG_SavePNG(image, path);
}

void Surface::low_level_putPixelColor(int x, int y, Uint32 color)
{
    ((Uint32*)image->pixels)[y * image->w + x] = color;
}

void Surface::line(I_Vector p1, I_Vector p2, Uint32 color)
{
    line(p1.x, p1.y, p2.x, p2.y, color);
}

void Surface::line(int x1, int y1, int x2, int y2, Uint32 color)
{
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if (steep)
    {
        SWAP(x1, y1);
        SWAP(x2, y2);
    }

    if (x1 > x2)
    {
        SWAP(x1, x2);
        SWAP(y1, y2);
    }

    const double dx = x2 - x1;
    const double dy = fabs(y2 - y1);

    double error = dx / 2.0;
    const int ystep = (y1 < y2)? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;

    for (int x = (int)x1; x < maxX; x++)
    {
        if (steep)
        {
            putPixel(y, x, color);
        }
        else
        {
            putPixel(x, y, color);
        }
        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}

Surface& Surface::operator=(Surface& source)
{
    throw std::runtime_error("Surface::operator= not implemented");
    this->destroy();
    this->create(source.getWidth(), source.getHeight(), 0xffff);
    SDL_Rect rect;
    memset(&rect, 0, sizeof(SDL_Rect));
    rect.w = getWidth();
    rect.h = getHeight();
    printf("%d x %d : %d x %d\n", rect.x, rect.y, rect.w, rect.h);
    this->blit(source, rect, 0, 0);
    return (*this);
}


void Surface::hack()
{
     /***
    ATENÇÃO, GAMBIARRA A FRENTE
    Por algum motivo algumas imagens estão sendo carregadas com a mascara invertida
    ou seja, imagem que foram salvas ARGB, estão sendo carregadas ABGR
    isso está causando as imagens serem mostradas com as "cores invertidas"
    sendo assim, eu verifico se a mascara está invertida, e se estiver eu inverto
    a posição dos componentes cores
    ***/

    // se não possui mascara retorna
    if (image->format->Rmask == 0) return;
    // se a mascara para AZUL estiver correta retorna
    if (image->format->Bmask == 0xFF) return;

    // faz uma varredura na imagem invertendo os componentes das cores
    int w = getWidth();
    int h = getHeight();
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            Color color = getPixelColor(x, y);

            int temp = color.r;
            color.r = color.b;
            color.b = temp;

            putPixelColor(x, y, color);
        }
    }

    /***
    FIM DA GAMBIARRA
    ***/

}
