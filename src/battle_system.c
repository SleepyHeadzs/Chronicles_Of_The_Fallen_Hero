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

static float Clampf(float v,float a,float b)
{
    return v<a?a:(v>b?b:v);
}
static float Len(Vector2 v)
{
    return sqrtf(v.x*v.x+v.y*v.y);
}
static float Dist(Vector2 a,Vector2 b)
{
    return Len((Vector2){a.x-b.x,a.y-b.y});
}
static Vector2 Norm(Vector2 v)
{
    float l=Len(v);
    return l>.001f?(Vector2){v.x/l,v.y/l}:(Vector2){0,0};
}
static const char *Existing(const char *name)
{
    static char rooted[128];
    if(FileExists(name))
    {
        return name;
    }
    snprintf(rooted,sizeof(rooted),"src/%s",name);
    return FileExists(rooted)?rooted:name;
}

static Texture2D LoadBossTexture(void)
{
    Image image=LoadImage(Existing("boss_sheet.png"));
    if(image.data)
    {
        Color *pixels=LoadImageColors(image);
        int count=image.width*image.height;
        for(int i=0;i<count;i++)
        {
            if(pixels[i].r<18&&pixels[i].g<18&&pixels[i].b<18)
            {
                pixels[i].a=0;
            }
        }
        Image transparent={pixels,image.width,image.height,1,PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
        Texture2D result=LoadTextureFromImage(transparent);
        UnloadImageColors(pixels);
        UnloadImage(image);
        return result;
    }
    return (Texture2D){0};
}

static Texture2D LoadDarkSheet(const char *name)
{
    Image image=LoadImage(Existing(name));
    if(!image.data) return (Texture2D){0};
    Color *pixels=LoadImageColors(image);
    int count=image.width*image.height;
    for(int i=0;i<count;i++)
    {
        if(pixels[i].r<18&&pixels[i].g<18&&pixels[i].b<18)pixels[i].a=0;
    }
    Image transparent={pixels,image.width,image.height,1,PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    Texture2D result=LoadTextureFromImage(transparent);
    UnloadImageColors(pixels);
    UnloadImage(image);
    return result;
}

static void LoadActors(void)
{
    if(actorsLoaded)return;
    heroTexture=LoadTexture(Existing(FileExists("hero_sheet.png")||FileExists("src/hero_sheet.png")?"hero_sheet.png":"Soldier_Walk.png"));
    bossTexture=LoadBossTexture();
    actorsLoaded=true;
}

static void LoadChapterEnemy(int chapter)
{
    if(enemyTexture.id) UnloadTexture(enemyTexture);
    enemyTexture=(Texture2D){0};
    int enemyChapter=chapter>=1&&chapter<=5?chapter:5;
    char name[32];
    snprintf(name,sizeof(name),"enemy_ch%d.png",enemyChapter);
    enemyTexture=LoadDarkSheet(name);
    if(enemyTexture.id==0)
    {
        enemyTexture=LoadTexture(Existing("enemy_sheet.png"));
    }
    enemySourceY=enemyTexture.height>300?300.0f:0.0f;
    enemySourceH=enemyTexture.height>300?270.0f:(float)enemyTexture.height;
}

static void LoadChapterMap(int chapter)
{
    if(mapTexture.id)UnloadTexture(mapTexture);
    char name[40];
    if(chapter>=1&&chapter<=5)
    {
        snprintf(name,sizeof(name),"ch%d.png",chapter);
    }
    else if(chapter==6)
    {
        snprintf(name,sizeof(name),"ch_bad.png");
    }
    else
    {
        snprintf(name,sizeof(name),"ch_boss.png");
    }
    mapTexture=LoadTexture(Existing(name));
    if(mapTexture.id==0)
    {
        mapTexture=LoadTexture(Existing("forest_map.png"));
    }
}

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

static void SpawnParticles(Vector2 pos,Color color,int count)
{
    for(int i=0;i<count&&particleCount<MAX_PARTICLES;i++)
    {
        float a=GetRandomValue(0,359)*PI_F/180,s=GetRandomValue(45,170);
        Particle *p=&particles[particleCount++];
        p->pos=pos;
        p->vel=(Vector2){cosf(a)*s,sinf(a)*s};
        p->life=GetRandomValue(20,50)/60.0f;p->color=color;
    }
}

static void Move(Fighter *f,Vector2 direction,float accel,float dt)
{
    Vector2 n=Norm(direction);f->vel.x+=n.x*accel*dt;
    f->vel.y+=n.y*accel*dt;
    if(n.x!=0)
    {
        f->face=n.x>0?1:-1;
    }
}
static void Physics(Fighter *f,float maxSpeed,float dt)
{
    float speed=Len(f->vel);
    if(speed>maxSpeed)
    {
        f->vel.x=f->vel.x/speed*maxSpeed;
        f->vel.y=f->vel.y/speed*maxSpeed;
    }
    f->pos.x+=f->vel.x*dt;f->pos.y+=f->vel.y*dt;float damp=powf(f->dash>0?.90f:.72f,dt*60);
    f->vel.x*=damp;f->vel.y*=damp;
    f->pos.x=Clampf(f->pos.x,55,905);
    f->pos.y=Clampf(f->pos.y,115,655);
    if(f->swing>0)f->swing-=dt;
    if(f->cooldown>0) f->cooldown-=dt;
    if(f->hit>0) f->hit-=dt;
    if(f->invincible>0) f->invincible-=dt;
    if(f->dash>0) f->dash-=dt;
    if(f->dashCooldown>0) f->dashCooldown-=dt;
    f->anim+=dt*(speed>20?10:4);f->frame=((int)f->anim)%8;
}

static void UpdatePlayer(float dt){
    if(player.dead)
    {
        Physics(&player,130,dt);
        return;
    }
    Vector2 input={(IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT))-(IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT)),(IsKeyDown(KEY_S)||IsKeyDown(KEY_DOWN))-(IsKeyDown(KEY_W)||IsKeyDown(KEY_UP))};
    if(player.hit<=0)Move(&player,input,470,dt);
    if((IsKeyPressed(KEY_LEFT_SHIFT)||IsKeyPressed(KEY_RIGHT_SHIFT)||IsKeyPressed(KEY_X))&&player.dashCooldown<=0&&Len(input)>0){Vector2 n=Norm(input);player.vel=(Vector2){n.x*260,n.y*260};player.dash=.15f;player.dashCooldown=1.15f;player.invincible=.2f;}
    if(IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_Z))StartAttack(&player,.34f,.52f);
    if(player.swing>0&&!player.attackLanded&&player.swing<=.18f)
    {
        for(int i=0;i<enemyCount;i++)
        {
            Damage(&player,&enemies[i],bossBattle?94:82);
        }
        player.attackLanded=true;
    }
    Physics(&player,player.dash>0?260:125,dt);
}

