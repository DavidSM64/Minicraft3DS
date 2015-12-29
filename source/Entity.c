#include "Entity.h"

#define PI 3.141592654
double gaussrand()
{
	static double U, V;
	static int phase = 0;
	double Z;

	if(phase == 0) {
		U = (rand() + 1.) / (RAND_MAX + 2.);
		V = rand() / (RAND_MAX + 1.);
		Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
	} else
		Z = sqrt(-2 * log(U)) * cos(2 * PI * V);

	phase = 1 - phase;

	return Z;
}

Entity newItemEntity(Item item, int x, int y, int level){
    Entity e;
    e.type = ENTITY_ITEM;
    e.level = level;
    e.entityItem.age = 0;
    e.entityItem.item = item;
    e.x = x;
    e.y = y;
    e.xr = 3;
    e.yr = 3;
    e.canPass = false;
    
	e.entityItem.xx = x;
	e.entityItem.yy = y;
	e.entityItem.zz = 2;
	e.entityItem.xa = gaussrand() * 0.1;
	e.entityItem.ya = gaussrand() * 0.1;
	e.entityItem.za = ((float)rand() / RAND_MAX) * 0.45 + 1;
    
    return e;
}

void assignInventory(Entity* e){
    if(eManager.nextInv > 300) return;
    e->entityFurniture.inv = &eManager.invs[eManager.nextInv];
    eManager.nextInv++;
}

Entity newFurnitureEntity(int itemID,Inventory * invPtr, int x, int y, int level){
    Entity e;
    e.type = ENTITY_FURNITURE;
    e.level = level;
    e.x = x;
    e.y = y;
    e.xr = 3;
    e.yr = 3;
    e.entityFurniture.itemID = itemID;
    e.canPass = false;
    if(itemID == ITEM_LANTERN) e.entityFurniture.r = 8;
    else if(itemID == ITEM_CHEST){ 
        if(invPtr == NULL)assignInventory(&e);
        else e.entityFurniture.inv = invPtr;
    }
    return e;
}


Entity newZombieEntity(int lvl, int x, int y, int level){
    Entity e;
    e.type = ENTITY_ZOMBIE;
    e.level = level;
    e.x = x;
    e.y = y;
    e.hurtTime = 0;
    e.xKnockback = 0;
    e.yKnockback = 0;
    e.zombie.lvl = lvl;
    e.zombie.health = lvl * lvl * 10;
    e.zombie.dir = 0;
    e.xr = 4;
    e.yr = 3;
    e.canPass = false;
    switch(lvl){
        case 2: e.zombie.color = 0xFF8282CC; break;
        case 3: e.zombie.color = 0xFFEFEFEF; break;
        case 4: e.zombie.color = 0xFFAA6262; break;
        default: e.zombie.color = 0xFF95DB95; break;
    }
    return e;
}

Entity newSlimeEntity(int lvl, int x, int y, int level){
    Entity e;
    e.type = ENTITY_SLIME;
    e.level = level;
    e.x = x;
    e.y = y;
    e.hurtTime = 0;
    e.xKnockback = 0;
    e.yKnockback = 0;
    e.slime.lvl = lvl;
    e.slime.xa = 0;
    e.slime.ya = 0;
    e.slime.dir = 0;
    e.slime.health = lvl * lvl * 5;
    e.xr = 4;
    e.yr = 3;
    e.canPass = false;
    switch(lvl){
        case 2: e.slime.color = 0xFF8282CC; break;
        case 3: e.slime.color = 0xFFEFEFEF; break;
        case 4: e.slime.color = 0xFFAA6262; break;
        default: e.slime.color = 0xFF95DB95; break;
    }
    return e;
}

Entity newAirWizardEntity(int x, int y, int level){
    Entity e;
    e.type = ENTITY_AIRWIZARD;
    e.level = level;
    e.x = x;
    e.y = y;
    e.hurtTime = 0;
    e.xKnockback = 0;
    e.yKnockback = 0;
    e.wizard.dir = 0;
    e.wizard.health = 2000;
	e.wizard.attackDelay = 0;
	e.wizard.attackTime = 0;
	e.wizard.attackType = 0;
    e.wizard.xa = 0;
    e.wizard.ya = 0;
    e.xr = 4;
    e.yr = 3;
    e.canPass = true;
    return e;
}

Entity newSparkEntity(Entity* parent, float xa, float ya){
    Entity e;
    e.type = ENTITY_SPARK;
    e.spark.age = 0;
    e.spark.parent = parent;
    e.spark.xa = xa;
    e.spark.ya = ya;
    e.spark.xx = parent->x;
    e.spark.yy = parent->y;
    e.xr = 3;
    e.yr = 3;
    e.canPass = true;
    return e;
}

Entity newTextParticleEntity(char * str, u32 color, int x, int y, int level){
    Entity e;
    e.type = ENTITY_TEXTPARTICLE;
    e.level = level;
    e.textParticle.color = color;
    e.textParticle.age = 0;
    e.textParticle.text = (char*)calloc(strlen(str),sizeof(char)); 
    strncpy(e.textParticle.text,str,strlen(str)); 
    e.x = x;
    e.y = y;
    e.canPass = true;
	e.textParticle.xx = x;
	e.textParticle.yy = y;
	e.textParticle.zz = 2;
	e.textParticle.xa = gaussrand() * 0.3;
	e.textParticle.ya = gaussrand() * 0.2;
	e.textParticle.za = ((float)rand() / RAND_MAX) * 0.7 + 2;
    
    return e;
}
Entity newSmashParticleEntity(int x, int y, int level){
    Entity e;
    e.type = ENTITY_SMASHPARTICLE;
    e.level = level;
    e.smashParticle.age = 0;
    e.x = x;
    e.y = y;
    e.canPass = true;
    playSound(snd_monsterHurt);
    return e;
}

void addEntityToList(Entity e, EntityManager* em){
    e.slotNum = em->lastSlot[e.level];
    em->entities[e.level][em->lastSlot[e.level]] = e;
    ++em->lastSlot[e.level];
}

Entity nullEntity;
void removeEntityFromList(Entity * e,int level,EntityManager* em){
    int i;
    if(em->entities[level][e->slotNum].type == ENTITY_TEXTPARTICLE) free(em->entities[level][e->slotNum].textParticle.text);
    for(i = e->slotNum; i < em->lastSlot[level];++i){
        em->entities[level][i] = em->entities[level][i + 1]; // Move the items down.
        em->entities[level][i].slotNum = i;
    }
    em->lastSlot[level]--;
    em->entities[level][em->lastSlot[level]] = nullEntity; // Make the last slot null.
}
