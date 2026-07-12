#include "raylib.h"
#include "game.h"

int main(void)
{
    InitWindow(1280, 720, "Chronicles of the Fallen Hero");
    SetTargetFPS(60);

    InitGame();

    while (!WindowShouldClose())
    {
        UpdateGame();

        BeginDrawing();
        DrawGame();
        EndDrawing();
    }

    CloseGame();
    CloseWindow();
    return 0;
}
