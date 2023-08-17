#include "asset_manager.h"


AssetManager::~AssetManager()
{
    for (int i = 0; i < animation_count; i++)
    {
        if (animations[i])
        {
            delete animations[i];
        }
    }
    delete animations;
}

void AssetManager::AddAnimation(Animation* anim)
{
    animation_count++;
    if (animations)
    {
        animations = (Animation**)realloc(animations, sizeof(Animation*) * animation_count);
    }
    else
    {
        animations = (Animation**)malloc(sizeof(Animation*) * animation_count);
    }
    animations[animation_count-1] = anim;
}


Animation* AssetManager::GetAnimation(int id)
{
    for (int i = 0; i < animation_count; i++)
    {
        if (animations[i]->id == id)
        {
            return animations[i];
        }
    }
    return nullptr;
}