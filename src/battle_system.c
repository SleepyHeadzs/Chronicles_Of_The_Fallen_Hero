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

int IsBattleFinished(void)
{

}
