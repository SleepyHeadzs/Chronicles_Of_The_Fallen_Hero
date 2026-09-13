#ifndef DIALOGUE_STYLE_H
#define DIALOGUE_STYLE_H
#include "raylib.h"

#define DIALOGUE_TITLE_COLOR ((Color){255, 235, 80, 255})
#define DIALOGUE_SPEAKER_COLOR ((Color){80, 220, 255, 255})
#define DIALOGUE_TEXT_COLOR ((Color){255, 255, 245, 255})

static inline void DrawDialogueBoldText(const char *text, int x, int y, int fontSize, Color color){
    Color outline = (Color){0, 0, 0, 235};
    DrawText(text, x - 2, y,     fontSize, outline);
    DrawText(text, x + 2, y,     fontSize, outline);
    DrawText(text, x,     y - 2, fontSize, outline);
    DrawText(text, x,     y + 2, fontSize, outline);
    DrawText(text, x - 1, y - 1, fontSize, outline);
    DrawText(text, x + 1, y + 1, fontSize, outline);
    DrawText(text, x,     y,     fontSize, color);
    DrawText(text, x + 1, y,     fontSize, color);
}

#define DrawText(text, x, y, size, color) \
    DrawDialogueBoldText((text), (x), (y), (size), (color))

static inline Rectangle DialogueSkipBounds(void){
    return (Rectangle){1150, 84, 100, 38};
}

static inline bool DialogueSkipClicked(void){
    Rectangle button=DialogueSkipBounds();
    return CheckCollisionPointRec(GetMousePosition(),button) &&
           IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

static inline void DrawDialogueSkipButton(void){
    Rectangle button=DialogueSkipBounds();
    bool hover=CheckCollisionPointRec(GetMousePosition(),button);
    Color buttonColor;
    Color borderColor;
    Color textColor;
    if(hover){
        buttonColor=(Color){105,66,30,225};
        borderColor=GOLD;
        textColor=GOLD;
    }
    else{
        buttonColor=(Color){0,0,0,175};
        borderColor=(Color){220,205,170,210};
        textColor=RAYWHITE;
    }
    DrawRectangleRounded(button,0.18f,6,buttonColor);
    DrawRectangleRoundedLinesEx(button,0.18f,6,1.5f,borderColor);
    DrawText("SKIP",1175,93,20,textColor);
}

#endif