static void StartAttack(Fighter *f,float duration,float cooldown)
{
    if(f->cooldown<=0&&f->hit<=0&&!f->dead)
    {
        f->swing=duration;
        f->cooldown=cooldown;
        f->attackLanded=false;
    }
}
static void Damage(Fighter *a,Fighter *t,float range)
{
    if(t->dead||t->invincible>0) return;
    Vector2 d={t->pos.x-a->pos.x,t->pos.y-a->pos.y};
    bool facing=(a->face>0&&d.x>=-12)||(a->face<0&&d.x<=12);
    if(Dist(a->pos,t->pos)<=range&&facing)
    {
        t->hp-=a->power;
        if(t->hp<0)t->hp=0;
        t->hit=.24f;
        t->invincible=.36f;
        Vector2 n=Norm(d);
        t->vel=(Vector2){n.x*190,n.y*145};
        SpawnParticles(t->pos,a==&player?ORANGE:RED,18);
        shake=10;if(t->hp<=0)t->dead=true;
    }
}

static void SeparateAll(void)\
{
    for(int i=0;i<enemyCount;i++)
    {
        if(enemies[i].dead)continue;
        Vector2 d={enemies[i].pos.x-player.pos.x,enemies[i].pos.y-player.pos.y};
        float l=Len(d),m=enemies[i].radius+player.radius;
        if(l>.01f&&l<m)
        {
            float p=(m-l)/2;d.x/=l;
            d.y/=l;player.pos.x-=d.x*p;
            player.pos.y-=d.y*p;enemies[i].pos.x+=d.x*p;
            enemies[i].pos.y+=d.y*p;
        }
    }
    for(int i=0;i<enemyCount;i++)
    {
        for(int j=i+1;j<enemyCount;j++)
        {
            if(enemies[i].dead||enemies[j].dead)continue;
            Vector2 d={enemies[j].pos.x-enemies[i].pos.x,enemies[j].pos.y-enemies[i].pos.y};
            float l=Len(d),m=enemies[i].radius+enemies[j].radius;
            if(l>.01f&&l<m)
            {
                float p=(m-l)/2;d.x/=l;d.y/=l;
                enemies[i].pos.x-=d.x*p;e
                nemies[i].pos.y-=d.y*p;
                enemies[j].pos.x+=d.x*p;
                enemies[j].pos.y+=d.y*p;
            }
        }
    }
}

static void SpawnFireball(Vector2 origin,Vector2 target)
{
    for(int i=0;i<MAX_FIREBALLS;i++)
    {
        if(!fireballs[i].active)
        {
            Vector2 n=Norm((Vector2){target.x-origin.x,target.y-origin.y});
            fireballs[i]=(Fireball){origin,{n.x*185,n.y*185},4.5f,12,true};
            break;
        }
    }
}

