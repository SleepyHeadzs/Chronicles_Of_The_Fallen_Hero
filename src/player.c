#include "raylib.h"
#include "player.h"
#include "tilemap.h"

#define FRAME_COUNT 8 // total 8 frames - 0,1,2,3,4,5,6,7
#define FRAME_SIZE 100  // 100*100 for 1 frame

// initializing all player information
void InitPlayer(Player *p){
    // Starting position
    p->position = (Vector2){100,150};

    // Player movement
    p->speed = 50.0f;

    // Players 2D image
    p->sprite = LoadTexture("Soldier_Walk.png");

    // Animation
    p->frame = 0;
    p->frameTimer = 0.0f;

    // Direction
    p->islookingLeft = false;
}

//Move the player and updating animation
void UpdatePlayer(Player *p){

    float dt = GetFrameTime(); // GetFrameTime() calculates 1sec/frames
    bool isMoving = false;

    if(IsKeyDown(KEY_RIGHT)){ 
        p->position.x += p->speed*dt; 
        p->islookingLeft = false; 
        isMoving = true; 
    }
    if(IsKeyDown(KEY_LEFT)){ 
        p->position.x -= p->speed*dt; 
        p->islookingLeft = true;  
        isMoving = true; 
    }
    if(IsKeyDown(KEY_UP)){ 
        p->position.y -= p->speed*dt; 
        isMoving = true; 
    }
    if(IsKeyDown(KEY_DOWN)){ 
        p->position.y += p->speed*dt; 
        isMoving = true; 
    }

    if(p->position.x < 0) p->position.x = 0;
    if(p->position.y < 0) p->position.y = 0;

    if(p->position.x > (GetMapWidth() - FRAME_SIZE)) 
                p->position.x = GetMapWidth() - FRAME_SIZE;

    if(p->position.y > (GetMapHeight() - FRAME_SIZE)) 
                p->position.y = GetMapHeight() - FRAME_SIZE;

    if(isMoving){
        p->frameTimer += dt;
        if(p->frameTimer > 0.08f){
            p->frame = (p->frame + 1) % FRAME_COUNT;
            p->frameTimer = 0.0f;
        }
    } 
    else{
        p->frame = 0;
    }
}

void DrawPlayer(Player *p){
    float drawWidth;

    if(p->islookingLeft)
        drawWidth = -FRAME_SIZE; // flipped
    else
        drawWidth = FRAME_SIZE;

    //x,y,width,height
    Rectangle src = {
        (float)(p->frame * FRAME_SIZE),
        0,
        drawWidth,
        (float)FRAME_SIZE
    };

    //x,y,width,height
    Rectangle dest = {
        p->position.x,
        p->position.y,
        4*FRAME_SIZE,
        4*FRAME_SIZE
    };

    //Raylib function
    DrawTexturePro(
        p->sprite,
        src,
        dest,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}

void ClosePlayer(Player *p){
    UnloadTexture(p->sprite);
}
