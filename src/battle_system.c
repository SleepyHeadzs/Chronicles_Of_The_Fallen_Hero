#include "raylib.h"
#include <stdio.h>
#include "battle_system.h"
#include <math.h>

#define ARENA_X 160.0f
#define ARENA_W 960.0f
#define ARENA_H 720.0f
#define MAX_ENEMIES 5
#define MAX_PARTICLES 160
#define MAX_FIREBALLS 20
#define PI_F 3.14159265f

typedef enum
{
   FIGHTING,
   WON,
   LOST,
   CONFIRMED,
   FAILED_CONFIRMED
} BattleState;

typedef enum
{
    CHASE,
    ATTACK,
    RETREAT,
    STRAFE
} AI;

typedef struct
{
    Vector2 pos, vel;
    float radius;
    int hp, Maxhp, power, face;
    float swing, cooldown, hit, invincible, anim, think;
    int frame, dead, attackLanded;
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
    Vector2 pos, vel;
    float life, radius;
    int active;
} Fireball;

static Fighter player,enemies[MAX_ENEMIES];
static int enemyCount,chapterNumber,particleCount;
static Particle particles[MAX_PARTICLES];
static Fireball fireballs[MAX_FIREBALLS];
static Texture2D mapTexture,heroTexture,enemyTexture,bossTexture;
static int actorsLoaded;
static float enemySourceY,enemySourceH;
static BattleState state;
static float introTimer,shake,bossSpecialTimer,bossCastTime;
static int bossBattle,badEndingBattle;

static float Clampf(float v,float a,float b)
{
    if (v < a) 
    {
        return a;
    } 
    else if (v > b) 
    {
        return b;
    } 
    else
    {
        return v;
    }
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
        if(l>0.001f)
        {
            Vector2 result={v.x/l,v.y/l};
            return result;
        }
    else 
    {
        Vector2 result = { 0.0f, 0.0f };
        return result;
    }
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
    const char* heroPath = "Soldier_Walk.png"; // Default fallback

    if (FileExists("hero_sheet.png")) 
    {
        heroPath = "hero_sheet.png";
    } 
    else if (FileExists("src/hero_sheet.png")) 
    {
        heroPath = "src/hero_sheet.png";
    }
    Texture2D heroTexture = LoadTexture(heroPath);
    bossTexture=LoadBossTexture();
    actorsLoaded=true;
}

static void LoadChapterEnemy(int chapter)
{
    if(enemyTexture.id) UnloadTexture(enemyTexture);
    enemyTexture=(Texture2D){0};
    int enemyChapter;
    if(chapter>=1&&chapter<=5)
    {
        enemyChapter=chapter;
    }
    else
    {
        enemyChapter=5;
    }
    char name[32];
    snprintf(name,sizeof(name),"enemy_ch%d.png",enemyChapter);
    enemyTexture=LoadDarkSheet(name);
    if(enemyTexture.id==0)
    {
        enemyTexture=LoadTexture(Existing("enemy_sheet.png"));
    }
    if(enemyTexture.height>300)
    {
        enemySourceY=300.0f;
        enemySourceH=270.0f;
    }
    else
    {
        enemySourceY=0.0f;
        enemySourceH=(float)enemyTexture.height;
    }
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
    Fighter f={0};
    f.pos=(x,y);
    f.radius=radius;
    f.maxhp=hp;
    f.hp=hp;
    f.power=power;
    f.face=face;
    f.mode=CHASE;
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
        p->life=GetRandomValue(20,50)/60.0f;
        p->color=color;
    }
}

static void Move(Fighter *f,Vector2 direction,float accel,float dt)
{
    Vector2 n=Norm(direction);
    f->vel.x+=n.x*accel*dt;
    f->vel.y+=n.y*accel*dt;
    if(n.x!=0)
    {
        f->face=1;
    }
    else
    {
        f->face=-1;
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
    f->pos.x+=f->vel.x*dt;
    f->pos.y+=f->vel.y*dt;
    float damp=powf(.72f,dt*60);
    f->vel.x*=damp;f->vel.y*=damp;
    f->pos.x=Clampf(f->pos.x,55,905);
    f->pos.y=Clampf(f->pos.y,115,655);
    if(f->swing>0)f->swing-=dt;
    if(f->cooldown>0) f->cooldown-=dt;
    if(f->hit>0) f->hit-=dt;
    if(f->invincible>0) f->invincible-=dt;
    f->anim+=dt*(speed>20?10:4);
    f->frame=((int)f->anim)%8;
}

static void UpdatePlayer(float dt){
    if(player.dead)
    {
        Physics(&player,130,dt);
        return;
    }
    Vector2 input={(IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT))-(IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT)),(IsKeyDown(KEY_S)||IsKeyDown(KEY_DOWN))-(IsKeyDown(KEY_W)||IsKeyDown(KEY_UP))};
    if(player.hit<=0)Move(&player,input,470,dt);
    if(IsKeyPressed(KEY_SPACE)||IsKeyPressed(KEY_Z))StartAttack(&player,.34f,.52f);
    if(player.swing>0&&!player.attackLanded&&player.swing<=.18f)
    {
        for(int i=0;i<enemyCount;i++)
        {
           if(bossBattle)
           {
             Damage(&player,&enemies[i], 94);
           }
           else
           {
             Damage(&player,&enemies[i], 82);
           }
        }
        player.attackLanded=1;
    }
    Physics(&player,125,dt);
}