static void UpdateFireballs(float dt)
{
    for(int i=0;i<MAX_FIREBALLS;i++)
    {
        Fireball *f=&fireballs[i];
        if(!f->active)continue;
        f->pos.x+=f->vel.x*dt;
        f->pos.y+=f->vel.y*dt;
        f->life-=dt;
        if(Dist(f->pos,player.pos)<f->radius+player.radius&&player.invincible<=0)
        {
            player.hp-=22;player.hit=.22f;player.invincible=.45f;
            SpawnParticles(player.pos,ORANGE,15);shake=9;f->active=false;
            if(player.hp<=0)
            {
                player.hp=0;player.dead=true;
            }
        }
        if(f->life<=0||f->pos.x<20||f->pos.x>940||f->pos.y<80||f->pos.y>700)
        {
            f->active=false;
        }
    }
}

static void UpdateEnemy(Fighter *e,int index,float dt)
{
    if(e->dead)
    {
        Physics(e,95,dt);
        return;
    }
    float distance=Dist(player.pos,e->pos);
    e->think-=dt;
    if(e->think<=0)
    {
        e->think=GetRandomValue(15,32)/60.0f;
        if(e->hit>0)e->mode=RETREAT;
        else if(player.swing>.15f&&distance<105&&GetRandomValue(0,99)<55)e->mode=STRAFE;
        else if(distance<66)e->mode=GetRandomValue(0,99)<70?ATTACK:RETREAT;
        else e->mode=CHASE;}
        if(e->mode==CHASE)
        {
            Move(e,toward,accel,dt);
            if(distance<82)StartAttack(e,.38f,.85f+index*.08f);
        }
        if(e->hit<=0)
        {
            Vector2 toward={player.pos.x-e->pos.x,player.pos.y-e->pos.y};
            float accel=310+chapterNumber*8,maxSpeed=78+chapterNumber*3;
            else if(e->mode==ATTACK)
        {
            Move(e,toward,accel*.3f,dt);
            StartAttack(e,.38f,.85f+index*.08f);
        }
        else if(e->mode==RETREAT)Move(e,(Vector2){-toward.x,-toward.y},accel,dt);
        else Move(e,(Vector2){-toward.y,toward.x},accel*1.15f,dt);
        e->face=player.pos.x>=e->pos.x?1:-1;
        if(e->swing>0&&!e->attackLanded&&e->swing<=.18f)
        {
            Damage(e,&player,72);
            e->attackLanded=true;
        }
        Physics(e,maxSpeed,dt);
    }
    else Physics(e,80,dt);
}

static void UpdateBoss(float dt){
    Fighter *b=&enemies[0];
    if(b->dead)
    {
        Physics(b,100,dt);
        return;
    }
    float distance=Dist(player.pos,b->pos),phase=b->hp<b->maxHp/2?1.45f:1.0f;
    Vector2 toward={player.pos.x-b->pos.x,player.pos.y-b->pos.y};
    bossSpecialTimer-=dt;
    if(bossSpecialTimer<=0)
    {
        SpawnFireball(b->pos,player.pos);
        if(phase>1)
        {
            Vector2 side={player.pos.x+GetRandomValue(-150,150),player.pos.y+GetRandomValue(-120,120)};
            SpawnFireball(b->pos,side);
        }
        bossSpecialTimer=(phase>1?1.15f:1.8f);
        bossCastTime=.55f;b->mode=STRAFE;
    }
    if(b->hit<=0)
    {
        if(distance>105)Move(b,toward,360*phase,dt);
        else if(b->cooldown<=0)
        {
            StartAttack(b,.48f,.9f/phase);
        }
        else 
        {
            Move(b,(Vector2){-toward.y,toward.x},270*phase,dt);
        }
    } 
    b->face=player.pos.x>=b->pos.x?1:-1;
    if(b->swing>0&&!b->attackLanded&&b->swing<=.23f)
    {
        Damage(b,&player,112);
        b->attackLanded=true;
    }
    if(bossCastTime>0)
    {
        bossCastTime-=dt;
    }
    Physics(b,115*phase,dt);
    UpdateFireballs(dt);
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
    if(mapTexture.id)
    {
        DrawTexturePro(mapTexture,(Rectangle){0,0,(float)mapTexture.width,(float)mapTexture.height},(Rectangle){ox,oy,1280,720},(Vector2){0,0},0,WHITE);
    }
    else 
    {
        DrawRectangle(0,0,1280,720,DARKGREEN);
        DrawRectangle(0,0,1280,720,(Color){0,8,4,45});
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
