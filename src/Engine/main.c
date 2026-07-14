#include "raylib.h"
#include "game.h"

int main() {
    const int screenW = 1280, screenH = 720;
    InitWindow(screenW, screenH, "Chronicles_Of_The_Fallen_Hero");
    SetTargetFPS(60);

    GameContext ctx = {0};
    Game_Init(&ctx);

    while (WindowShouldClose()==0) {
        float dt = GetFrameTime();
        Game_Update(&ctx, dt);

        BeginDrawing();
            ClearBackground(BLACK);
            Game_Draw(&ctx);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
