#include "raylib.h"
#include "tilemap.h"

static Texture2D mapTexture; //Only functions inside tilemap.c can use this variable.

void InitTilemap(void){
    mapTexture = LoadTexture("forest_map.png");
}

void DrawTilemap(void){
    DrawTexture(mapTexture, 0, 0, WHITE); //raylib func.
}

int GetMapWidth(void){ 
    return mapTexture.width; // returns map width
}
int GetMapHeight(void){ 
    return mapTexture.height; //returns map height
}

//free memory
void CloseTilemap(void){
    UnloadTexture(mapTexture);
}
