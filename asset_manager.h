
#ifndef __ASSET_MANAGER
#define __ASSET_MANAGER
#include "animation.h"

struct AssetManager
{
    Animation** animations = nullptr;
    int animation_count = 0;
    
    ~AssetManager();
    Animation* GetAnimation(int id);
    void AddAnimation(Animation* anim);
};

#endif