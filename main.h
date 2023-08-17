#ifndef __MAIN
#define __MAIN
#include "asset_manager.h"

int Gametic();
AssetManager* GetAssetMng();
bool CheckAABBCol(int x1, int y1, int w1, int h1,
                  int x2, int y2, int w2, int h2);

const float GRAVITY = 0.35f;

#endif