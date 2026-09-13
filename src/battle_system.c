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
};

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



int IsBattleFinished(void)
{

}
