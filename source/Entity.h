#pragma once
#include "Crafting.h"
#include <stdlib.h>

// Entity types
#define ENTITY_NULL 0
#define ENTITY_ITEM 1
#define ENTITY_FURNITURE 2
#define ENTITY_ZOMBIE 3
#define ENTITY_SLIME 4
#define ENTITY_AIRWIZARD 5
#define ENTITY_SPARK 6
#define ENTITY_TEXTPARTICLE 7
#define ENTITY_SMASHPARTICLE 8
#define ENTITY_PLAYER 9

typedef struct Entity Entity;

typedef struct {
    s8 ax;
    s8 ay;
    u8 dir;
    s8 health;
    s8 stamina;
    s8 staminaRecharge;
    s8 staminaRechargeDelay;
    s8 attackTimer;
    u8 spawnTrigger;
    bool isDead;
    bool hasWon;
    bool hasWonSaved;
    s8 endTimer;
    s16 walkDist;
    bool isCarrying;
    bool isSwimming;
    int swimTimer;
	int score;
    Inventory* inv;
    Item* activeItem;
} Player;


typedef struct {
    float xa;
    float ya;
    float za;
    float xx;
    float yy;
    float zz;
    s16 age;
    Item item;
} EntityItem;

typedef struct {
    s16 itemID;
    bool active;
    s8 r; // light radius for lantern. window select for chests.
    Inventory* inv; // Points to chest inventory.
    s16 oSel; // other selection inside the chest inv.
} EntityFurniture;

typedef struct {
    s8 xa;
    s8 ya;
    s16 health;
    s8 dir;
    s8 lvl;
    s8 randWalkTime;
    s8 walkDist;
    u32 color;
} Zombie;

typedef struct {
    s8 xa;
    s8 ya;
    s16 health;
    s8 lvl;
    s8 dir;
    s8 jumpTime;
    u32 color;
} Slime;

typedef struct {
    s8 xa;
    s8 ya;
    s16 health;
    s8 randWalkTime;
    s8 walkDist;
    s8 dir;
	int attackDelay;
	int attackTime;
	int attackType;
	s8 spriteAdjust;
} AirWizard;

typedef struct {
    Entity* parent;
    s16 age;
    float xa;
    float ya;
    float xx;
    float yy;
} Spark;

typedef struct {
    float xa;
    float ya;
    float za;
    float xx;
    float yy;
    float zz;
    s16 age;
    char* text;
    int color;
} TextParticleEntity;

typedef struct {
    s16 age;
} SmashParticleEntity;

struct Entity {
    s16 x;
    s16 y;
    s8 xKnockback,yKnockback;
    u8 xr,yr;
    u8 type;
    u8 level;
    s8 hurtTime;
    s16 slotNum; // Read-only. Do not mess with this.
    bool canPass;
    bool canSwim;
    union {
        Player p;
        EntityItem entityItem;
        EntityFurniture entityFurniture;
        Zombie zombie;
        Slime slime;
        AirWizard wizard;
        Spark spark;
        TextParticleEntity textParticle;
        SmashParticleEntity smashParticle;
    };
};

typedef struct {
    Entity entities[5][1000];
    Entity wizardSparks[120];
    s16 lastSlot[5];
    Inventory invs[301];//1 for the player, 300 for chests.
    s16 nextInv;
} EntityManager;

EntityManager eManager;
s8 currentLevel;


double gaussrand();
Entity newItemEntity(Item item, int x, int y, int level);
Entity newFurnitureEntity(int itemID,Inventory * invPtr, int x, int y, int level);
Entity newZombieEntity(int lvl, int x, int y, int level);
Entity newSlimeEntity(int lvl, int x, int y, int level);
Entity newAirWizardEntity(int x, int y, int level);
Entity newSparkEntity(Entity* parent, float xa, float ya);
Entity newTextParticleEntity(char * str, u32 color, int xa, int ya, int level);
Entity newSmashParticleEntity(int xa, int ya, int level);
void addEntityToList(Entity e, EntityManager* em);
void removeEntityFromList(Entity * e,int level,EntityManager* em);



