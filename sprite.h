#include <SDL.h>

#ifndef __SPRITE
#define __SPRITE

struct Sprite
{
    int id = -1;
    SDL_Surface* _surface = nullptr; //maybe storing the surface is useless.
    SDL_Texture* _texture = nullptr;
    
    Sprite(SDL_Renderer* renderer, const char* path);
    ~Sprite();
    void Draw(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_RendererFlip flip);
    void Draw(SDL_Renderer* renderer, int x, int y, float scale, SDL_RendererFlip flip);
};

struct Tilemap : public Sprite
{
    Tilemap(SDL_Renderer* renderer, const char* path, int tiles_w, int tiles_h);

    int width = 0;
    int height = 0;
    int map_width = 0;
    int map_height = 0;
    int tile_width = 0;
    int tile_height = 0;

    void DrawTile(int tilex, int tiley, SDL_Renderer* renderer, int x, int y, int w, int h, SDL_RendererFlip flip);
};

#endif