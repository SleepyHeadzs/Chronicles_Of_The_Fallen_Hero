#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum {
    STATE_PROLOGUE,
    STATE_CH1_FOREST,
    STATE_COUNT
} GameState;

typedef struct {
    GameState current;
    GameState next;
    bool transitioning;
    float fadeAlpha;
} GameContext;

void Game_Init(GameContext *ctx);
void Game_Update(GameContext *ctx, float dt);
void Game_Draw(GameContext *ctx);
void Game_RequestTransition(GameContext *ctx, GameState next);

#endif
