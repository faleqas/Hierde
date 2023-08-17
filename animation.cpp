#include "animation.h"
#include "main.h"
#include <stdio.h>

Animation::Animation(int speed)
{
    this->speed = speed;
}

Animation::~Animation()
{
    //sprites are cleaned when the animation is deleted. make this optional TODO()
    if (frames)
    {
        for (int i = 0; i < frames_count; i++)
        {
            if (frames[i])
            {
                delete frames[i];
                frames[i] = nullptr;
            }
        }
        free(frames);
    }
}

void Animation::AddSprite(Sprite* sprite)
{
    if (frames)
    {
        frames_count++;
        frames = (Sprite**)realloc(frames, sizeof(Sprite*) * frames_count);
        frames[frames_count-1] = sprite;
    }
    else
    {
        frames_count++;
        frames = (Sprite**)malloc(sizeof(Sprite*) * frames_count);
        frames[frames_count-1] = sprite;
    }
}

void Animation::Play(SDL_Renderer* renderer, int tic, int x, int y, float scale)
{
    if (frames)
    {
        just_done = false;
        Sprite* sprite = frames[current_frame];
        sprite->Draw(renderer, x, y, scale, flip);
        Advance(tic, 0, frames_count-1);
    }
}

void Animation::Play(SDL_Renderer* renderer, int tic, int x, int y, int w, int h)
{
    if (frames)
    {
        just_done = false;
        if (since_done >= 0)
        {
            since_done++;
        }
        Sprite* sprite = frames[current_frame];
        sprite->Draw(renderer, x, y, w, h, flip);
        Advance(tic, 0, frames_count-1);
    }
}

void Animation::Advance(int tic, int min, int max)
{
    if ((Gametic() % speed) == 0)
    {
        if (current_frame < max)
        {
            current_frame++;
            if (current_frame >= max)
            {
                just_done = true;
                since_done = 0;
            }
        }
        else
        {
            if (loop)
            {
                current_frame = min;
            }
            else
            {
                current_frame = max;
            }
        }
    }
    if (current_frame < min)
    {
        current_frame = min;
    }
}

void Animation::Advance(int tic, const int* anim_indices, int length, int* current_index)
{
    if ((Gametic() % speed) == 0)
    {
        if (*current_index >= length)
        {
            if (loop)
            {
                *current_index = 0;
            }
            else
            {
                *current_index = length-1;
            }
        }
        current_frame = anim_indices[*current_index];
        (*current_index)++;
        if (*current_index >= length)
        {
            just_done = true;
            since_done = 0;
        }
    }
}

void Animation::DrawTile(int sprite_index, int tilex, int tiley, SDL_Renderer* renderer,
 int x, int y, int w, int h, SDL_RendererFlip flip)
{
    Tilemap* tilemap = nullptr;
    if (sprite_index == -1)
    {
        tilemap = (Tilemap*)frames[0];
    }
    else
    {
        tilemap = (Tilemap*)frames[sprite_index];
    }
    tilemap->DrawTile(tilex, tiley, renderer, x, y, w, h, flip);
}

void Animation::Restart()
{
    current_frame = 0;
    printf("restart\n");
}