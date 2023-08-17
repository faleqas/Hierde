#include "sprite.h"
#include <SDL_image.h>


Sprite::Sprite(SDL_Renderer* renderer, const char* path)
{
    _surface = IMG_Load(path);
    _texture = SDL_CreateTextureFromSurface(renderer, _surface);
}

Sprite::~Sprite()
{
    SDL_FreeSurface(_surface);
    SDL_DestroyTexture(_texture);
}

void Sprite::Draw(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_RendererFlip flip)
{
    SDL_Rect dest = {x, y, w, h};
    if (w == 0)
    {
        SDL_QueryTexture(_texture, nullptr, nullptr, &dest.w, nullptr);
    }
    if (h == 0)
    {
        SDL_QueryTexture(_texture, nullptr, nullptr, &dest.h, nullptr);
    }
    SDL_RenderCopyEx(renderer, _texture, nullptr, &dest, 0, nullptr, flip);
}

void Sprite::Draw(SDL_Renderer* renderer, int x, int y, float scale, SDL_RendererFlip flip)
{
    SDL_Rect dest = {x, y, 0, 0};
    SDL_QueryTexture(_texture, nullptr, nullptr, &dest.w, &dest.h);

    dest.w *= scale;
    dest.h *= scale;

    SDL_RenderCopyEx(renderer, _texture, nullptr, &dest, 0, nullptr, flip);
}


Tilemap::Tilemap(SDL_Renderer* renderer, const char* path, int tiles_w, int tiles_h) : Sprite(renderer, path)
{
    map_width = tiles_w;
    map_height = tiles_h;
    SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);
    tile_width = width / map_width;
    tile_height = height / map_height;
}

void Tilemap::DrawTile(int tilex, int tiley, SDL_Renderer* renderer,
 int x, int y, int w, int h, SDL_RendererFlip flip)
{
    SDL_Rect src = {tilex * tile_width,
                     tiley * tile_height,
                     tile_width,
                     tile_height };
    SDL_Rect dest = {x, y, w, h};
    if (w == 0)
    {
        SDL_QueryTexture(_texture, nullptr, nullptr, &dest.w, nullptr);
    }
    if (h == 0)
    {
        SDL_QueryTexture(_texture, nullptr, nullptr, &dest.h, nullptr);
    }
    SDL_RenderCopyEx(renderer, _texture, &src, &dest, 0, nullptr, flip);
}