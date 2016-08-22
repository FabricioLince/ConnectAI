#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL.h>
#include "Renderer.h"

class Texture
{
    public:

        Texture();
        Texture(Renderer* renderer, Uint32 format, int access, int w, int h);
        Texture(Renderer* renderer, SDL_Surface* surface);
        Texture(Texture& texture);
        Texture(Renderer* renderer, const char* filePath);
        virtual ~Texture();

        bool create(Renderer* renderer, Uint32 format, int access, int w, int h);
        bool create(Renderer* renderer, SDL_Surface* surface);
        bool create(Renderer* renderer, const char* filePath);

        void destroy();

        bool render(Renderer& renderer, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
        bool render(Renderer& renderer,
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center = NULL,
                     const SDL_RendererFlip flip = SDL_FLIP_NONE);

        bool render(Renderer* renderer, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
        bool render(Renderer* renderer,
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center = NULL,
                     const SDL_RendererFlip flip = SDL_FLIP_NONE);

        bool render(const SDL_Rect* srcrect, const SDL_Rect* dstrect);
        bool render(
                    const SDL_Rect*        srcrect,
                    const SDL_Rect*        dstrect,
                    const double           angle,
                    const SDL_Point*       center = NULL,
                    const SDL_RendererFlip flip = SDL_FLIP_NONE
                    );

        void queryInfo(Uint32* format, int* access, int* w, int* h);

        bool update(const SDL_Rect* rect, const void* pixels, int pitch);
        bool update(const SDL_Rect* rect, SDL_Surface* src);

        void setBlendMode(SDL_BlendMode blendMode);
        bool setAlphaMod(Uint8 alpha);
        bool setColorMod(Color color);
        bool setColorMod(Uint8 r, Uint8 g, Uint8 b);

        operator SDL_Texture*()
        {
            return texture;
        }
        bool operator !()
        {
            return (!texture);
        }

        Texture& operator=(Texture& other)
        {
            exit(0);
            texture = other.texture;
            ownTexture = false;
            return (*this);
        }

        SDL_Rect* getSize();

        Renderer* getRenderer();

    protected:

    SDL_Texture* texture;
    Renderer* renderer;
    bool ownTexture;
    SDL_Rect size;
};

#endif // TEXTURE_H
