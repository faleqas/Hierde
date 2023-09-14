#pragma once

typedef struct Object;

struct Camera
{
    int screen_x = 0; //Offset the object positions from these 2
    int screen_y = 0;
    int x = 0; //Global real position
    int y = 0;

    Object* target = nullptr;

    void AdjustPosToCamera(int* pos_x, int* pos_y);
    void Follow(Object* object);
    void Update();
};