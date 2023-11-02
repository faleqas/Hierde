#include "map.h"
#include "main.h"
#include <stdio.h>


int LoadMap(ObjectManager* object_mng, const char* path)
{
    FILE* fp = nullptr;
    fopen_s(&fp, path, "rb");

    if (!fp)
    {
        return 1;
    }

    fseek(fp, 0, 0);

    int width = _getw(fp);
    int height = _getw(fp);

    if (!width)
    {
        return 2;
    }
    if (!height)
    {
        return 3;
    }
    printf("width is %d\n", width);

    int x = 0;
    int y = 0;

    bool made_player = false;

    while (!feof(fp))
    {
        int i = _getw(fp);
        if (x >= width)
        {
            x = 0;
            y++;
        }
        if (y >= height)
        {
            y = 0; //Would cause weird behavior. give out an error and abort the loading instead
        }

        switch (i)
        {
            case 1: //its the player in tiles.txt for the map edtior
            {
                object_mng->AddObject(new Player(x * TILE_WIDTH, y * TILE_HEIGHT, 3.0f));
            };
        };

        x++;
    }

    return 0;
}