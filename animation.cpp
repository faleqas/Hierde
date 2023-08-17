#include "animation.h"
#include "main.h"


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

void Animation::Play(SDL_Renderer* renderer, int x, int y, float scale)
{
    if (frames)
    {
        Sprite* sprite = frames[current_frame];
        sprite->Draw(renderer, x, y, scale, flip);
        Advance(0, frames_count-1);
    }
}

void Animation::Play(SDL_Renderer* renderer, int x, int y, int w, int h)
{
    if (frames)
    {
        Sprite* sprite = frames[current_frame];
        sprite->Draw(renderer, x, y, w, h, flip);
        Advance(0, frames_count-1);
    }
}

void Animation::Advance(int min, int max)
{
    if ((Gametic() % speed) == 0)
    {
        if (current_frame < max)
        {
            current_frame++;
        }
        else
        {
            current_frame = min;
        }
    }
    if (current_frame < min)
    {
        current_frame = min;
    }
}

void Animation::Advance(const int* anim_indices, int length, int* current_index)
{
    if ((Gametic() % speed) == 0)
    {
        if (*current_index >= length)
        {
            *current_index = 0;
        }
        current_frame = anim_indices[*current_index];
        (*current_index)++;
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