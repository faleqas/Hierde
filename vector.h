#pragma once

struct Vector2
{
    float x = 0;
    float y = 0;

    float Length();
    Vector2 Normalize();
    void Print();
    float DotProduct(Vector2 v2);
};

struct Line
{
    Vector2 v1;
    Vector2 v2;
};