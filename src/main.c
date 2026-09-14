#include "raylib.h"
#include "game.h"

bool ShouldGameQuit(void);

int main(void)
{
    InitWindow(1280, 720, "Chronicles of the Fallen Hero");
    if (!IsWindowReady())
        return 1;
    SetTargetFPS(60);
    InitAudioDevice();
    InitGame();

    while (!WindowShouldClose())
    {
        UpdateGame();
        if (ShouldGameQuit())
            break;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawGame();
        EndDrawing();
    }

    CloseGame();
    if (IsAudioDeviceReady())
        CloseAudioDevice();
    CloseWindow();
    return 0;
}
