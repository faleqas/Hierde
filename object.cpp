#include "object.h"
#include "main.h"
#include <stdio.h>


Object::Object()
{
    born_tic = Gametic();
}

Object* Object::TestMove(float velocx, float velocy)
{
    if (mng)
    {
        x += velocx;
        y += velocy;

        Object* obj = mng->IsColliding(this);
        if (obj)
        {
            x -= velocx;
            y -= velocy;
            return obj;
        }
        else
        {
            x -= velocx;
            y -= velocy;
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

//This function always uses AABB collision for performance reasons
//In the object's OnCollision function you can use SATCollision if needed
Object* ObjectManager::IsColliding(Object* obj)
{
    for (int i = 0; i < objects_count; i++)
    {
        Object* obj2 = _objects[i];
        if (obj2 && obj != obj2)
        {
            {
                if (CheckAABBCol(obj->x, obj->y, obj->w, obj->h,
                                obj2->x, obj2->y, obj2->w, obj2->h))
                {
                    return obj2;
                }
            }
        }
    }
    return nullptr;
}

int ObjectManager::AddObject(Object* obj)
{
    _objects_added++;
    obj->id = (_objects_added << 8) | 0xEF;
    obj->mng = this;

    for (int i = 0; i < objects_count; i++)
    {
        if (_objects[i] == nullptr)
        {
            _objects[i] = obj;
            return obj->id;
        }
    }

    objects_count++;
    if (_objects)
    {
        _objects = (Object**)realloc(_objects, sizeof(Object*) * objects_count);
    }
    else
    {
        _objects = (Object**)malloc(sizeof(Object*) * objects_count);
    }
    _objects[objects_count-1] = obj;

    if (obj->type == OBJECT_PLAYER)
    {
        player = (Player*)obj;
    }
    return obj->id;
}

Object* ObjectManager::GetObject(int id)
{
    for (int i = 0; i < objects_count; i++)
    {
        if (_objects[i]->id == id)
        {
            return _objects[i];
        }
    }
    return nullptr;
}

void ObjectManager::DeleteObject(int id)
{
    for (int i = 0; i < objects_count; i++)
    {
        if (_objects[i]->id == id)
        {
            delete _objects[i];
            _objects[i] = nullptr;
        }
    }
}

void ObjectManager::DeleteObject(Object* obj)
{
    for (int i = 0; i < objects_count; i++)
    {
        if (_objects[i] == obj)
        {
            delete obj;
            _objects[i] = nullptr;
        }
    }
}

void ObjectManager::DeleteObjects()
{
    for (int i = 0; i < objects_count; i++)
    {
        if (_objects[i])
        {
            delete _objects[i];
        }
    }
    delete _objects;
    _objects = nullptr;
}


void ObjectManager::Update()
{
    int tic = Gametic();
    bool clean_inactive = false;
    if (tic % 10)
    {

    }
    else
    {
        clean_inactive = true;
    }

    camera.Follow(player);
    camera.Update();

    for (int i = 0; i < objects_count; i++)
    {
        if (_objects[i])
        {
            if (_objects[i]->active)
            {
                _objects[i]->Update();
            }
            else if (clean_inactive)
            {
                delete _objects[i];
                _objects[i] = nullptr;
            }
        }
    }
}

void ObjectManager::Draw(SDL_Renderer* renderer)
{
    for (int i = 0; i < objects_count; i++)
    {
        if (_objects[i] && _objects[i]->active)
        {
            _objects[i]->Draw(renderer, &camera);
        }
    }
}

Player::Player(float x, float y, float scale)
{
    born_tic = Gametic();
    this->scale = scale;
    this->x = x;
    this->y = y;
    this->w = 14 * scale;
    this->h = 24 * scale;
    this->draw_h = h;
    this->draw_w = w;
    this->type = OBJECT_PLAYER;
    this->collision_type = COLLISION_SAT;
}


void CollidingObject::Move(float velocity_x, float velocity_y)
{
    if (!TestMove(velocx, 0))
    {
        x += velocx;
    }
    else
    {
        Object* col = TestMove(velocx, 0);
        if (col)
        {
            float feet_y = (y + h);
            if ((feet_y - col->y) > 5)
            {
                if (velocx > 0)
                {
                    x = col->x - w;
                }
                else if (velocx < 0)
                {
                    x = col->x + col->w;
                }
            }
        }
    }
    
    Object* col = TestMove(0, velocy);
    if (!col && velocy)
    {
        y += velocy;
        on_ground = false;
    }
    else
    {
        if (velocy > 0)
        {
            on_ground = true;
            
            if (col)
            {
                float feet_y = (y + h);
                if ((feet_y - col->y) > 0)
                {
                    y = col->y - h;
                    on_ground = true;
                }
            }
        }
    }
}


void Player::Update()
{
    const uint8_t* state = SDL_GetKeyboardState(nullptr);

    is_shooting = (last_shot_elapsed < 38) && (last_shot_elapsed > 0);

    printf("%f, %d\n", velocy, on_ground);

    if (!on_ground)
    {
        if (!is_shooting)
        {
            velocy += gravity;
        }
        else if (velocy > 0)
        {
            velocy += gravity;
        }
    }
    else
    {
        {
            velocy = 0;
        }
        if (TestMove(0, 2) == NULL)
        {
            on_ground = false;
        }
    }

    if (state[SDL_SCANCODE_D])
    {
        velocx += acceleration;
        if (velocx >= speed)
        {
            velocx = speed;
        }
        dir = 1;
    }
    else if (state[SDL_SCANCODE_A])
    {
        velocx -= acceleration;
        if (velocx <= -speed)
        {
            velocx = -speed;
        }
        dir = -1;
    }
    else
    {
        if (velocx < 0)
        {
            velocx += friction;
            if (velocx > 0)
            {
                velocx = 0;
            }
        }
        else if (velocx > 0)
        {
            velocx -= friction;
            if (velocx < 0)
            {
                velocx = 0;
            }
        }
    }

    if (state[SDL_SCANCODE_SPACE] && on_ground)
    {
        velocy = -jump_force;
    }

    if (is_shooting)
    {
        velocx = 0;
        if (velocy <= 0)
        {
            velocy = 0;
        }
    }

    {
        Move(velocx, velocy);
    }

    if (state[SDL_SCANCODE_X])
    {
        if (!is_shooting) //X tics since last shot or haven't shot
        {
            last_shot_elapsed = 0;
        }
    }
    if (last_shot_elapsed >= 0) //shot even once since start
    {
        last_shot_elapsed++;
    }

    SquashAndStretch();
}

void Player::SquashAndStretch()
{
    if (!on_ground && velocy > 0)
    {
        if (draw_h < (h * 1.2f))
        {
            draw_h += 1;
        }
        if (draw_w > (w * 0.95f))
        {
            draw_w -= 1;
        }
    }
    else
    {
        if (draw_h > (h))
        {
            draw_h -= 2;
        }
        if (draw_w < (w))
        {
            draw_w += 2;
        }
    }
}

void Player::Draw(SDL_Renderer* renderer, Camera* camera)
{
    if (is_shooting)
    {
        SetAnim(ANIM_PLAYER_SHOOT);
    }
    else
    {
        if (dir == 1)
        {
            flip = SDL_FLIP_NONE;
        }
        else
        {
            flip = SDL_FLIP_HORIZONTAL;
        }
        if (velocx > 0)
        {
            SetAnim(ANIM_PLAYER_RUN);
            flip = SDL_FLIP_NONE;

            anim->Advance(anim_play_tic,
            PLAYER_RUN_INDICES, sizeof(PLAYER_RUN_INDICES) / sizeof(int),
            &sprite_indices_index);
        }
        else if (velocx < 0)
        {
            SetAnim(ANIM_PLAYER_RUN);
            flip = SDL_FLIP_HORIZONTAL;

            anim->Advance(anim_play_tic,
            PLAYER_RUN_INDICES, sizeof(PLAYER_RUN_INDICES) / sizeof(int),
            &sprite_indices_index);
        }
        else
        {
            SetAnim(ANIM_PLAYER_IDLE);
            anim->Advance(anim_play_tic,
            PLAYER_IDLE_INDICES, sizeof(PLAYER_IDLE_INDICES) / sizeof(int), &sprite_indices_index);
        }
    }

    anim->flip = flip;

    float draw_x = x;
    float draw_y = y;

    float offset_x = (w - draw_w) / 2;
    float offset_y = (h - draw_h) / 2;

    draw_x += offset_x;
    draw_y += offset_y;

    switch (anim->id)
    {
        case ANIM_PLAYER_SHOOT:
        {
            //14
            //22
            //32
            // if (anim->current_frame == 3)
            // {
            //     draw_w = 22 * scale;
            //     if (flip == SDL_FLIP_HORIZONTAL)
            //     {
            //         draw_x -= draw_w - w;
            //     }
            // }
            anim->loop = false;
            if (anim->since_done == 7)
            {
                Shoot(); //WHY IS THIS IN DRAW()
            }
        } break;

        default:
        {
            
        } break;
    };

    int int_draw_x = (int)draw_x;
    int int_draw_y = (int)draw_y;

    if (camera)
    {
        camera->AdjustPosToCamera(&int_draw_x, &int_draw_y);
    }

    anim->Play(renderer, anim_play_tic, int_draw_x, int_draw_y, draw_w, draw_h);

    anim_play_tic++;
}

void Player::SetAnim(int anim_id)
{
    if (!anim || (anim->id != anim_id))
    {
        AssetManager* asset_mng = GetAssetMng();
        sprite_indices_index = 0;
        anim = asset_mng->GetAnimation(anim_id);
        anim->Restart();
        anim_play_tic = 0;
    }
}

void Player::Shoot()
{
    int dir = 1;
    if (flip == SDL_FLIP_HORIZONTAL)
    {
        dir = -1;
    }
    int proj_x = x;

    if (dir == 1)
    {
        proj_x += w;
    }
    else
    {
        proj_x -= w / 2;
    }
    
    int proj_y = y + 4 * scale;
    Projectile* proj = new Projectile(this, proj_x, proj_y, 4, 2,
                                        dir, scale);
    mng->AddObject(proj);
}

Tile::Tile(int x, int y, int w, int h, float scale, int tile_x, int tile_y)
{
    this->scale = scale;
    this->x = x;
    this->y = y;
    this->w = w * scale;
    this->h = h * scale;
    this->tile_x = tile_x;
    this->tile_y = tile_y;
    this->type = OBJECT_TILE;
}

void Tile::Draw(SDL_Renderer* renderer, Camera* camera)
{
    AssetManager* asset_mng = GetAssetMng();
    Animation* anim = asset_mng->GetAnimation(ANIM_TILE);

    int draw_x = x;
    int draw_y = y;

    if (camera)
    {
        camera->AdjustPosToCamera(&draw_x, &draw_y);
    }

    anim->DrawTile(0, tile_x, tile_y, renderer, draw_x, draw_y, w, h, SDL_FLIP_NONE);
}

Projectile::Projectile(Object* parent, float x, float y, int w, int h, int dir, float scale, float speed)
{
    born_tic = Gametic();
    printf("new projectile\n");
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->w = w * scale;
    this->h = h * scale;
    this->dir = dir;
    this->speed = speed;
    this->type = OBJECT_PROJECTILE;
}

void Projectile::Update()
{
    int tic = Gametic();

    this->x += speed * dir;

    Object* collider = mng->IsColliding(this);
    if (collider)
    {
        active = false;
        printf("dead\n");
        collider->OnCollision(this);

    }

    else if ((tic - born_tic) > 2000)
    {
        active = false;
        printf("dead\n");
    }
}


void Projectile::Draw(SDL_Renderer* renderer, Camera* camera)
{
    AssetManager* asset_mng = GetAssetMng();
    Animation* anim = asset_mng->GetAnimation(ANIM_LASER_BULLET);

    int draw_x = x;
    int draw_y = y;

    if (camera)
    {
        camera->AdjustPosToCamera(&draw_x, &draw_y);
    }

    anim->Play(renderer, 0, draw_x, draw_y, w, h);
}