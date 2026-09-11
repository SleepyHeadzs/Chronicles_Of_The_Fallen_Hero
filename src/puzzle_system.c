#include "raylib.h"
#include "puzzle_system.h"
#include <string.h>

typedef struct{
    const char *question;
    const char *options[4];
    int correctAnswer;
}QuizQuestion;