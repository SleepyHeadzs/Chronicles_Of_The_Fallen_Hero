#ifndef DIALOGUE_STYLE_H
#define DIALOGUE_STYLE_H

#include "raylib.h"

#define DIALOGUE_TITLE_COLOR ((Color){255, 235, 80, 255})
#define DIALOGUE_SPEAKER_COLOR ((Color){80, 220, 255, 255})
#define DIALOGUE_TEXT_COLOR ((Color){255, 255, 245, 255})

// A faux-bold treatment using a dark outline. It keeps dialogue readable over
// bright or detailed backgrounds without covering the artwork with a box.
static inline void DrawDialogueBoldText(const char *text, int x, int y,
                                        int fontSize, Color color)
{
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

static inline Rectangle DialogueSkipBounds(void)
{
    return (Rectangle){1150, 84, 100, 38};
}

static inline bool DialogueSkipClicked(void)
{
    Rectangle button=DialogueSkipBounds();
    return CheckCollisionPointRec(GetMousePosition(),button) &&
           IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

static inline void DrawDialogueSkipButton(void)
{
    Rectangle button=DialogueSkipBounds();
    bool hover=CheckCollisionPointRec(GetMousePosition(),button);
    DrawRectangleRounded(button,0.18f,6,
        hover?(Color){105,66,30,225}:(Color){0,0,0,175});
    DrawRectangleRoundedLinesEx(button,0.18f,6,1.5f,
        hover?GOLD:(Color){220,205,170,210});
    DrawText("SKIP",1175,93,20,hover?GOLD:RAYWHITE);
}

#endif
