#include "raylib.h"
#include "intro.h"

typedef enum{
    MENU_NEW_GAME,
    MENU_RESUME,
    MENU_QUIT,
    MENU_COUNT
}MenuOption;
//3 Options
static int selected;
static bool finished;
static int result;
//Initialize
void InitIntro(void){
    selected = -1;
    finished = false;
    result = 0;
}

void UpdateIntro(void){
    if(finished) return;

    Vector2 mouse = GetMousePosition();
    int startY = 290;
    int gap = 62;
    selected = -1;

    for(int i=0; i<MENU_COUNT; i++){
        Rectangle btn = {490, (float)(startY + i*gap), 300, 48};

        if(CheckCollisionPointRec(mouse, btn)){ //mouse posn detect
            selected = i;

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ //check
                if(i == MENU_NEW_GAME){
                    finished = true;
                    result = 1;
                }
                else if(i == MENU_RESUME){
                    finished = true;
                    result = 2;
                }
                else if(i == MENU_QUIT){
                    finished = true;
                    result = 3;
                }
            }
        }
    }
}
//background, buttons
void DrawIntro(void){
    static Texture2D background = {0};

    if(background.id == 0)
        background = LoadTexture("intro_background.png");

    ClearBackground(BLACK);

    if(background.id != 0){
        Rectangle src = {0, 0, (float)background.width, (float)background.height};
        Rectangle dst = {0, 0, 1280, 720};

        DrawTexturePro(background, src, dst, (Vector2){0,0}, 0.0f, WHITE);
    }

    DrawRectangle(0, 0, 1280, 720, (Color){0,0,0,45});

    const char *labels[MENU_COUNT] = {"NEW GAME", "RESUME", "QUIT"};

    int startY = 290;
    int gap = 62;

    for(int i=0; i<MENU_COUNT; i++){
        int y = startY + i*gap;
        bool isSel = (i == selected);

        Color buttonColor;
        Color borderColor;
        float borderWidth;
        Color textColor;

        if(isSel){
            buttonColor = (Color){120,72,32,220};
            borderColor = GOLD;
            borderWidth = 2.0f;
            textColor = GOLD;
        }
        else{
            buttonColor = (Color){8,8,10,185};
            borderColor = (Color){190,165,125,190};
            borderWidth = 1.0f;
            textColor = RAYWHITE;
        }

        DrawRectangleRounded((Rectangle){490, (float)y, 300, 48}, 0.15f, 6, buttonColor);

        DrawRectangleRoundedLinesEx((Rectangle){490, (float)y, 300, 48}, 0.15f, 6, borderWidth, borderColor);

        int textX = 640 - MeasureText(labels[i], 28)/2;

        DrawText(labels[i], textX, y + 10, 28, textColor);
    }
}

int IsIntroFinished(void){
    if(!finished) return 0;
    else return result;
}
