#include "raylib.h"
#include "dialogue.h"
#include "scene_ch1_forest.h"

typedef enum{
    STATE_PROLOGUE,
    STATE_CHAPTER1_FOREST
}GameState;

static GameState state;

void InitGame(void){
    state = STATE_PROLOGUE;
    InitDialogue();
}

void UpdateGame(void){
    switch(state){
        case STATE_PROLOGUE:
            UpdateDialogue();
            if(IsDialogueFinished()){
                CloseDialogue();
                InitDialogueCh1();
                state = STATE_CHAPTER1_FOREST;
            }
            break;

        case STATE_CHAPTER1_FOREST:
            UpdateDialogueCh1();
            break;
    }
}

void DrawGame(void){
    switch(state){
        case STATE_PROLOGUE:
            DrawDialogueBox();
            break;

        case STATE_CHAPTER1_FOREST:
            DrawDialogueBoxCh1();
            break;
    }
}

void CloseGame(void){
    if(state == STATE_PROLOGUE) CloseDialogue();
    else CloseDialogueCh1();
}
