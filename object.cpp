#include "object.h"
#include "main.h"


Object::Object()
{
    born_tic = Gametic();
}



Object* ObjectManager::IsColliding(Object* obj)
{
    for (int i = 0; i < objects_count; i++)
    {
        Object* obj2 = _objects[i];
        if (obj2 && obj != obj2)
        {
            if (CheckAABBCol(obj->x, obj->y, obj->w, obj->h,
                             obj2->x, obj2->y, obj2->w, obj2->h))
            {
                return obj2;
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

//TODO() Clean up all inactive objects every few frames
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
    for (int i = 0; i < objects_count; i++)
    {
        if (_objects[i])
        {
            if (_objects[i]->active)
            {
                _objects[i]->Update();
            }
            else
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
            _objects[i]->Draw(renderer);
        }
    }
}

Player::Player(int x, int y, float scale)
{
    this->scale = scale;
    this->x = x;
    this->y = y;
    this->w = 12 * scale;
    this->h = 24 * scale;
}


void Player::Update()
{
    const uint8_t* state = SDL_GetKeyboardState(nullptr);

    if (!mng->IsColliding(this))
    {
        velocy += GRAVITY;
        y += velocy;
        Object* collider = mng->IsColliding(this);
        if (collider && !on_ground)
        {
            collision_dir = COL_DIR_DOWN;
            y = collider->y - h;
            velocy = 0;
            on_ground = true;
        }
        else
        {
            on_ground = false;
        }
    }

    if (state[SDL_SCANCODE_D])
    {
        velocx = speed;
        x += velocx;
        if (mng->IsColliding(this))
        {
            velocx = -speed;
            x += velocx;
            collision_dir = COL_DIR_RIGHT;
        }
    }

    else if (state[SDL_SCANCODE_A])
    {
        velocx = -speed;
        x += velocx;
        if (mng->IsColliding(this))
        {
            velocx = speed;
            x += velocx;
            collision_dir = COL_DIR_LEFT;
        }
    }

    else
    {
        velocx = 0;
    }

    if (state[SDL_SCANCODE_SPACE] && on_ground)
    {
        on_ground = false;
        velocy = -jump_force;
        y += velocy;
    }

    if (state[SDL_SCANCODE_X])
    {
        if (last_shot_elapsed > 30 || last_shot_elapsed < 0) //X tics since last shot or haven't shot
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
                proj_x -= 24;
            }
            
            int proj_y = y + (h / 2);
            Projectile* proj = new Projectile(this, proj_x, proj_y, 24, 12,
                                                dir, 1);
            mng->AddObject(proj);
            last_shot_elapsed = 0;
        }
    }
    if (last_shot_elapsed >= 0) //shot even once since start
    {
        last_shot_elapsed++;
    }
}

void Player::Draw(SDL_Renderer* renderer)
{
    AssetManager* asset_mng = GetAssetMng();
    Animation* anim = asset_mng->GetAnimation(ANIM_PLAYER);
    anim->speed = 7;


    if (velocx > 0)
    {
        flip = SDL_FLIP_NONE;
        anim->Advance(PLAYER_WALK_INDICES, sizeof(PLAYER_WALK_INDICES) / sizeof(int),
         &sprite_indices_index);
    }
    else if (velocx < 0)
    {
        flip = SDL_FLIP_HORIZONTAL;
        anim->Advance(PLAYER_WALK_INDICES, sizeof(PLAYER_WALK_INDICES) / sizeof(int),
         &sprite_indices_index);
    }
    else
    {
        anim->Advance(PLAYER_IDLE_INDICES, 1, &sprite_indices_index);
    }

    anim->flip = flip;

    anim->DrawTile(0, anim->current_frame, 0, renderer, x, y, w, h, flip);
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
}

void Tile::Draw(SDL_Renderer* renderer)
{
    AssetManager* asset_mng = GetAssetMng();
    Animation* anim = asset_mng->GetAnimation(ANIM_TILE);

    anim->DrawTile(0, tile_x, tile_y, renderer, x, y, w, h, SDL_FLIP_NONE);
}

Projectile::Projectile(Object* parent, int x, int y, int w, int h, int dir, float scale, int speed)
{
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->w = w;
    this->h = h;
    this->dir = dir;
    this->speed = speed;
}

void Projectile::Update()
{
    int tic = Gametic();

    this->x += speed * dir;

    if (mng->IsColliding(this))
    {
        active = false;
    }

    else if ((tic - born_tic) > 2000)
    {
        active = false;
    }
}


void Projectile::Draw(SDL_Renderer* renderer)
{
    AssetManager* asset_mng = GetAssetMng();
    Animation* anim = asset_mng->GetAnimation(ANIM_PLAYER);

    anim->DrawTile(0, 0, 0, renderer, x, y, w, h, SDL_FLIP_NONE);
}












