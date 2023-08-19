#ifndef __MAIN
#define __MAIN
#include "asset_manager.h"

int Gametic();
AssetManager* GetAssetMng();
bool CheckAABBCol(int x1, int y1, int w1, int h1,
                  int x2, int y2, int w2, int h2);
bool SATCollision(float x1, float y1, float w1, float h1,
                  float x2, float y2, float w2, float h2);

const float GRAVITY = 0.35f;

#endif