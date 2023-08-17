
#ifndef __ANIMATION
#define __ANIMATION
#include "sprite.h"
enum
{
    ANIM_PLAYER,
    ANIM_TILE
};

//const int PLAYER_WALK_INDICES[7] = {5, 3, 4, 5, 4, 3, 4};
const int PLAYER_WALK_INDICES[5] = {4, 1, 5, 3, 2};
const int PLAYER_IDLE_INDICES[1] = {0};

struct Animation
{
    int id = -1;
    Sprite** frames = nullptr;
    int frames_count = 0;
    int current_frame = 0;
    int speed = 1; //if gametic % speed is 0 advance the animation.
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    //TODO() add the ability to reorder the sprites. maybe read animation data from a file and
    //create a program with a GUI to edit animations freely

    Animation(int speed = 60);
    ~Animation();
    void AddSprite(Sprite* sprite);
    void Play(SDL_Renderer* renderer, int x, int y, float scale);
    void Play(SDL_Renderer* renderer, int x, int y, int w, int h);
    void DrawTile(int sprite_index,
    int tilex, int tiley, SDL_Renderer* renderer, int x, int y, int w, int h, SDL_RendererFlip flip);
    void Advance(int min, int max);
    void Advance(const int* anim_indices, int length, int* current_index);
};
#endif