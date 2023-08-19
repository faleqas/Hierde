#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"


void Vector2::Print()
{
    printf("x: %f | y: %f\n", x, y);
}

float Vector2::Length()
{
    float length = sqrt((x * x) + (y * y));
    return length;
}

Vector2 Vector2::Normalize()
{
    Vector2 vector = {};
    float length = Length();
    vector.x = x / length;
    vector.y = y / length;
    return vector;
}

float Vector2::DotProduct(Vector2 v2)
{
    return (x * v2.x) + (y * v2.y);
}