static void StartAttack(Fighter *f,float duration,float cooldown)
{
    if(f->cooldown<=0&&f->hit<=0&&!f->dead)
    {
        f->swing=duration;
        f->cooldown=cooldown;
        f->attackLanded=0;
    }
}
static void Damage(Fighter *a,Fighter *t,float range)
{
    if(t->dead||t->invincible>0) return;
    Vector2 d={t->pos.x-a->pos.x,t->pos.y-a->pos.y};
    int facing=(a->face>0&&d.x>=-12)||(a->face<0&&d.x<=12);
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

static void SeparateAll(void)
{
    for(int i=0;i<enemyCount;i++)
    {
        if(enemies[i].dead)continue;
        Vector2 d={enemies[i].pos.x-player.pos.x,enemies[i].pos.y-player.pos.y};
        float l=Len(d),m=enemies[i].radius+player.radius;
        if(l>.01f&&l<m)
        {
            float p=(m-l)/2;
            d.x/=l;
            d.y/=l;player.pos.x-=d.x*p;
            player.pos.y-=d.y*p;
            enemies[i].pos.x+=d.x*p;
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
            player.hp-=22;
            player.hit=.22f;
            player.invincible=.45f;
            SpawnParticles(player.pos,ORANGE,15);
            shake=9;
            f->active=false;
            if(player.hp<=0)
            {
                player.hp=0;
                player.dead=true;
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
        Physics(e,95,dt);return;
    }
    float distance=Dist(player.pos,e->pos);
    e->think-=dt;
    if(e->think<=0)
    {
        e->think=GetRandomValue(15,32)/60.0f;
        if(e->hit>0)e->mode=RETREAT;
        else if(player.swing>.15f&&distance<105&&GetRandomValue(0,99)<55)e->mode=STRAFE;
        else if(distance<66)e->mode=GetRandomValue(0,99)<70?ATTACK:RETREAT;
        else e->mode=CHASE;
    }
    if(e->hit<=0)
    {
        Vector2 toward={player.pos.x-e->pos.x,player.pos.y-e->pos.y};
        float accel=310+chapterNumber*8,maxSpeed=78+chapterNumber*3;
        if(e->mode==CHASE)
        {
            Move(e,toward,accel,dt);
            if(distance<82)StartAttack(e,.38f,.85f+index*.08f);
        }
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
    }else Physics(e,80,dt);
}

static void UpdateBoss(float dt){
    Fighter *b=&enemies[0];
    if(b->dead)
    {
        Physics(b,100,dt);
        return;
    }
    float distance=Dist(player.pos,b->pos);
    float phase;
    if(b->hp<b->maxHp/2)
    {
        phase=1.45f;
    }
    else
    {
        phase=1.0f;
    }
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
        
        if(phase>1)
        {
            bossSpecialTimer=1.15f;
        }
        else
        {
            bossSpecialTimer=1.8f;
        }
        bossCastTime=.55f;
        b->mode=STRAFE;
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
    if(player.pos.x>=b->pos.x)
    {
        b->face=1;
    }
    else
    {
        b->face=-1;
    }
    if(b->swing>0&&!b->attackLanded&&b->swing<=.23f)
    {
        Damage(b,&player,112);
        b->attackLanded=1;
    }
    if(bossCastTime>0)
    {
        bossCastTime-=dt;
    }
    Physics(b,115*phase,dt);
    UpdateFireballs(dt);
}

void UpdateBattleSystem(void)
{
    float dt=GetFrameTime();
    if(dt>.05f)dt=.05f;
    if(state==WON)
    {
        if(IsKeyPressed(KEY_ENTER)||IsKeyPressed(KEY_SPACE))state=CONFIRMED;
        return;
    }
    if(state==LOST){
        if(IsKeyPressed(KEY_ENTER)||IsKeyPressed(KEY_SPACE))state=FAILED_CONFIRMED;
        return;
    }
    if(state!=FIGHTING)return;
    if(introTimer>0)introTimer-=dt;
    else
    {
        UpdatePlayer(dt);
        if(bossBattle)UpdateBoss(dt);
        else for(int i=0;i<enemyCount;i++)
        {
            UpdateEnemy(&enemies[i],i,dt);
        }
        SeparateAll();
    }
    for(int i=particleCount-1;i>=0;i--)
    {
        Particle *p=&particles[i];
        p->pos.x+=p->vel.x*dt;
        p->pos.y+=p->vel.y*dt;
        p->vel.x*=.92f;
        p->vel.y*=.92f;
        p->life-=dt;
        if(p->life<=0)particles[i]=particles[--particleCount];
    }
    if(shake>0) shake*=.72f;
    int alive=0;
    for(int i=0;i<enemyCount;i++) if(!enemies[i].dead) alive++;
    if(alive==0) state=WON;
    else if(player.dead) state=LOST;
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
    if(!bossBattle)
    {
        LoadChapterEnemy(chapter);
    }
    particleCount=0;
    shake=0;
    introTimer=1.1f;
    bossSpecialTimer=2.5f;
    bossCastTime=0;
    for(int i=0; i<Max_Fireballs; i++)
    {
        fireballs[i].active=0;
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

static void DrawRegular(const Fighter *f,Texture2D tex,float size,bool enemyActor)
{
    float x=ARENA_X+f->pos.x,y=f->pos.y;
    DrawEllipse((int)x,(int)(y+19),24,8,(Color){0,0,0,100});
    if(tex.id==0)
    {
        DrawCircle((int)x,(int)y,(int)f->radius,f==&player?BLUE:MAROON);
        return;
    }
    int frame=f->swing>0?(int)Clampf(7-(f->swing/.38f)*7,0,7):f->frame;
    float frameW=enemyActor?tex.width/8.0f:100.0f,sourceY=enemyActor?enemySourceY:0,sourceH=enemyActor?enemySourceH:100.0f;
    float sx=f->face<0?(frame+1)*frameW:frame*frameW;
    Rectangle src={sx,sourceY,frameW*f->face,sourceH};
    Rectangle dst={x,y-31,size,size};
    Color tint=f->hit>0?(Color){255,145,145,255}:WHITE;
    if(f->invincible>0&&((int)(f->invincible*30)%2)==0)tint.a=120;
    DrawTexturePro(tex,src,dst,(Vector2){size/2,size/2},0,tint);
}
static void DrawBoss(const Fighter *b)
{
    float x=ARENA_X+b->pos.x,y=b->pos.y;
    DrawEllipse((int)x,(int)(y+34),47,15,(Color){0,0,0,130});
    if(bossTexture.id==0)
    {
        DrawCircle((int)x,(int)y,48,ORANGE);
        return;
    }
    int frame;
    if(b->dead)
    {
        frame=7;
    }
    else if(bossCastTime>0)
    {
        frame=bossCastTime>.28f?6:3;
    }
    else if(b->swing>0)
    {
        float progress=1.0f-b->swing/.48f;
        frame=progress<.34f?4:(progress<.72f?5:6);
    }
    else 
    {
        frame=((int)(b->anim*1.2f))%4;
    }
    float cw=bossTexture.width/8.0f,sourceX=b->face<0?(frame+1)*cw:frame*cw;
    Rectangle src={sourceX,300,(float)(cw*b->face),270};
    Rectangle dst={x,y-64,190,210};
    Color tint=b->hit>0?(Color){255,160,100,255}:WHITE;
    DrawTexturePro(bossTexture,src,dst,(Vector2){95,105},0,tint);
    if(b->hp<b->maxHp/2)
    {
        Color aura=(Color){255,95,20,80};
        DrawRing((Vector2){x,y},48,58,0,360,24,aura);
    }
}

static void DrawBar(int x,int y,int w,int value,int max,Color color,const char *label,bool right)
{
    DrawRectangle(x-3,y-3,w+6,23,(Color){4,12,10,235});
    DrawRectangle(x,y,w,17,(Color){45,38,35,255});
    int fill=(int)(w*(float)value/max);
    DrawRectangle(right?x+w-fill:x,y,fill,17,color);
    DrawRectangleLines(x,y,w,17,(Color){225,205,140,255});
    int tw=MeasureText(label,14);
    DrawText(label,right?x+w-tw:x,y-20,14,(Color){246,236,208,255});
}


void DrawBattleSystem(void){
    float ox, oy;
    if(shake>0)
    {
        ox=GetRandomValue((int)-shake,(int)shake).
        oy=GetRandomValue((int)-shake,(int)shake).
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
            drawOrder[j+1]=drawOrder[j];
            j--;
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
    if(state==CONFIRMED)return 1;
    if(state==FAILED_CONFIRMED)return -1;
    return 0;
}
