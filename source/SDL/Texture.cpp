#include "Texture.h"
#include <stdexcept>
#include <SDL_image.h>
//#include "String/LString.h"
#include <string>
#define LString std::string

Texture::Texture()
{
    texture = NULL;
    renderer = NULL;
    ownTexture = false;
    SDL_zero(size);
}
Texture::Texture(Renderer* renderer_, Uint32 format, int access, int w, int h)
{
    texture = NULL;
    this->renderer = NULL;
    if (!create(renderer_, format, access, w, h))
        throw std::runtime_error(SDL_GetError());
        /*
            LString::format("texture creation error 1 (renderer:%p, format:%u, acc:%d, dim:%dx%d) : %s",
                            &renderer_, format, access, w, h, SDL_GetError()));
                            */
}
Texture::Texture(Renderer* renderer, SDL_Surface* surface)
{
    texture = NULL;
    this->renderer = NULL;
    create(renderer, surface);
}
Texture::Texture(Renderer* renderer_, const char* filePath)
{
    texture = NULL;
    this->renderer = NULL;
    create(renderer_, filePath);
}
bool Texture::create(Renderer* renderer_, Uint32 format, int access, int w, int h)
{
    destroy();
    this->renderer = renderer_;
    if (!renderer) throw std::runtime_error("Invalid Renderer for texture creation (0x02)");
    return (ownTexture = ((texture = SDL_CreateTexture(renderer_->getSDL_Renderer(), format, access, w, h)) != NULL));
}
bool Texture::create(Renderer* renderer_, SDL_Surface* surface)
{
    destroy();
    this->renderer = renderer_;
    if (!renderer) throw std::runtime_error("Invalid Renderer for texture creation (0x02)");
    if ((texture = SDL_CreateTextureFromSurface(renderer->getSDL_Renderer(), surface)) == NULL)
        throw std::runtime_error(SDL_GetError());
        /*
            LString::format("texture creation error 0 (renderer:%p, surface:%p): %s",
                            this->renderer, surface, SDL_GetError()));
                            */
    return (ownTexture = true);
}
bool Texture::create(Renderer* renderer_, const char* filePath)
{
    destroy();
    this->renderer = renderer_;
    if (!renderer) throw std::runtime_error("Invalid Renderer for texture creation (0x02)");
    SDL_Surface* image = IMG_Load(filePath);
    if (image == NULL)
    {
        char errMsg[64];
        sprintf(errMsg, "couldn't load image '%s'", filePath);
        throw std::runtime_error(errMsg);
    }

        /*
            LString::format("image loading error (renderer:%p, file path:%s): %s",
                            this->renderer, filePath, SDL_GetError()));
                            */
    texture = SDL_CreateTextureFromSurface(renderer->getSDL_Renderer(), image);
    SDL_FreeSurface(image);
    if (!texture)
    {
        throw std::runtime_error(SDL_GetError());
                                 /*LString::format("texture creation error 2 (renderer:%p, file path:%s): %s",
                                                 this->renderer, filePath, SDL_GetError()));
                                                 */
    }
    return (ownTexture = true);
}

Texture::~Texture()
{
    destroy();
}
void Texture::destroy()
{
    if (!renderer) texture = NULL;
    else if (!renderer->getSDL_Renderer()) texture = NULL;

    if (texture) SDL_DestroyTexture(texture);
    texture = NULL;
}

bool Texture::render(Renderer& renderer, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
    return !SDL_RenderCopy(renderer.getSDL_Renderer(), texture, srcrect, dstrect);
    if (SDL_RenderCopy(renderer.getSDL_Renderer(), texture, srcrect, dstrect))
        throw std::runtime_error(LString("error in rendering: ")+SDL_GetError());
    return true;
}
bool Texture::render(Renderer& renderer,
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center,
                     const SDL_RendererFlip flip)
{
    return !SDL_RenderCopyEx(renderer.getSDL_Renderer(),
                     texture,
                     srcrect,
                     dstrect,
                     angle,
                     center,
                     flip);
}

bool Texture::render(Renderer* renderer, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
    return renderer&& !SDL_RenderCopy(renderer->getSDL_Renderer(), texture, srcrect, dstrect);

    if (SDL_RenderCopy(renderer->getSDL_Renderer(), texture, srcrect, dstrect))
        throw std::runtime_error(LString("error in rendering: ")+SDL_GetError());
    return true;
}
bool Texture::render(Renderer* renderer,
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center,
                     const SDL_RendererFlip flip)
{
    return renderer&& !SDL_RenderCopyEx(renderer->getSDL_Renderer(),
                     texture,
                     srcrect,
                     dstrect,
                     angle,
                     center,
                     flip);
}

bool Texture::render(const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
    return renderer&&renderer->getSDL_Renderer()
        &&!SDL_RenderCopy(renderer->getSDL_Renderer(), texture, srcrect, dstrect);

}
bool Texture::render(
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center,
                     const SDL_RendererFlip flip)
{
    return renderer&&renderer->getSDL_Renderer()&& !SDL_RenderCopyEx(renderer->getSDL_Renderer(),
                     texture,
                     srcrect,
                     dstrect,
                     angle,
                     center,
                     flip);
}



bool Texture::update(const SDL_Rect* rect, const void* pixels, int pitch)
{
    return (!SDL_UpdateTexture(texture, rect, pixels, pitch));
}
bool Texture::update(const SDL_Rect* rect, SDL_Surface* src)
{
    return (!SDL_UpdateTexture(texture, rect, src->pixels, src->pitch));
}
void Texture::queryInfo(Uint32* format, int* access, int* w, int* h)
{
    SDL_QueryTexture(texture, format, access, w, h);
}

void Texture::setBlendMode(SDL_BlendMode blendMode)
{
    SDL_SetTextureBlendMode(texture, blendMode);
}
bool Texture::setAlphaMod(Uint8 alpha)
{
    return !SDL_SetTextureAlphaMod(texture, alpha);
}
bool Texture::setColorMod(Color color)
{
    return !SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}
bool Texture::setColorMod(Uint8 r, Uint8 g, Uint8 b)
{
    return !SDL_SetTextureColorMod(texture, r, g, b);
}

SDL_Rect* Texture::getSize()
{
    queryInfo(NULL, NULL, &size.w, &size.h);
    return &size;
}

Texture::Texture(Texture& texture)
{
    *this = texture;
}

Renderer* Texture::getRenderer()
{
    return renderer;
}

