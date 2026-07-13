#include "raylib.h"
#include "tilemap.h"

static Texture2D mapTexture;
static Texture2D minimapTexture;

void InitTilemap(void){
    mapTexture = LoadTexture("forest_map.png");
    minimapTexture = LoadTexture("forest_map_mini.png");
}

void DrawTilemap(void){
    DrawTexture(mapTexture, 0, 0, WHITE);
}

void DrawMinimap(Vector2 playerPos){
    int mmX = 1280 - 160, mmY = 10, mmW = 150, mmH = 150;

    DrawRectangle(mmX-3, mmY-3, mmW+6, mmH+6, BLACK);
    DrawRectangleLines(mmX-3, mmY-3, mmW+6, mmH+6, WHITE);

    Rectangle src  = {0, 0, (float)minimapTexture.width, (float)minimapTexture.height};
    Rectangle dest = {(float)mmX, (float)mmY, (float)mmW, (float)mmH};
    DrawTexturePro(minimapTexture, src, dest, (Vector2){0,0}, 0.0f, WHITE);

    float scaleX = mmW / (float)mapTexture.width;
    float scaleY = mmH / (float)mapTexture.height;
    DrawCircle(mmX + playerPos.x*scaleX, mmY + playerPos.y*scaleY, 4, RED);
}

int GetMapWidth(void){ return mapTexture.width; }
int GetMapHeight(void){ return mapTexture.height; }

void CloseTilemap(void){
    UnloadTexture(mapTexture);
    UnloadTexture(minimapTexture);
}
