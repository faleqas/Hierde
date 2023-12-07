#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "animation.h"
#include "asset_manager.h"
#include "object.h"
#include "vector.h"
#include "main.h"
#include "map.h"
#include <algorithm>

//NEXT: OBJECT HANDLING SYSTEM (CREATION, DELETION) DONE()


static int tic = 0;
AssetManager* asset_mng = nullptr;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* window = SDL_CreateWindow("Window", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    

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

        {

        }
        
    }

    ObjectManager* object_mng = new ObjectManager();
    //int player_id = object_mng->AddObject(new Player(200, 0, 3.0f));
    
    // int tile_x = 0;
    // int tile_y = 0;
    // for (int i = 0; i < 50; i++)
    // {
    //     object_mng->AddObject(new Tile(19 * i * 3.0f, 400, 19, 13, 3.0f, 1, 0));
    //     tile_x++;
    //     if (tile_x >= 5)
    //     {
    //         tile_x = 0;
    //         tile_y++;
    //     }
    // }

    //object_mng->AddObject(new Abdo(100, 100, 3.5f));
    LoadMap(object_mng, "C:/Users/admin/source/repos/Temporintial/Debug/FIRST.lvl");

    SDL_Rect rect1 =
    {
        0, 0,
        128, 128
    };

    SDL_Rect rect2 =
    {
        200, 10,
        128, 128
    };

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

            const uint8_t* state = SDL_GetKeyboardState(nullptr);
            
            // if (state[SDL_SCANCODE_RIGHT])
            // {
            //     rect1.x += 4;
            // }

            // if (state[SDL_SCANCODE_LEFT])
            // {
            //     rect1.x -= 4;
            // }

            // if (state[SDL_SCANCODE_UP])
            // {
            //     rect1.y -= 4;
            // }

            // if (state[SDL_SCANCODE_DOWN])
            // {
            //     rect1.y += 4;
            // }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            object_mng->Draw(renderer);

            // Vector2* rect1_vertices = AABBToVertices(rect1.x, rect1.y, rect1.w, rect1.h);
            // DrawVertices(renderer, rect1_vertices, 4);
            // Vector2* rect2_vertices = AABBToVertices(rect2.x, rect2.y, rect2.w, rect2.h);
            // bool collide = SATCollision(rect1_vertices, 4, rect2_vertices, 4);
            // free(rect1_vertices);
            // free(rect2_vertices);

            // if (collide)
            // {
            //     SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            // }
            // else
            // {
            //     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            // }

            //SDL_RenderDrawRect(renderer, &rect1);
            SDL_RenderDrawRect(renderer, &rect2);
            
            SDL_RenderPresent(renderer);

            start_tick = tick;
        }
        delta = 0;
        tic = tick;
    }

    //// DESTRUCTION CODE NOT NEEDED NOW
    //// WE MIGHT NEED IT IF THE GAME IS WRAPPED IN A FRONT END OR SOMETHING
    // delete asset_mng;
    // object_mng->DeleteObjects();
    // delete object_mng;

    // printf("bye\n");


    // SDL_DestroyWindow(window);
    // SDL_Quit();
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


void DrawVertices(SDL_Renderer* renderer, Vector2* vertices, int len)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    if (len == 3)
    {
        SDL_RenderDrawLine(renderer, vertices[0].x, vertices[0].y,
                           vertices[1].x, vertices[1].y);
        SDL_RenderDrawLine(renderer, vertices[1].x, vertices[1].y,
                    vertices[2].x, vertices[2].y);
        SDL_RenderDrawLine(renderer, vertices[2].x, vertices[2].y,
                           vertices[0].x, vertices[0].y);
    }
    if (len == 4)
    {
        SDL_RenderDrawLine(renderer, vertices[0].x, vertices[0].y,
                           vertices[1].x, vertices[1].y);
        SDL_RenderDrawLine(renderer, vertices[1].x, vertices[1].y,
                    vertices[3].x, vertices[3].y);
        SDL_RenderDrawLine(renderer, vertices[2].x, vertices[2].y,
                           vertices[0].x, vertices[0].y);
        SDL_RenderDrawLine(renderer, vertices[2].x, vertices[2].y,
                           vertices[3].x, vertices[3].y);
    }
}


