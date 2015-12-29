#include "SaveLoad.h"

s16 calculateImportantEntites(EntityManager * eManager, int level){
    int i;
    s16 count = 0;
    for(i = 0; i < eManager->lastSlot[level]; ++i){
        switch(eManager->entities[level][i].type){
            case ENTITY_AIRWIZARD:
            case ENTITY_SLIME:
            case ENTITY_ZOMBIE:
            case ENTITY_ITEM:
            case ENTITY_FURNITURE:
                count++;
                break;
        }
    }
    return count;
}

bool entityIsImportant(Entity * e){
    switch(e->type){
        case ENTITY_AIRWIZARD:
        case ENTITY_SLIME:
        case ENTITY_ZOMBIE:
        case ENTITY_ITEM:
        case ENTITY_FURNITURE:
            return true;
        default:
            return false;
    }
}

void saveCurrentWorld(char * filename, EntityManager * eManager, Entity * player, u8 * map, u8 * mapData){
    FILE * file = fopen(filename, "wb");
    int i,j;

    // Player Data
    fwrite(&player->p.score, sizeof(int), 1, file);
    fwrite(&player->p.hasWonSaved, sizeof(bool), 1, file);
    fwrite(&player->p.health, sizeof(s16), 1, file);
    fwrite(&player->x, sizeof(s16), 1, file);
    fwrite(&player->y, sizeof(s16), 1, file);
    fwrite(&currentLevel, sizeof(s8), 1, file);
    
    // Inventory Data
    fwrite(&eManager->nextInv, sizeof(s16), 1, file); // write amount of inventories.
    for(i = 0; i < eManager->nextInv; ++i) {
        fwrite(&eManager->invs[i].lastSlot, sizeof(s16), 1, file); // write amount of items in inventory;
        for(j = 0; j < eManager->invs[i].lastSlot; ++j) {
            fwrite(&eManager->invs[i].items[j].id, sizeof(s16), 1, file); // write ID of item
            fwrite(&eManager->invs[i].items[j].countLevel, sizeof(s16), 1, file); // write count/level of item
            fwrite(&eManager->invs[i].items[j].onlyOne, sizeof(bool), 1, file);
            if(eManager->invs[i].items[j].id == ITEM_CHEST){
                int invIndex = eManager->invs[i].items[j].chestPtr - eManager->invs;
                fwrite(&invIndex, sizeof(int), 1, file);
            }
        }
    }
    
    // Entity Data
    for(i = 0; i < 5; ++i){
        int amount = calculateImportantEntites(eManager,i);
        fwrite(&amount, sizeof(s16), 1, file); // read amount of entities in level.
        for(j = 0; j < eManager->lastSlot[i]; ++j){
            if(!entityIsImportant(&eManager->entities[i][j])) continue;
            fwrite(&eManager->entities[i][j].type, sizeof(s16), 1, file); // write entity's type ID
            fwrite(&eManager->entities[i][j].x, sizeof(s16), 1, file); // write entity's x coordinate
            fwrite(&eManager->entities[i][j].y, sizeof(s16), 1, file); // write entity's y coordinate
            switch(eManager->entities[i][j].type){
                case ENTITY_AIRWIZARD:
                    fwrite(&eManager->entities[i][j].wizard.health, sizeof(s16), 1, file);
                    break;
                case ENTITY_SLIME:
                    fwrite(&eManager->entities[i][j].slime.health, sizeof(s16), 1, file);
                    fwrite(&eManager->entities[i][j].slime.lvl, sizeof(s8), 1, file);
                    break;
                case ENTITY_ZOMBIE:
                    fwrite(&eManager->entities[i][j].zombie.health, sizeof(s16), 1, file);
                    fwrite(&eManager->entities[i][j].zombie.lvl, sizeof(s8), 1, file);
                    break;
                case ENTITY_ITEM:
                    fwrite(&eManager->entities[i][j].entityItem.item.id, sizeof(s16), 1, file);
                    fwrite(&eManager->entities[i][j].entityItem.item.countLevel, sizeof(s16), 1, file);
                    fwrite(&eManager->entities[i][j].entityItem.age, sizeof(s16), 1, file);
                    break;
                case ENTITY_FURNITURE:
                    fwrite(&eManager->entities[i][j].entityFurniture.itemID, sizeof(s16), 1, file);
                    int invIndex = eManager->entities[i][j].entityFurniture.inv - eManager->invs;
                    fwrite(&invIndex, sizeof(int), 1, file);
                    break;
            }
        }
    }
    
    // Map Data
    fwrite(map, sizeof(u8), 128*128*5, file); // Map Tile IDs, 128*128*5 bytes = 80KB
    fwrite(mapData, sizeof(u8), 128*128*5, file); // Map Tile Data (Damage done to trees/rocks, age of wheat & saplings, etc). 80KB
    
    fclose(file);
}

