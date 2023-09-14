#include "camera.h"
#include "main.h"
#include <stdio.h>
#include "object.h"


void Camera::AdjustPosToCamera(int* pos_x, int* pos_y)
{
    *pos_x -= screen_x;
    *pos_y -= screen_y;
}

void Camera::Update()
{
    if (target)
    {
        x = target->x;
        y = target->y;
    }


    int realx = (x + target->w) - screen_x;
    
    if (realx > WINDOW_WIDTH)
    {
        screen_x = x - WINDOW_WIDTH + target->w;
    }

    realx = (x - screen_x);
    if (realx < 0)
    {
        screen_x = x;
    }

}

void Camera::Follow(Object* object)
{
    if (object)
    {
        target = object;
    }
}