bool SATCollision(Vector2* poly1, int poly1_len,
                  Vector2* poly2, int poly2_len)
{
    Line* poly1_sides = nullptr;
    Line* poly2_sides = nullptr;
    int poly1_sides_count = 0;
    int poly2_sides_count = 0;
    if (poly1_len == 4)
    {
        poly1_sides = (Line*)calloc(poly1_len, sizeof(Line));
        poly1_sides[0] = {poly1[0], poly1[1]};
        poly1_sides[1] = {poly1[3], poly1[1]};
        poly1_sides[2] = {poly1[2], poly1[0]};
        poly1_sides[3] = {poly1[2], poly1[3]};
        poly1_sides_count = 4;
    }
    else if (poly1_len == 3)
    {
        poly1_sides = (Line*)calloc(poly1_len, sizeof(Line));
        poly1_sides[0] = {poly1[0], poly1[1]};
        poly1_sides[1] = {poly1[1], poly1[2]};
        poly1_sides[2] = {poly1[2], poly1[0]};
        poly1_sides_count = 3;
    }
    else
    {
        return false;
    }

    if (poly2_len == 4)
    {
        poly2_sides = (Line*)calloc(poly2_len, sizeof(Line));
        poly2_sides[0] = {poly2[0], poly2[1]};
        poly2_sides[1] = {poly2[3], poly2[1]};
        poly2_sides[2] = {poly2[2], poly2[0]};
        poly2_sides[3] = {poly2[2], poly2[3]};
        poly1_sides_count = 4;
    }
    else if (poly2_len == 3)
    {
        poly2_sides = (Line*)calloc(poly2_len, sizeof(Line));
        poly2_sides[0] = {poly2[0], poly2[1]};
        poly2_sides[1] = {poly2[1], poly2[2]};
        poly2_sides[2] = {poly2[2], poly2[0]};
        poly1_sides_count = 3;
    }
    else
    {
        return false;
    }


    for (int i = 0; i < poly1_sides_count; i++)
    {
        Line side = poly1_sides[i];

        Vector2 axis = {
            -(side.v2.y - side.v1.y),
            side.v2.x - side.v1.x
        };

        Vector2 normal_axis = axis.Normalize();
        normal_axis.Print();

        float poly1_min = normal_axis.DotProduct(poly1[0]);
        float poly1_max = poly1_min;

        for (int i = 0; i < poly1_len; i++)
        {
            Vector2 vertex = poly1[i];
            if (normal_axis.x)
            {
                poly1_min = std::min(vertex.x, poly1_min);
                poly1_max = std::max(vertex.x, poly1_max);
            }
            if (normal_axis.y)
            {
                poly1_min = std::min(vertex.y, poly1_min);
                poly1_max = std::max(vertex.y, poly1_max);
            }
        }

        float poly2_min = normal_axis.DotProduct(poly2[0]);
        float poly2_max = poly2_min;

        for (int i = 0; i < poly2_len; i++)
        {
            Vector2 vertex = poly2[i];
            if (normal_axis.x)
            {
                poly2_min = std::min(vertex.x, poly2_min);
                poly2_max = std::max(vertex.x, poly2_max);
            }
            if (normal_axis.y)
            {
                poly2_min = std::min(vertex.y, poly2_min);
                poly2_max = std::max(vertex.y, poly2_max);
            }
        }

        Vector2 distance = {
            poly1[0].x - poly2[0].x,
            poly1[0].y - poly2[0].y
        };

        float projected_distance = normal_axis.DotProduct(distance);

        // poly1_min += projected_distance;
        // poly1_max += projected_distance;

        bool gap_exists = ((poly1_min - poly2_max) > 0) || ((poly2_min - poly1_max) > 0);

        if (gap_exists)
        {
            free(poly1_sides);
            free(poly2_sides);
            return false;
        }
    }

    for (int i = 0; i < poly2_sides_count; i++)
    {
        Line side = poly2_sides[i];

        Vector2 axis = {
            -(side.v2.y - side.v1.y),
            side.v2.x - side.v1.x
        };

        Vector2 normal_axis = axis.Normalize();
        normal_axis.Print();

        float poly1_min = normal_axis.DotProduct(poly1[0]);
        float poly1_max = poly1_min;

        for (int i = 0; i < poly1_len; i++)
        {
            Vector2 vertex = poly1[i];
            if (normal_axis.x)
            {
                poly1_min = std::min(vertex.x, poly1_min);
                poly1_max = std::max(vertex.x, poly1_max);
            }
            if (normal_axis.y)
            {
                poly1_min = std::min(vertex.y, poly1_min);
                poly1_max = std::max(vertex.y, poly1_max);
            }
        }

        float poly2_min = normal_axis.DotProduct(poly2[0]);
        float poly2_max = poly2_min;

        for (int i = 0; i < poly2_len; i++)
        {
            Vector2 vertex = poly2[i];
            if (normal_axis.x)
            {
                poly2_min = std::min(vertex.x, poly2_min);
                poly2_max = std::max(vertex.x, poly2_max);
            }
            if (normal_axis.y)
            {
                poly2_min = std::min(vertex.y, poly2_min);
                poly2_max = std::max(vertex.y, poly2_max);
            }
        }

        Vector2 distance = {
            poly1[0].x - poly2[0].x,
            poly1[0].y - poly2[0].y
        };

        float projected_distance = normal_axis.DotProduct(distance);

        poly1_min += projected_distance;
        poly1_max += projected_distance;

        bool gap_exists = ((poly1_min - poly2_max) > 0) || ((poly2_min - poly1_max) > 0);

        if (gap_exists)
        {
            free(poly1_sides);
            free(poly2_sides);
            return false;
        }
    }
    free(poly1_sides);
    free(poly2_sides);
    return true;
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

//FREE IT YOURSELF
Vector2* AABBToVertices(float x, float y, float w, float h)
{
    Vector2* vertices = (Vector2*)calloc(4, sizeof(Vector2));

    vertices[0] = {x, y};
    vertices[1] = {x + w, y};
    vertices[2] = {x, y + h};
    vertices[3] = {x + w, y + h};

    return vertices;
}