#include "raylib.h"
#include "dialogue.h"
#include "dialogue_style.h"
#include "scene_ch5_final.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"CHAPTER V — The Forgotten Oath (Finale)", "Location", "Location: The Shattered Throne — Ashfeld", "5 (1).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "The road to Ashfeld is the same road it's always been. The stones are the same stones. The way the valley opens up past the last hill — Tonoy remembers it the way you remember the moment right before your life changed.", "5 (1).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "He's walked this in his mind so many times that the real place feels like a memory of a memory. Smaller than it should be. Quieter.", "5 (1).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "He sang here. The night before the battle. His voice was terrible and he had no idea. He sang like he was alone in a room.", "5 (2).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sakib", "I remember.", "5 (2).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "I used to laugh at him. Told him he'd wake the enemy. He'd say — 'Good. Let them come. I'm not hiding.'", "5 (2).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "A pause.", "5 (2).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "I didn't understand what that meant until much later. He was never hiding from anything. Not even at the end. Not even when we left him behind.", "5 (2).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "Sabin stands on Ashfeld's broken dais when they arrive. He looks the way fifteen years of war, captivity, and abandonment look when they decide to keep walking anyway: broken, but upright. His eyes, when they find Tonoy, are the eyes of someone who has already decided not to be surprised by anything.", "5 (3).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "Then he's surprised.", "5 (3).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "You came.", "5 (4).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "Did you think I wouldn't?", "5 (4).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "I thought you might. I didn't let myself believe it.", "5 (4).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "I searched for you straight from Ashfeld. Three years, before I stopped. And when I stopped, it wasn't surrender. Someone convinced me you were gone.", "5 (4).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "The king.", "5 (4).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "Yes. The king. We're going back to him.", "5 (4).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "He stops in front of Sabin. Close enough that there's no ambiguity about whether this is confrontation or reunion. He chooses reunion.", "5 (5).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "You wrote my name on my door.", "5 (5).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "I wanted to know if you'd come.", "5 (5).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "You already knew.", "5 (5).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "I needed to see it.", "5 (5).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "Tonoy turns to Sakib. This is the moment he's prepared for and feared. He isn't speaking to make peace — he's speaking to tell the truth, the thing neither of them has ever been able to say in front of Sabin.", "5 (6).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "What Sakib did at Ashfeld — the king forced him to choose between you and me. He chose me. I'm not asking you to be fine with that. I'm asking you to understand it wasn't betrayal. It was a man forced to make an impossible decision in the dark, and he chose the one he thought he could live with.", "5 (6).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "He didn't live with it. Not for one day. He's carried it every day since. If you want to be angry at someone, be angry at the one who handed him that choice.", "5 (6).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "Sabin looks at Sakib for a long time. Sakib doesn't look away. He's stopped looking away.", "5 (7).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "You were crying. At Ashfeld. I saw it. I didn't understand it then. I thought it meant you knew you were doing something wrong.", "5 (7).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sakib", "I knew I was doing something wrong. That's why I was crying.", "5 (7).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "I've spent fifteen years angry at the wrong person.", "5 (7).png"},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "No. You were angry at the right person, just the wrong order. Fix the order. Start with the king.", "5 (7).png"},
};

static int total = 29;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH5_DIALOGUE,
    CH5_FINAL,
}Ch5State;

static Ch5State ch5State;

static Texture2D GetTexture(const char *name){
    if(name[0] == '\0') return (Texture2D){0};
    for(int i=0; i<textureCount; i++){
        if(strcmp(textureNames[i], name)==0) return textureCache[i];
    }
    textureCache[textureCount] = LoadTexture(name);
    textureNames[textureCount] = name;
    textureCount++;
    return textureCache[textureCount-1];
}

static void DrawWrapped(const char *text, int x, int y, int maxWidth, int fontSize, Color color){
    char buffer[4096];
    int lineY = y;
    int lineStart = 0;
    int lastSpace = -1;
    int len = strlen(text);

    for(int i=0; i<=len; i++){
        if(text[i]==' ' || text[i]=='\0'){
            int testLen = i - lineStart;
            if(testLen>0 && testLen<(int)sizeof(buffer)){
                strncpy(buffer, text+lineStart, testLen);
                buffer[testLen] = '\0';
                if(MeasureText(buffer, fontSize) > maxWidth && lastSpace > lineStart){
                    int cut = lastSpace - lineStart;
                    strncpy(buffer, text+lineStart, cut);
                    buffer[cut] = '\0';
                    DrawText(buffer, x, lineY, fontSize, color);
                    lineY += fontSize + 8;
                    lineStart = lastSpace + 1;
                }
            }
            lastSpace = i;
        }
    }
    if(lineStart < len) DrawText(text+lineStart, x, lineY, fontSize, color);
}

void InitDialogueCh5(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch5State = CH5_DIALOGUE;
}
void UpdateDialogueCh5(void){
    if(ch5State == CH5_DIALOGUE){
        if(DialogueSkipClicked()){
            ch5State = CH5_FINAL;
            return;
        }
        if(IsKeyPressed(KEY_SPACE)){
            if(current < total-1){
                current++;
                if(lines[current].title[0] != '\0') currentTitle = lines[current].title;
                currentBg = GetTexture(lines[current].image);
            } 
            else{
                ch5State = CH5_FINAL;
            }
        }
    } 
}

void DrawDialogueBoxCh5(void){
    if(ch5State == CH5_DIALOGUE){
        DrawTexture(currentBg, 0, 0, WHITE);

        DrawText(currentTitle, 70, 35, 24, DIALOGUE_TITLE_COLOR);

        DrawText(lines[current].speaker, 70, 515, 26, DIALOGUE_SPEAKER_COLOR);
        DrawWrapped(lines[current].text, 70, 550, 1140, 22, DIALOGUE_TEXT_COLOR);
        DrawDialogueSkipButton();

    } 
}

int IsDialogueFinishedCh5(void){
    return ch5State == CH5_FINAL;
}

void CloseDialogueCh5(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
}
