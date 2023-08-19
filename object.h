
#ifndef __OBJECT
#define __OBJECT
#include "animation.h"

struct ObjectManager;

enum
{
    OBJECT_PLAYER,
    OBJECT_TILE,
    OBJECT_PROJECTILE,
    OBJECT_ABDO
};

enum
{
    COLLISION_AABB,
    COLLISION_SAT
};

struct Object
{
    int id = -1;
    int born_tic = 0;
    int type = -1;
    bool active = true;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    float scale = 1.0f; //drawing and collision scale
    ObjectManager* mng = nullptr;
    int collision_type = COLLISION_AABB;

    Object();

    virtual void Update() {};
    virtual void Draw(SDL_Renderer* renderer) {};
    virtual void OnCollision(Object* collider) {};
};


enum
{
    NONE,
    COL_DIR_UP,
    COL_DIR_DOWN,
    COL_DIR_LEFT,
    COL_DIR_RIGHT
};

struct Player : public Object
{
    Player(int x, int y, float scale);
    ~Player();

    int dir = 1;
    int collision_dir = NONE;
    int sprite_indices_index = 0;
    float velocx = 0;
    float velocy = 0;
    bool on_ground = false;
    int last_shot_elapsed = -1;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    Animation* anim = nullptr;
    int anim_play_tic = 0;
    bool is_shooting = false;

    const int speed = 4;
    const int jump_force = 6;

    void Update() override;
    void Draw(SDL_Renderer* renderer) override;
    void SetAnim(int anim_id);
    void Shoot();
};

struct Projectile : public Object
{
    Projectile(Object* parent, int x, int y, int w, int h, int dir, float scale, int speed = 8);

    Object* parent = nullptr;

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    int speed = 4;

    int dir = 1;

    void Update() override;
    void Draw(SDL_Renderer* renderer);
};

struct Abdo : public Object
{
    Abdo(int x, int y);

    int collision_dir = NONE;
    int dir = 0;
    Animation* anim = nullptr;
    bool on_ground = false;

    float velocx = 0;
    float velocy = 0;

    const int speed = 1;

    void Update() override;
    void Draw(SDL_Renderer* renderer);
    void OnCollision(Object* collider);
};

struct Tile : public Object
{
    Tile(int x, int y, int w, int h, float scale, int tile_x, int tile_y);
    int tile_x = 0;
    int tile_y = 0;

    void Draw(SDL_Renderer* renderer) override;
};

struct ObjectManager
{
    Object** _objects = nullptr;
    int objects_count = 0;
    int _objects_added = 0; //number of objects added since the start. used to generate ID

    int AddObject(Object* obj); //returns object id for future use
    Object* GetObject(int id);
    void DeleteObject(int id);
    void DeleteObject(Object* obj);
    void DeleteObjects(); //CALL IT BEFORE DELETING ObjectManager or if starting a new scene or smth
    void Update(); //calls update for every object
    void Draw(SDL_Renderer* renderer); //calls draw for every object

    Object* IsColliding(Object* obj);
};


#endif