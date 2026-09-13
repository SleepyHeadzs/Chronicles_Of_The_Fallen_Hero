#include "raylib.h"
#include <stdio.h>
#include "battle_system.h"

#define ArenaX 160.0f
#define ArenaW 960.0f
#define ArenaH 720.0f
#define Max_Enemies 5
#define Max_Fireballs 20
#define Max_Particles 160
#define PI 3.1415927f

typedef enum
{
    Battle,
    Win,
    Lost,
    Confirmed
} BattleStates;

typedef enum
{
    Chase,
    Attack,
    Back,
    Side
} AI;

typedef struct
{
    Vector2 pos, vel;
    float radius;
    int hp, Maxhp, power, face;
    float swing, cooldown, hit, invincible, dash, dashCooldown, animation, ai;
    int frame, dead, atksuc;
    AI mode;
} Fighter;

typedef struct
{
    Vector2 pos, vel;
    float life;
    Color color;
} Particle;

typedef struct
{
    Vector2 pos vel;
    float life, radius;
    int active;
} Fireball;

static Fighter player,enemies[MAX_ENEMIES];
static int enemyCount,chapterNumber,particleCount;
static Particle particles[MAX_PARTICLES];
static Fireball fireballs[MAX_FIREBALLS];
static Texture2D mapTexture,heroTexture,enemyTexture,bossTexture;
static int actorsLoaded;
static BattleState state;
static float introTimer,shake,bossSpecialTimer,bossCastTime;
static int bossBattle,badEndingBattle;

static Fighter NewFighter(float x, float y, int hp, int power, int face, float radius)
{
    Fighter F={0};
    F.pos=(x,y);
    F.radius=radius;
    F.maxhp=hp;
    F.hp=hp;
    F.face=face;
    F.mode=Chase;
    return f;
}

void InitBattleSystem(int chapter)
{
    chapterNumber=chapter;
    if(chapter==7)
    {
        bossBattle=1;
    }
    else
    {
        bossBattle=0;
    }
    if(chapter==6)
    {
        badEndingBattle=1;
    }
    else
    {
        badEndingBattle=0;
    }
    LoadActors();
    LoadChapterMap(chapter);
    particleCount=0;
    shake=0;
    introTimer=1.1f;
    bossSpecialTimer=2.5f;
    bossCastTime=0;
    for(int i=0; i<Max_Fireballs; i++)
    {
        fireballs[i].active=1;
    }
    int heroHp;
    if(chapter<=1)
    {
        heroHp=150;
    }
    else if(chapter==2)
    {
        heroHp=200;
    }
    else if(chapter==3)
    {
        heroHp=270;
    }
    else if(chapter==4)
    {
        heroHp=300;
    }
    else if(chapter==5)
    {
        heroHp=340;
    }
    else if(bossBattle)
    {
        heroHp=380;
    }
    else
    {
        heroHp=230;
    }
    player=NewFighter(250,500,heroHp,28,1,22);
    if(chapter>=1&&chapter<=5)
    {
        enemyCount=chapter;
    }
    else
    {
        enemyCount=1;
    }
    if(bossBattle)
    {
        enemies[0]=NewFighter(690,285,720,26,-1,48);
    }
    else
    {
        int hp, power;
        if(badEndingBattle)
        {
            hp=180;
            power=19;
        }
        else
        {
            hp=45+chapter*10;
            power=6+chapter*2;
        }
        static const Vector2 spawn[MAX_ENEMIES]={{690,260},{760,460},{520,230},{590,540},{830,350}};
        for(int i=0;i<enemyCount;i++)
        {
            enemies[i]=NewFighter(spawn[i].x,spawn[i].y,hp,power,-1,20);
            enemies[i].think=i*.12f;
        }
    }
    state=Battle;
}

