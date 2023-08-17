#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "animation.h"
#include "asset_manager.h"
#include "object.h"

//NEXT: OBJECT HANDLING SYSTEM (CREATION, DELETION) DONE()


static int tic = 0;
AssetManager* asset_mng = nullptr;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* window = SDL_CreateWindow("Window", 100, 100, 800, 600, 0);
    SDL_Rect window_rect = {0, 0, 800, 600};

    if (!window)
    {
        return 1;
    }

    SDL_Surface* win_surface = SDL_GetWindowSurface(window);
    int start_tick = SDL_GetTicks();
    float delta = 0;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    
    asset_mng = new AssetManager;

    {
        // Sprite* sprite = new Sprite(renderer, "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/walk1.png");
        // Animation* animation = new Animation();
        // animation->AddSprite(sprite);
        // //sprites are cleaned when the animation is deleted. make this optional TODO()
        // sprite = new Sprite(renderer, "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/walk2.png");
        // animation->AddSprite(sprite);
        // animation->id = ANIM_PLAYER_WALK;
        // animation->speed = 7;
        // asset_mng->AddAnimation(animation);

        // sprite = new Sprite(renderer, "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/idle.png");
        // animation = new Animation();
        // animation->AddSprite(sprite);
        // animation->id = ANIM_PLAYER_IDLE;
        // asset_mng->AddAnimation(animation);

        Tilemap* dirt_map = new Tilemap(renderer,
        "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/dirt.png",
                    5, 7);

        Animation* animation = new Animation();
        animation->AddSprite(dirt_map);
        animation->id = ANIM_TILE;
        asset_mng->AddAnimation(animation);

        {
            Animation* player_idle = new Animation();

            const char* idle_frames[] =
            {
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/idle/0.png",
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/idle/2.png",
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/idle/3.png"
            };

            for (int i = 0; i < sizeof(idle_frames) / sizeof(const char*); i++)
            {
                Sprite* idle = new Sprite(renderer, idle_frames[i]);
                if (idle)
                {
                    player_idle->AddSprite(idle);
                }
            }

            player_idle->id = ANIM_PLAYER_IDLE;
            player_idle->speed = 18;
            asset_mng->AddAnimation(player_idle);
        }

        {
            Animation* player_run = new Animation();

            const char* run_frames[] =
            {
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/running/1.png",
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/running/2.png",
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/running/3.png",
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/running/4.png",
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/running/5.png"
            };

            for (int i = 0; i < sizeof(run_frames) / sizeof(const char*); i++)
            {
                Sprite* run = new Sprite(renderer, run_frames[i]);
                if (run)
                {
                    player_run->AddSprite(run);
                }
            }

            player_run->id = ANIM_PLAYER_RUN;
            player_run->speed = 8;
            asset_mng->AddAnimation(player_run);
        }

        {
            Animation* player_shoot = new Animation();

            const char* shoot_frames[] =
            {
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/shooting/1.png",
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/shooting/2.png",
            "C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/shooting/3.png"
            };

            for (int i = 0; i < sizeof(shoot_frames) / sizeof(const char*); i++)
            {
                Sprite* shoot = new Sprite(renderer, shoot_frames[i]);
                if (shoot)
                {
                    player_shoot->AddSprite(shoot);
                }
            }

            player_shoot->id = ANIM_PLAYER_SHOOT;
            player_shoot->speed = 9;
            asset_mng->AddAnimation(player_shoot);
        }

        {
            Animation* laser_bullet = new Animation();
            Sprite* frame1 = new Sprite(renderer,
"C:/Users/admin/source/repos/Temporintial/Temporintial/assets/player/shooting/laser_bullet.png");

            laser_bullet->AddSprite(frame1);
            laser_bullet->id = ANIM_LASER_BULLET;
            asset_mng->AddAnimation(laser_bullet);
        }
        
    }

    ObjectManager* object_mng = new ObjectManager();
    int player_id = object_mng->AddObject(new Player(0, 0, 5.0f));
    
    int tile_x = 0;
    int tile_y = 0;
    for (int i = 0; i < 10; i++)
    {
        object_mng->AddObject(new Tile(19 * i * 5.0f, 400, 19, 13, 5.0f, tile_x, tile_y));
        tile_x++;
        if (tile_x >= 5)
        {
            tile_x = 0;
            tile_y++;
        }
    }

    while (true)
    {
        int tick = SDL_GetTicks();

        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }
        }

        delta = tick - start_tick;
        if (delta > 1000/60.0)
        {
            object_mng->Update();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            object_mng->Draw(renderer);
            
            SDL_RenderPresent(renderer);

            start_tick = tick;
        }
        delta = 0;
        tic = tick;
    }

    delete asset_mng;
    object_mng->DeleteObjects();
    delete object_mng;

    printf("bye\n");


    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


AssetManager* GetAssetMng()
{
    return asset_mng;
}

int Gametic()
{
    return tic;
}

bool CheckAABBCol(int x1, int y1, int w1, int h1,
                  int x2, int y2, int w2, int h2)
{
    if(x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}