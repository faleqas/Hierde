
#ifndef __ANIMATION
#define __ANIMATION
#include "sprite.h"
enum
{
    ANIM_PLAYER_IDLE,
    ANIM_PLAYER_RUN,
    ANIM_PLAYER_SHOOT,
    ANIM_LASER_BULLET,
    ANIM_TILE
};

//const int PLAYER_WALK_INDICES[7] = {5, 3, 4, 5, 4, 3, 4};
const int PLAYER_RUN_INDICES[5] = {0, 1, 2, 3, 4};
const int PLAYER_IDLE_INDICES[3] = {0, 1, 2};

struct Animation
{
    int id = -1;
    Sprite** frames = nullptr;
    int frames_count = 0;
    int current_frame = 0;
    int speed = 1; //if gametic % speed is 0 advance the animation.
    bool loop = true;
    bool just_done = false; //when animation just reached last frame
    int since_done = -1;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    //TODO() add the ability to reorder the sprites. maybe read animation data from a file and
    //create a program with a GUI to edit animations freely

    Animation(int speed = 60);
    ~Animation();
    void AddSprite(Sprite* sprite);
    void Play(SDL_Renderer* renderer, int tic, int x, int y, float scale);
    void Play(SDL_Renderer* renderer, int tic, int x, int y, int w, int h);
    void DrawTile(int sprite_index,
    int tilex, int tiley, SDL_Renderer* renderer, int x, int y, int w, int h, SDL_RendererFlip flip);
    void Advance(int tic, /*since last change of anim tic*/ int min, int max);
    void Advance(int tic, const int* anim_indices, int length, int* current_index);
    void Restart();

    //int tic is the time since this animation started being used or played in tics
    //to avoid weird glitching in advance and stuff so it starts from 0 everytime
};
#endif