void DrawBattleSystem(void){
    float ox, oy;
    if(shake>0)
    {
        ox=GetRandomValue(int)-shake;
        oy=GetRandomValue(int)-shake;
    }
    else
    {
        ox=0;
        oy=0;
    }
    Fighter *drawOrder[MAX_ENEMIES+1];
    int drawCount=0;
    drawOrder[drawCount++]=&player;
    for(int i=0;i<enemyCount;i++)
    {
        if(!enemies[i].dead)drawOrder[drawCount++]=&enemies[i];
    }
    for(int i=1;i<drawCount;i++)
    {
        Fighter *item=drawOrder[i];
        int j=i-1;
        while(j>=0&&drawOrder[j]->pos.y>item->pos.y)
        {
            drawOrder[j+1]=drawOrder[j];j--;
        }
        drawOrder[j+1]=item;
    }
    for(int i=0;i<drawCount;i++)
    {
        if(drawOrder[i]==&player)
        DrawRegular(&player,heroTexture,150,false);
        else if(bossBattle)
        {
            DrawBoss(drawOrder[i]);
        }
        else 
        {
            DrawRegular(drawOrder[i],enemyTexture,132,true);
        }
    }
    for(int i=0;i<MAX_FIREBALLS;i++)
    {
        if(fireballs[i].active)
        {
            DrawCircle((int)(ARENA_X+fireballs[i].pos.x),(int)fireballs[i].pos.y,15,(Color){255,90,20,100});
            DrawCircle((int)(ARENA_X+fireballs[i].pos.x),(int)fireballs[i].pos.y,8,ORANGE);
        }
    }
    for(int i=0;i<particleCount;i++)
    {
        Color c=particles[i].color;
        c.a=(unsigned char)Clampf(particles[i].life*500,0,255);
        DrawRectangle((int)(ARENA_X+particles[i].pos.x),(int)particles[i].pos.y,4,4,c);
    }
    DrawBar(28,45,350,player.hp,player.maxHp,(Color){65,181,96,255},"TONOY",false);
    int totalHp=0,totalMax=0,alive=0;
    for(int i=0;i<enemyCount;i++)
    {
        totalHp+=enemies[i].hp;totalMax+=enemies[i].maxHp;
        if(!enemies[i].dead)
        {
            alive++;
        }
    }
    char label[80];
    if(bossBattle)
    {
        snprintf(label,sizeof(label),"REVENGE BOSS - PHASE %d",enemies[0].hp<enemies[0].maxHp/2?2:1);
    }
    else if(badEndingBattle)
    {
        snprintf(label,sizeof(label),"FINAL DUEL");
    }
    else 
    {
        snprintf(label,sizeof(label),"CHAPTER %d - ENEMIES %d/%d",chapterNumber,alive,enemyCount);
    }
    DrawBar(902,45,350,totalHp,totalMax,bossBattle?(Color){238,112,30,255}:(Color){200,68,81,255},label,true);
    DrawText("WASD / ARROWS  Move",22,650,14,RAYWHITE);
    DrawText("SPACE / Z  Sword",22,672,14,RAYWHITE);
    DrawText("SHIFT / X  Dash",22,694,14,RAYWHITE);
    if(introTimer>0)
    {
        DrawRectangle(465,300,350,82,(Color){3,12,9,225});
        DrawRectangleLines(465,300,350,82,GOLD);
        const char *start=bossBattle?"BOSS FIGHT":badEndingBattle?"FINAL DUEL":"FIGHT!";
        int tw=MeasureText(start,38);
        DrawText(start,640-tw/2,324,38,(Color){243,223,163,255});
    }
    if(state==WON||state==LOST)
    {
        Color c=state==WON?GREEN:RED;
        const char *title=state==WON?"VICTORY":"DEFEATED",*hint=state==WON?"Press ENTER to continue":"Press ENTER to attempt the quiz";
        DrawRectangle(0,0,1280,720,(Color){0,0,0,180});
        int tw=MeasureText(title,64);
        DrawText(title,640-tw/2,275,64,c);
        tw=MeasureText(hint,24);DrawText(hint,640-tw/2,370,24,WHITE);
    }
}

int IsBattleFinished(void)
{

}
