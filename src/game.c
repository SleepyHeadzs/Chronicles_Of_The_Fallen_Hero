#include "raylib.h"
#include "dialogue.h"
#include "game.h"
#include "scene_ch1_forest.h"
#include "scene_ch2_burned_village.h"
#include "scene_ch3_diary.h"
#include "scene_ch4_truth.h"
#include "scene_ch5_final.h"
#include "ending_bad.h"
#include "ending_good.h"
#include "ending_secret.h"

typedef enum{
    STATE_PROLOGUE,
    STATE_CHAPTER1_FOREST,
    STATE_CHAPTER2_VILLAGE,
    STATE_CHAPTER3_DIARY,
    STATE_CHAPTER4_TRUTH,
    STATE_CHAPTER5_FINAL,
    STATE_CHOICE,
    STATE_ENDING_BAD,
    STATE_ENDING_GOOD,
    STATE_ENDING_SECRET
}GameState;

static GameState state;
static int choiceSelected = 0; // 0 = bad, 1 = good, 2 = secret

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
            if(IsDialogueFinishedCh1()){
                CloseDialogueCh1();
                InitDialogueCh2();
                state = STATE_CHAPTER2_VILLAGE;
            }
            break;

        case STATE_CHAPTER2_VILLAGE:
            UpdateDialogueCh2();
            if(IsDialogueFinishedCh2()){
                CloseDialogueCh2();
                InitDialogueCh3();
                state = STATE_CHAPTER3_DIARY;
            }
            break;

        case STATE_CHAPTER3_DIARY:
            UpdateDialogueCh3();
            if(IsDialogueFinishedCh3()){
                CloseDialogueCh3();
                InitDialogueCh4();
                state = STATE_CHAPTER4_TRUTH;
            }
            break;

        case STATE_CHAPTER4_TRUTH:
            UpdateDialogueCh4();
            if(IsDialogueFinishedCh4()){
                CloseDialogueCh4();
                InitDialogueCh5();
                state = STATE_CHAPTER5_FINAL;
            }
            break;

        case STATE_CHAPTER5_FINAL:
            UpdateDialogueCh5();
            if(IsDialogueFinishedCh5()){
                CloseDialogueCh5();
                choiceSelected = 0;
                state = STATE_CHOICE;
            }
            break;

        case STATE_CHOICE:
            if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
                choiceSelected--;
                if(choiceSelected < 0) choiceSelected = 2;
            }
            if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
                choiceSelected++;
                if(choiceSelected > 2) choiceSelected = 0;
            }
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                if(choiceSelected == 0){
                    InitDialogueBad();
                    state = STATE_ENDING_BAD;
                } else if(choiceSelected == 1){
                    InitDialogueGood();
                    state = STATE_ENDING_GOOD;
                } else {
                    InitDialogueSecret();
                    state = STATE_ENDING_SECRET;
                }
            }
            break;

        case STATE_ENDING_BAD:
            UpdateDialogueBad();
            break;

        case STATE_ENDING_GOOD:
            UpdateDialogueGood();
            break;

        case STATE_ENDING_SECRET:
            UpdateDialogueSecret();
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

        case STATE_CHAPTER2_VILLAGE:
            DrawDialogueBoxCh2();
            break;

        case STATE_CHAPTER3_DIARY:
            DrawDialogueBoxCh3();
            break;

        case STATE_CHAPTER4_TRUTH:
            DrawDialogueBoxCh4();
            break;

        case STATE_CHAPTER5_FINAL:
            DrawDialogueBoxCh5();
            break;

        case STATE_CHOICE:{
            ClearBackground(BLACK);
            DrawText("What does Tonoy do?", 400, 220, 32, YELLOW);

            const char *options[3] = {
                "1. Turn on the king alone  (Bad Ending)",
                "2. Confront the king with Sabin and Sakib  (Good Ending)",
                "3. Expose the king's secret to the whole court  (Secret Ending)"
            };

            for(int i=0; i<3; i++){
                Color c = (i==choiceSelected) ? YELLOW : WHITE;
                DrawText(options[i], 400, 300 + i*50, 22, c);
                if(i==choiceSelected) DrawText(">", 370, 300 + i*50, 22, YELLOW);
            }

            DrawText("[UP/DOWN to choose, ENTER to confirm]", 400, 500, 18, PURPLE);
            break;
        }

        case STATE_ENDING_BAD:
            DrawDialogueBoxBad();
            break;

        case STATE_ENDING_GOOD:
            DrawDialogueBoxGood();
            break;

        case STATE_ENDING_SECRET:
            DrawDialogueBoxSecret();
            break;
    }
}

void CloseGame(void){
    switch(state){
        case STATE_PROLOGUE:        CloseDialogue();       break;
        case STATE_CHAPTER1_FOREST: CloseDialogueCh1();    break;
        case STATE_CHAPTER2_VILLAGE:CloseDialogueCh2();    break;
        case STATE_CHAPTER3_DIARY:  CloseDialogueCh3();    break;
        case STATE_CHAPTER4_TRUTH:  CloseDialogueCh4();    break;
        case STATE_CHAPTER5_FINAL:  CloseDialogueCh5();    break;
        case STATE_CHOICE:          break;
        case STATE_ENDING_BAD:      CloseDialogueBad();    break;
        case STATE_ENDING_GOOD:     CloseDialogueGood();   break;
        case STATE_ENDING_SECRET:   CloseDialogueSecret(); break;
    }
}
