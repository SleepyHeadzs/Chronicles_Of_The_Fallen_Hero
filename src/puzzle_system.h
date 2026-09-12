#ifndef PUZZLE_SYSTEM_H
#define PUZZLE_SYSTEM_H

void InitPuzzleSystem(int chapter); // Starts the puzzle for the given chapter
void UpdatePuzzleSystem(void); // Updates puzzle logic, input, timer, score, and lives
void DrawPuzzleSystem(void); // Draws the current puzzle interface on the screen

int IsPuzzleFinished(void); // Returns the current puzzle state
int GetPuzzleScore(void); // Returns the player's current puzzle score
int GetPuzzleLives(void); // Returns the player's remaining puzzle lives

#endif