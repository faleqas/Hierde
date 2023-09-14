#ifndef __MAIN
#define __MAIN
#include "asset_manager.h"
#include "vector.h"

int Gametic();
AssetManager* GetAssetMng();
bool CheckAABBCol(int x1, int y1, int w1, int h1,
                  int x2, int y2, int w2, int h2);
bool SATCollision(Vector2* poly1, int poly1_len,
                  Vector2* poly2, int poly2_len);
Vector2* AABBToVertices(float x, float y, float w, float h);
void DrawVertices(SDL_Renderer* renderer, Vector2* vertices, int len);

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float GRAVITY = 0.35f;

#endif