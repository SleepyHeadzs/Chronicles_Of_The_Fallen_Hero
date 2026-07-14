#ifndef TILEMAP_H
#define TILEMAP_H
#include "raylib.h"

void InitTilemap(void);
void DrawTilemap(void);
void DrawMinimap(Vector2 playerPos);
int GetMapWidth(void);
int GetMapHeight(void);
void CloseTilemap(void);

#endif