int loadWorld(char * filename, EntityManager * eManager, Entity * player, u8 * map, u8 * mapData){
    FILE * file;
    int i,j;
        
        if ((file = fopen(filename, "rb"))){
            
            fread(&player->p.score, sizeof(int), 1, file);
            fread(&player->p.hasWonSaved, sizeof(bool), 1, file);
            fread(&player->p.health, sizeof(s16), 1, file);
            fread(&player->x, sizeof(s16), 1, file);
            fread(&player->y, sizeof(s16), 1, file);
            fread(&currentLevel, sizeof(s8), 1, file);
            
            fread(&eManager->nextInv, sizeof(s16), 1, file);
            for(i = 0; i < eManager->nextInv; ++i) {
                fread(&eManager->invs[i].lastSlot, sizeof(s16), 1, file); // read amount of items in inventory;
                for(j = 0; j < eManager->invs[i].lastSlot; ++j) {
                    fread(&eManager->invs[i].items[j].id, sizeof(s16), 1, file); // write ID of item
                    fread(&eManager->invs[i].items[j].countLevel, sizeof(s16), 1, file); // write count/level of item
                    fread(&eManager->invs[i].items[j].onlyOne, sizeof(bool), 1, file);
                    eManager->invs[i].items[j].invPtr = (int*)&eManager->invs[i]; // setup Inventory pointer
                    eManager->invs[i].items[j].slotNum = j; // setup slot number
                    if(eManager->invs[i].items[j].id == ITEM_CHEST){ // for chest item specifically.
                        int invIndex; 
                        fread(&invIndex, sizeof(int), 1, file);
                        eManager->invs[i].items[j].chestPtr = (Inventory*)&eManager->invs[invIndex]; // setup chest inventory pointer.
                    }
                }
            }
                
            for(i = 0; i < 5; ++i){
                fread(&eManager->lastSlot[i], sizeof(s16), 1, file); // read amount of entities in level.
                for(j = 0; j < eManager->lastSlot[i]; ++j){
                    fread(&eManager->entities[i][j].type, sizeof(s16), 1, file); // read entity's type ID
                    fread(&eManager->entities[i][j].x, sizeof(s16), 1, file); // read entity's x coordinate
                    fread(&eManager->entities[i][j].y, sizeof(s16), 1, file); // read entity's y coordinate
                    eManager->entities[i][j].slotNum = j;
                    switch(eManager->entities[i][j].type){
                        case ENTITY_SMASHPARTICLE:
                            eManager->entities[i][j].level = i;
                            eManager->entities[i][j].smashParticle.age = 300;
                            eManager->entities[i][j].canPass = true;
                            break;
                        case ENTITY_TEXTPARTICLE:
                            eManager->entities[i][j].level = i;
                            eManager->entities[i][j].canPass = true;
                            eManager->entities[i][j].textParticle.age = 59;
                            eManager->entities[i][j].textParticle.text = NULL;
	                        eManager->entities[i][j].textParticle.xx = eManager->entities[i][j].x;
	                        eManager->entities[i][j].textParticle.yy = eManager->entities[i][j].y;
	                        eManager->entities[i][j].textParticle.zz = 2;
	                        eManager->entities[i][j].textParticle.xa = 0;
	                        eManager->entities[i][j].textParticle.ya = 0;
	                        eManager->entities[i][j].textParticle.za = 0;
                            break;
                        case ENTITY_SPARK:
                            eManager->entities[i][j].level = i;
                            eManager->entities[i][j].spark.age = 300;
                            break;
                        case ENTITY_AIRWIZARD:
                            fread(&eManager->entities[i][j].wizard.health, sizeof(s16), 1, file);
                            eManager->entities[i][j].level = i;
                            eManager->entities[i][j].hurtTime = 0;
                            eManager->entities[i][j].xKnockback = 0;
                            eManager->entities[i][j].yKnockback = 0;
                            eManager->entities[i][j].wizard.dir = 0;
                            eManager->entities[i][j].wizard.attackDelay = 0;
                            eManager->entities[i][j].wizard.attackTime = 0;
                            eManager->entities[i][j].wizard.attackType = 0;
                            eManager->entities[i][j].wizard.xa = 0;
                            eManager->entities[i][j].wizard.ya = 0;
                            eManager->entities[i][j].xr = 4;
                            eManager->entities[i][j].yr = 3;
                            eManager->entities[i][j].canPass = true;
                            break;
                        case ENTITY_SLIME:
                            fread(&eManager->entities[i][j].slime.health, sizeof(s16), 1, file);
                            fread(&eManager->entities[i][j].slime.lvl, sizeof(s8), 1, file);
                            eManager->entities[i][j].level = i;
                            eManager->entities[i][j].hurtTime = 0;
                            eManager->entities[i][j].xKnockback = 0;
                            eManager->entities[i][j].yKnockback = 0;
                            eManager->entities[i][j].slime.xa = 0;
                            eManager->entities[i][j].slime.ya = 0;
                            eManager->entities[i][j].slime.dir = 0;
                            eManager->entities[i][j].xr = 4;
                            eManager->entities[i][j].yr = 3;
                            eManager->entities[i][j].canPass = false;
                            switch(eManager->entities[i][j].slime.lvl){
                                case 2: eManager->entities[i][j].slime.color = 0xFF8282CC; break;
                                case 3: eManager->entities[i][j].slime.color = 0xFFEFEFEF; break;
                                case 4: eManager->entities[i][j].slime.color = 0xFFAA6262; break;
                                default: eManager->entities[i][j].slime.color = 0xFF95DB95; break;
                            }
                            break;
                        case ENTITY_ZOMBIE:
                            fread(&eManager->entities[i][j].zombie.health, sizeof(s16), 1, file);
                            fread(&eManager->entities[i][j].zombie.lvl, sizeof(s8), 1, file);
                            eManager->entities[i][j].level = i;
                            eManager->entities[i][j].hurtTime = 0;
                            eManager->entities[i][j].xKnockback = 0;
                            eManager->entities[i][j].yKnockback = 0;
                            eManager->entities[i][j].zombie.dir = 0;
                            eManager->entities[i][j].xr = 4;
                            eManager->entities[i][j].yr = 3;
                            eManager->entities[i][j].canPass = false;
                            switch(eManager->entities[i][j].zombie.lvl){
                                case 2: eManager->entities[i][j].zombie.color = 0xFF8282CC; break;
                                case 3: eManager->entities[i][j].zombie.color = 0xFFEFEFEF; break;
                                case 4: eManager->entities[i][j].zombie.color = 0xFFAA6262; break;
                                default: eManager->entities[i][j].zombie.color = 0xFF95DB95; break;
                            }
                            break;
                        case ENTITY_ITEM:
                            //eManager->entities[i][j].entityItem.item = newItem(0,0);
                            fread(&eManager->entities[i][j].entityItem.item.id, sizeof(s16), 1, file);
                            fread(&eManager->entities[i][j].entityItem.item.countLevel, sizeof(s16), 1, file);
                            fread(&eManager->entities[i][j].entityItem.age, sizeof(s16), 1, file);
                            eManager->entities[i][j].level = i;
                            eManager->entities[i][j].entityItem.age = 0;
                            eManager->entities[i][j].xr = 3;
                            eManager->entities[i][j].yr = 3;
                            eManager->entities[i][j].canPass = false;
	                        eManager->entities[i][j].entityItem.xx = eManager->entities[i][j].x;
	                        eManager->entities[i][j].entityItem.yy = eManager->entities[i][j].y;
	                        eManager->entities[i][j].entityItem.zz = 2;
	                        eManager->entities[i][j].entityItem.xa = 0;
	                        eManager->entities[i][j].entityItem.ya = 0;
	                        eManager->entities[i][j].entityItem.za = 0;
                            break;
                        case ENTITY_FURNITURE:
                            fread(&eManager->entities[i][j].entityFurniture.itemID, sizeof(s16), 1, file);
                            int invIndex;
                            fread(&invIndex, sizeof(int), 1, file);
                            eManager->entities[i][j].entityFurniture.inv = &eManager->invs[invIndex];
                            eManager->entities[i][j].xr = 3;
                            eManager->entities[i][j].yr = 3;
                            eManager->entities[i][j].canPass = false;
                            if(eManager->entities[i][j].entityFurniture.itemID == ITEM_LANTERN) eManager->entities[i][j].entityFurniture.r = 8;
                            break;
                    }
                }
            }
            fread(map, sizeof(u8), 128*128*5, file);
            fread(mapData, sizeof(u8), 128*128*5, file);
            fclose(file);
            return 0;
        }
    return 1;
}
