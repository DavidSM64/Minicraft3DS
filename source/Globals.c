#include "Globals.h"

char versionText[34] = "BETA BUILD 3";
char fpsstr[34];
u8 currentMenu = 0;

void addItemsToWorld(Item item,int x, int y, int count){
    int i;
    for(i = 0;i<count;++i)addEntityToList(newItemEntity(item,x,y,currentLevel), &eManager);
}

bool intersects(Entity e,int x0, int y0, int x1, int y1) {
	return !(e.x + e.xr < x0 || e.y + e.yr < y0 || e.x - e.xr > x1 || e.y - e.yr > y1);
}

int getEntities(Entity** result,int x0, int y0, int x1, int y1) {
    int i, last = 0;
	for (i = 0; i < eManager.lastSlot[currentLevel]; ++i) {
        Entity* e = &eManager.entities[currentLevel][i];
		if (intersects(*e,x0, y0, x1, y1)){
            result[last] = e;
            ++last;
        }
	}
	return last;
}

void removeSimilarElements(Entity * arr1[], Entity * arr2[]){
    int i,j;
    for(i=0;i<sizeof(arr1)/sizeof(arr1[0]);++i){
        for(j=0;j<sizeof(arr2)/sizeof(arr2[0]);++j){
            if(arr1[i] == arr2[j]) arr2[j] = NULL;
        }
    }
}

bool move2(Entity* e,int xa, int ya) {
	if (xa != 0 && ya != 0) return false;
	int xto0 = ((e->x) - e->xr) >> 4;
	int yto0 = ((e->y) - e->yr) >> 4;
	int xto1 = ((e->x) + e->xr) >> 4;
	int yto1 = ((e->y) + e->yr) >> 4;

	int xt0 = ((e->x + xa) - e->xr) >> 4;
	int yt0 = ((e->y + ya) - e->yr) >> 4;
	int xt1 = ((e->x + xa) + e->xr) >> 4;
	int yt1 = ((e->y + ya) + e->yr) >> 4;
	bool blocked = false;
	int xt,yt;
	for (yt = yt0; yt <= yt1; ++yt)
		for (xt = xt0; xt <= xt1; ++xt) {
			if (xt >= xto0 && xt <= xto1 && yt >= yto0 && yt <= yto1) continue;
			entityTileInteract(e, getTile(xt,yt), xt, yt);
			if (!e->canPass && tileIsSolid(getTile(xt,yt),e)) {
				blocked = true;
				return false;
			}
		}
	if (blocked) return false;

	Entity * wasInside[eManager.lastSlot[currentLevel]];
	Entity * isInside[eManager.lastSlot[currentLevel]];
    getEntities(wasInside, e->x - e->xr, e->y - e->yr, e->x + e->xr, e->y + e->yr);
	int isSize = getEntities(isInside, e->x + xa - e->xr, e->y + ya - e->yr, e->x + xa + e->xr, e->y + ya + e->yr);
	int i;
		
	for (i = 0; i < isSize; ++i) {
		Entity * ent = isInside[i];
		if (ent == e || ent == NULL) continue;
		EntityVsEntity(e, ent);
	}
	if(e->type != ENTITY_PLAYER){
        if(intersects(player, e->x + xa - e->xr, e->y + ya - e->yr, e->x + xa + e->xr, e->y + ya + e->yr)){
            EntityVsEntity(e, &player);
        }
    }
	removeSimilarElements(wasInside, isInside);
		
	for (i = 0; i < isSize; ++i) {
		Entity * ent = isInside[i];
		if (ent == e || ent == NULL) continue;
		if (EntityBlocksEntity(e,ent)) return false;
	}

    if(e->x + xa > 0 && e->x + xa < 2048) e->x += xa;
	if(e->y + ya > 0 && e->y + ya < 2048) e->y += ya;
	return true;
}

bool move(Entity* e, int xa, int ya) {
	if (xa != 0 || ya != 0) {
		bool stopped = true;
		if (xa != 0 && move2(e, xa, 0)) stopped = false;
		if (ya != 0 && move2(e, 0, ya)) stopped = false;
		return !stopped;
	}
	return true;
}

bool moveMob(Entity* e, int xa, int ya){
		if (e->xKnockback < 0) {
			move2(e,-1, 0);
			e->xKnockback++;
		}
		if (e->xKnockback > 0) {
			move2(e,1, 0);
			e->xKnockback--;
		}
		if (e->yKnockback < 0) {
			move2(e,0, -1);
			e->yKnockback++;
		}
		if (e->yKnockback > 0) {
			move2(e,0, 1);
			e->yKnockback--;
		}
		if (e->hurtTime > 0) return true;
		return move(e, xa, ya);
}

void hurtEntity(Entity* e, int damage, int dir, u32 hurtColor){
    if (e->hurtTime > 0) return;
	int xd = player.x - e->x;
	int yd = player.y - e->y;
	if (xd * xd + yd * yd < 80 * 80) playSound(snd_monsterHurt);

    char hurtText[11];
    sprintf(hurtText, "%d", damage);
    addEntityToList(newTextParticleEntity(hurtText,hurtColor,e->x,e->y,currentLevel), &eManager);
    
    // In hindsight I should've made a generic Mob struct, but whatever. ¯\_(-.-)_/¯ 
    switch(e->type){
	   case ENTITY_PLAYER: 
            e->p.health -= damage; 
            if(e->p.health < 1){ 
                playSound(snd_bossdeath);
                e->p.endTimer = 60;
                e->p.isDead = true;
                return;
            }
       break;
	   case ENTITY_ZOMBIE: 
            e->zombie.health -= damage; 
            if(e->zombie.health < 1){ 
                addItemsToWorld(newItem(ITEM_FLESH,1),e->x+8, e->y+8, (rand()%2) + 1);
                player.p.score += 50 * e->zombie.lvl;
                removeEntityFromList(e,e->level,&eManager);
                trySpawn(3, currentLevel);
                return;
            }
       break;
	   case ENTITY_SLIME: 
            e->slime.health -= damage; 
            if(e->slime.health < 1){ 
                addItemsToWorld(newItem(ITEM_SLIME,1),e->x+8, e->y+8, (rand()%2) + 1);
                player.p.score += 25 * e->slime.lvl;
                removeEntityFromList(e,e->level,&eManager);
                trySpawn(3, currentLevel);
                return;
            }
        break;
	   case ENTITY_AIRWIZARD: 
            e->wizard.health -= damage; 
            airWizardHealthDisplay = e->wizard.health;
            if(e->wizard.health < 1){ 
                removeEntityFromList(e,e->level,&eManager);
                playSound(snd_bossdeath);
                player.p.score += 1000;
                player.p.endTimer = 60;
                player.p.hasWon = true;
                player.p.hasWonSaved = true;
                return;
            }
        break;
    }
	
	switch(dir){
        case -1:
            switch(e->type){
                case ENTITY_PLAYER:
                    switch(e->p.dir){
	                   case 0: e->yKnockback = -10; break;
	                   case 1: e->yKnockback = +10; break;
	                   case 2: e->xKnockback = +10; break;
	                   case 3: e->xKnockback = -10; break;
                    } 
                break;
                case ENTITY_ZOMBIE:
                    switch(e->zombie.dir){
	                   case 0: e->yKnockback = -10; break;
	                   case 1: e->yKnockback = +10; break;
	                   case 2: e->xKnockback = +10; break;
	                   case 3: e->xKnockback = -10; break;
                    } 
                break;
                case ENTITY_SLIME:
                    switch(e->slime.dir){
	                   case 0: e->yKnockback = -10; break;
	                   case 1: e->yKnockback = +10; break;
	                   case 2: e->xKnockback = +10; break;
	                   case 3: e->xKnockback = -10; break;
                    } 
                break;
            }
        break;
        case 0: e->yKnockback = +10; break;
	    case 1: e->yKnockback = -10; break;
	    case 2: e->xKnockback = -10; break;
	    case 3: e->xKnockback = +10; break;
    }
	e->hurtTime = 10;
}

bool ItemVsEntity(Item* item, Entity* e, int dir){
    switch(item->id){
        case ITEM_POWGLOVE:
            if(e->type == ENTITY_FURNITURE){
                Item nItem = newItem(e->entityFurniture.itemID,0);
                if(e->entityFurniture.itemID == ITEM_CHEST) nItem.chestPtr = e->entityFurniture.inv;
                pushItemToInventoryFront(nItem, player.p.inv);
                removeEntityFromList(e,currentLevel,&eManager);
                player.p.activeItem = &player.p.inv->items[0];
                player.p.isCarrying = true;
                return true;
            } break;
        case TOOL_AXE:
            switch(e->type){
                case ENTITY_ZOMBIE:
                case ENTITY_SLIME:
                case ENTITY_AIRWIZARD:
                    if(playerUseEnergy(4-item->countLevel)) hurtEntity(e,(item->countLevel + 1) * 2 + (rand()%4),dir,0xFF0000FF);  
                    else hurtEntity(e,1+rand()%3,dir,0xFF0000FF);  
                return true;
            } break;
        case TOOL_SWORD:
            switch(e->type){
                case ENTITY_ZOMBIE:
                case ENTITY_SLIME:
                case ENTITY_AIRWIZARD:
                    if(playerUseEnergy(4-item->countLevel)) hurtEntity(e,(item->countLevel+1)*3+(rand()%(2+item->countLevel*item->countLevel*2)),dir,0xFF0000FF);   
                    else hurtEntity(e,1+rand()%3,dir,0xFF0000FF);     
                return true;
            } break;
        case ITEM_NULL:
            switch(e->type){
                case ENTITY_ZOMBIE:
                case ENTITY_SLIME:
                case ENTITY_AIRWIZARD:
                    hurtEntity(e,1+rand()%3,dir,0xFF0000FF);
                return true;
            } break;
    }
    return false;
}

bool interact(int x0, int y0, int x1, int y1) {
	Entity * es[eManager.lastSlot[currentLevel]];
	int eSize = getEntities(es, x0, y0, x1, y1);
	int i;
	for (i = 0; i < eSize; ++i) {
		Entity * ent = es[i];
		if (ent != &player){
             if (ItemVsEntity(player.p.activeItem, ent, player.p.dir)) return true;
        }
	}
	return false;
}

void EntityVsEntity(Entity* e1, Entity* e2){
    switch(e1->type){
        case ENTITY_PLAYER: playerEntityInteract(e2); break;
        case ENTITY_ZOMBIE:
            if(e2->type == ENTITY_PLAYER){ 
                hurtEntity(e2, 2, e1->zombie.dir, 0xFF00AFFF);
                switch(e1->zombie.dir){
	                case 0: e1->yKnockback = -4; break;
	                case 1: e1->yKnockback = +4; break;
	                case 2: e1->xKnockback = +4; break;
	                case 3: e1->xKnockback = -4; break;
                } 
            }
        break;
        case ENTITY_SLIME:
            if(e2->type == ENTITY_PLAYER){
                hurtEntity(e2, 1, e1->slime.dir, 0xFF00AFFF);
                switch(e1->slime.dir){
	                case 0: e1->yKnockback = -4; break;
	                case 1: e1->yKnockback = +4; break;
	                case 2: e1->xKnockback = +4; break;
	                case 3: e1->xKnockback = -4; break;
                } 
            }
        break;
        case ENTITY_AIRWIZARD:
            if(e2->type == ENTITY_PLAYER) hurtEntity(e2, 3, e1->wizard.dir, 0xFF00AFFF);
        break;
        case ENTITY_SPARK:
            if(e2 != e1->spark.parent) hurtEntity(e2, 1, -1, 0xFF00AFFF);
        break;
    }  
}

bool EntityBlocksEntity(Entity* e1, Entity* e2){
    switch(e1->type){
        case ENTITY_PLAYER:
        case ENTITY_FURNITURE: 
            switch(e2->type){
                case ENTITY_FURNITURE:
                case ENTITY_ZOMBIE:
                case ENTITY_SLIME:
                case ENTITY_AIRWIZARD:
                case ENTITY_PLAYER:
                    return true;
                break;
            }
        break;
    }  
    return false;
}

bool tileIsSolid(int tile, Entity * e){
    switch(tile){
        case TILE_TREE: 
        case TILE_ROCK: 
        case TILE_HARDROCK: 
        case TILE_CACTUS: 
        case TILE_IRONORE: 
        case TILE_GOLDORE: 
        case TILE_GEMORE: 
        case TILE_CLOUDCACTUS: 
        case TILE_LAVA: 
        case 255: 
            return true;
        case TILE_WATER:
            if(e != NULL && !e->canSwim) return true;
    }
    return false;
}

/* For minimap */
u32 getTileColor(int tile){
    switch(tile){
        case TILE_WATER: return 0xFFFF0000;
        case TILE_LAVA: return 0xFF0000FF;
        case TILE_DIRT: return 0xFF6C6D82;
        case TILE_ROCK: return 0xFF7F7F7F;
        case TILE_HARDROCK: return 0xFF7F5F5F;
        case TILE_GRASS: return 0xFF00FF00;
        case TILE_TREE: return 0xFF007F00;
        case TILE_SAND: return 0xFF00FFFF;
        case TILE_CACTUS: return 0xFF009F00;
        case TILE_FLOWER: return 0xFF00FF3F;
        case TILE_IRONORE: return 0xFF9696DC;
        case TILE_GOLDORE: return 0xFF9AE8E5;
        case TILE_GEMORE: return 0xFFDE98DF;
        case TILE_CLOUD: return 0xFFFFFFFF;
        case TILE_CLOUDCACTUS: return 0xFFAFAFAF;
        case TILE_STAIRS_DOWN: return 0xFF9F9F9F;
        case TILE_STAIRS_UP: return 0xFF9F9F9F;
        case TILE_HOLE: return 0xFF383838;
        default: return 0xFF111111;
    }
}

void healPlayer(int amount){
    player.p.health += amount;
    if(player.p.health > 10) player.p.health = 10;
    char healText[11];
    sprintf(healText, "%d", amount);
    addEntityToList(newTextParticleEntity(healText,0x00FF00FF,player.x,player.y,currentLevel), &eManager);
}

s8 itemTileInteract(int tile, Item* item, int x, int y, int px, int py, int dir){
    
     // Furniture items
    if(item->id > 27){
        if(!tileIsSolid(getTile(x,y), NULL)){
            addEntityToList(newFurnitureEntity(item->id,item->chestPtr, (x<<4)+8, (y<<4)+8, currentLevel), &eManager);
            removeItemFromCurrentInv(item);
            player.p.activeItem = &noItem;
            return 2;
        }
        return 0;
    }
    
    // Health items
    switch(item->id){
        case ITEM_APPLE:
            if(player.p.health < 10 && playerUseEnergy(2)){
                healPlayer(1); 
                --item->countLevel;
                if(item->countLevel < 1){ 
                    removeItemFromCurrentInv(item);
                    player.p.activeItem = &noItem;
                }
            }
            return 0;
        case ITEM_FLESH:
            if(player.p.health < 10 && playerUseEnergy(4+(rand()%4))){
                healPlayer(1); 
                --item->countLevel;
                if(item->countLevel < 1){ 
                    removeItemFromCurrentInv(item);
                    player.p.activeItem = &noItem;
                }
            }
            return 0;
        case ITEM_BREAD:
            if(player.p.health < 10 && playerUseEnergy(3)){
                healPlayer(2); 
                --item->countLevel;
                if(item->countLevel < 1){ 
                    removeItemFromCurrentInv(item);
                    player.p.activeItem = &noItem;
                }
            }
            return 0;
    }
    
    switch(tile){
        case TILE_TREE:
            if(item->id == TOOL_AXE && playerUseEnergy(4-item->countLevel)){
                playerHurtTile(tile, x, y, (rand()%10) + (item->countLevel) * 5 + 10, player.p.dir);
                return 1;
            } break;
        case TILE_ROCK:
            if(item->id == TOOL_PICKAXE && playerUseEnergy(4-item->countLevel)){
                playerHurtTile(tile, x, y, (rand()%10) + (item->countLevel) * 5 + 10, player.p.dir);
                return 1;
            } break;
        case TILE_HARDROCK:
            if(item->id == TOOL_PICKAXE && playerUseEnergy(4-item->countLevel)){
                playerHurtTile(tile, x, y, (rand()%10) + (item->countLevel) * 5 + 10, player.p.dir);
                return 1;
            } break;
        case TILE_IRONORE:
            if(item->id == TOOL_PICKAXE && playerUseEnergy(4-item->countLevel)){
                playerHurtTile(tile, x, y, 1, player.p.dir);
                return 1;
            } break;
        case TILE_GOLDORE:
            if(item->id == TOOL_PICKAXE && playerUseEnergy(4-item->countLevel)){
                playerHurtTile(tile, x, y, 1, player.p.dir);
                return 1;
            } break;
        case TILE_GEMORE:
            if(item->id == TOOL_PICKAXE && playerUseEnergy(4-item->countLevel)){
                playerHurtTile(tile, x, y, 1, player.p.dir);
                return 1;
            } break;
        case TILE_CLOUDCACTUS:
            if(item->id == TOOL_PICKAXE && playerUseEnergy(4-item->countLevel)){
                playerHurtTile(tile, x, y, 1, player.p.dir);
                return 1;
            } break;
        case TILE_GRASS:
            if(item->id == TOOL_HOE && playerUseEnergy(4-item->countLevel)){ 
                setTile(TILE_FARM,x,y);
                return 1;
            }
            else if(item->id == ITEM_ACORN){ 
                setTile(TILE_SAPLING_TREE,x,y); --item->countLevel;
                return 1;
            } 
            else if(item->id == ITEM_FLOWER){ 
                setTile(TILE_FLOWER,x,y); --item->countLevel;
				setData(rand()%4,x,y); // determines mirroring.
                return 1;
            } 
            else if(item->id == TOOL_SHOVEL && playerUseEnergy(4-item->countLevel)){ 
                if(rand()%5==0)addEntityToList(newItemEntity(newItem(ITEM_SEEDS,1),(x<<4)+8, (y<<4)+8,currentLevel),&eManager);
                setTile(TILE_DIRT,x,y); 
                return 1;
            } break;
        case TILE_SAND:
            if(item->id == ITEM_CACTUS){ 
                setTile(TILE_SAPLING_CACTUS,x,y); 
                --item->countLevel;
                return 1;
            }
            else if(item->id == TOOL_SHOVEL && playerUseEnergy(4-item->countLevel)){ 
                addEntityToList(newItemEntity(newItem(ITEM_SAND,1), (x<<4)+8, (y<<4)+8, currentLevel), &eManager);
                setTile(TILE_DIRT,x,y); 
                return 1;
            } break;
        case TILE_DIRT:
            if(item->id == TOOL_HOE && playerUseEnergy(4-item->countLevel)){ 
                setTile(TILE_FARM,x,y);
                return 1;
            }   
            else if(item->id == TOOL_SHOVEL && playerUseEnergy(4-item->countLevel)){ 
                addEntityToList(newItemEntity(newItem(ITEM_DIRT,1), (x<<4)+8, (y<<4)+8, currentLevel), &eManager);
                setTile(TILE_HOLE,x,y); 
                return 1;
            } break;
        case TILE_HOLE:
        case TILE_WATER:
        case TILE_LAVA:
            if(item->id == ITEM_DIRT){
                setTile(TILE_DIRT,x,y);    
                --item->countLevel;
                return 1;
            } break;
        case TILE_NULL:
            if(item->id == ITEM_CLOUD){
                setTile(TILE_CLOUD,x,y);    
                --item->countLevel;
                return 1;
            } break;
        case TILE_FARM:
            if(item->id == ITEM_SEEDS){
                setTile(TILE_WHEAT,x,y);
                setData(0,x,y);
                --item->countLevel;
                return 1;
            } break;
        case TILE_WHEAT:
            if(item->id == TOOL_HOE){
                if(getData(x,y) > -1){
                    int age = getData(x,y);
                    int count = (rand() % 2);
                    if(age >= 80) count = (rand()%2) + 1;
                    addItemsToWorld(newItem(ITEM_SEEDS,1),(x<<4)+8,(y<<4)+8,count);
                    count = 0;
			        if(age == 100)count = (rand()%3) + 2;
                    else if(age >= 80)count = (rand()%2) + 1;
                    addItemsToWorld(newItem(ITEM_WHEAT,1),(x<<4)+8,(y<<4)+8,count);
				    setTile(TILE_DIRT,x,y);
                }
            } break;
    }
    return 0;
}

void tickTile(int x, int y){
    int tile = getTile(x,y);
    int data = getData(x,y);
    
    switch(tile){
        case TILE_SAPLING_TREE:
            setData(++data,x,y); if(data>100){setData(0,x,y); setTile(TILE_TREE,x,y);}
            break;
        case TILE_SAPLING_CACTUS:
            setData(++data,x,y); if(data>100){setData(0,x,y); setTile(TILE_CACTUS,x,y);}
            break;
        case TILE_WHEAT:
            if(data<100)setData(++data,x,y);
            break;
        case TILE_WATER:
            if(getTile(x+1,y)==TILE_HOLE) setTile(TILE_WATER,x+1,y);
            if(getTile(x-1,y)==TILE_HOLE) setTile(TILE_WATER,x-1,y);
            if(getTile(x,y+1)==TILE_HOLE) setTile(TILE_WATER,x,y+1);
            if(getTile(x,y-1)==TILE_HOLE) setTile(TILE_WATER,x,y-1);
            break;
        case TILE_LAVA:
            if(getTile(x+1,y)==TILE_HOLE) setTile(TILE_LAVA,x+1,y);
            if(getTile(x-1,y)==TILE_HOLE) setTile(TILE_LAVA,x-1,y);
            if(getTile(x,y+1)==TILE_HOLE) setTile(TILE_LAVA,x,y+1);
            if(getTile(x,y-1)==TILE_HOLE) setTile(TILE_LAVA,x,y-1);
            break;
        case TILE_HOLE: // This makes water flow slightly faster than lava
            if(getTile(x+1,y)==TILE_WATER) setTile(TILE_WATER,x,y);
            if(getTile(x-1,y)==TILE_WATER) setTile(TILE_WATER,x,y);
            if(getTile(x,y+1)==TILE_WATER) setTile(TILE_WATER,x,y);
            if(getTile(x,y-1)==TILE_WATER) setTile(TILE_WATER,x,y);
            break;
        case TILE_GRASS:
            if(getTile(x+1,y)==TILE_DIRT) if((rand()%25) == 0) setTile(TILE_GRASS,x+1,y);
            if(getTile(x-1,y)==TILE_DIRT) if((rand()%25) == 0) setTile(TILE_GRASS,x-1,y);
            if(getTile(x,y+1)==TILE_DIRT) if((rand()%25) == 0) setTile(TILE_GRASS,x,y+1);
            if(getTile(x,y-1)==TILE_DIRT) if((rand()%25) == 0) setTile(TILE_GRASS,x,y-1);
            break;
    }
    
}

void tickEntityItem(Entity* e){
        ++e->entityItem.age;
        if(e->entityItem.age == 630){ 
            removeEntityFromList(e,e->level,&eManager);
            /*
            Programming pro tip: 
                Remember to put a return statement after you remove the entity,
                or else your going to have a very bad time like I did.
            */
            return;
        }
		e->entityItem.xx += e->entityItem.xa;
		e->entityItem.yy += e->entityItem.ya;
		e->entityItem.zz += e->entityItem.za;
		if (e->entityItem.zz < 0) {
			e->entityItem.zz = 0;
			e->entityItem.za *= -0.5;
			e->entityItem.xa *= 0.6;
			e->entityItem.ya *= 0.6;
		}
		e->entityItem.za -= 0.15;
		int ox = e->x;
		int oy = e->y;
		int nx = (int) e->entityItem.xx;
		int ny = (int) e->entityItem.yy;
		int expectedx = nx - e->x;
		int expectedy = ny - e->y;
		move(e, nx - e->x, ny - e->y);
		int gotx = e->x - ox;
		int goty = e->y - oy;
		e->entityItem.xx += gotx - expectedx;
		e->entityItem.yy += goty - expectedy;
}

void tickEntityTextParticle(Entity* e){
        ++e->textParticle.age;
        if(e->textParticle.age == 60){ 
            removeEntityFromList(e,e->level,&eManager);
            return;
        }
		e->textParticle.xx += e->textParticle.xa;
		e->textParticle.yy += e->textParticle.ya;
		e->textParticle.zz += e->textParticle.za;
		if (e->textParticle.zz < 0) {
			e->textParticle.zz = 0;
			e->textParticle.za *= -0.5;
			e->textParticle.xa *= 0.6;
			e->textParticle.ya *= 0.6;
		}
		e->textParticle.za -= 0.15;
		e->x = (int) e->textParticle.xx;
		e->y = (int) e->textParticle.yy;
}

void tickEntity(Entity* e){
    switch(e->type){
        case ENTITY_ITEM: tickEntityItem(e); return;
        case ENTITY_FURNITURE: return;
        case ENTITY_ZOMBIE: 
            if (e->hurtTime > 0) e->hurtTime--;
            if (e->zombie.randWalkTime == 0) {
			    int xd = player.x - e->x;
			    int yd = player.y - e->y;
			    if (xd * xd + yd * yd < 50 * 50) {
				    e->zombie.xa = 0;
				    e->zombie.ya = 0;
				    if (xd < 0) e->zombie.xa = -1; 
				    if (xd > 0) e->zombie.xa = +1;
				    if (yd < 0) e->zombie.ya = -1;
				    if (yd > 0) e->zombie.ya = +1;
			    }
		    }
		    
		    if(e->zombie.xa < 0) e->zombie.dir = 2;
		    else if(e->zombie.xa > 0) e->zombie.dir = 3;
		    if(e->zombie.ya < 0) e->zombie.dir = 1;
		    else if(e->zombie.ya > 0) e->zombie.dir = 0;
		    
		    if(e->zombie.xa != 0 || e->zombie.ya != 0) e->zombie.walkDist++;

		    int speed = tickCount & 1;
		    if (!moveMob(e, e->zombie.xa * speed, e->zombie.ya * speed) || (rand()%100) == 0) {
			    e->zombie.randWalkTime = 60;
			    e->zombie.xa = ((rand()%3) - 1) * (rand()%2);
			    e->zombie.ya = ((rand()%3) - 1) * (rand()%2);
		    }
		    if (e->zombie.randWalkTime > 0) e->zombie.randWalkTime--;
            return;
        case ENTITY_SLIME: 
            if (e->hurtTime > 0) e->hurtTime--;
            
            if (!moveMob(e, e->slime.xa, e->slime.ya) || (rand()%10) == 0) {
                if (e->slime.jumpTime <= -10) {
			        e->slime.xa = ((rand()%3) - 1);
			        e->slime.ya = ((rand()%3) - 1);
			        
			        int xd = player.x - e->x;
			        int yd = player.y - e->y;
			        if (xd * xd + yd * yd < 50 * 50) {
				        if (xd < 0) e->slime.xa = -1; 
				        if (xd > 0) e->slime.xa = +1;
				        if (yd < 0) e->slime.ya = -1;
				        if (yd > 0) e->slime.ya = +1;
			        }
			        
			        if (e->slime.xa != 0 || e->slime.ya != 0) e->slime.jumpTime = 10;
                }
		    }
		    
		    if(e->slime.xa < 0) e->slime.dir = 2;
		    else if(e->slime.xa > 0) e->slime.dir = 3;
		    if(e->slime.ya < 0) e->slime.dir = 1;
		    else if(e->slime.ya > 0) e->slime.dir = 0;
		    
		    if (e->slime.jumpTime > -10) e->slime.jumpTime--;
            if(e->slime.jumpTime == 0){
                e->slime.xa = 0;
                e->slime.ya = 0;
            }
            return;
        case ENTITY_AIRWIZARD: 
            if (e->hurtTime > 0) e->hurtTime--;
            
            if (e->wizard.attackDelay > 0) {
			    e->wizard.dir = (e->wizard.attackDelay - 45) / 4 % 4;
			    e->wizard.dir = (e->wizard.dir * 2 % 4) + (e->wizard.dir / 2);
			    if (e->wizard.attackDelay < 45) e->wizard.dir = 0;
			    e->wizard.attackDelay--;
			    if (e->wizard.attackDelay <= 0) {
				    e->wizard.attackType = 0;
				    if (e->wizard.health < 1000) e->wizard.attackType = 1;
				    if (e->wizard.health < 200) e->wizard.attackType = 2;
				    e->wizard.attackTime = 120;
			    }
			    return;
		    }
            
		    if (e->wizard.attackTime > 0) {
			    e->wizard.attackTime--;
			    float dir = e->wizard.attackTime * 0.25 * (e->wizard.attackTime % 2 * 2 - 1);
			    float speed = (0.7) + (e->wizard.attackType+1) * 0.2;
			    addEntityToList(newSparkEntity(e, cos(dir) * speed, sin(dir) * speed), &eManager);
			    return;
		    }
            
            if (e->wizard.randWalkTime == 0) {
			    int xd = player.x - e->x;
			    int yd = player.y - e->y;
			    int dist = xd * xd + yd * yd;
			    if (dist > 80 * 80) {
				    e->wizard.xa = 0;
				    e->wizard.ya = 0;
				    if (xd < 0) e->wizard.xa = -1; 
				    if (xd > 0) e->wizard.xa = +1;
				    if (yd < 0) e->wizard.ya = -1;
				    if (yd > 0) e->wizard.ya = +1;
			    } else if (dist < 24 * 24) {
				    e->wizard.xa = 0;
				    e->wizard.ya = 0;
				    if (xd < 0) e->wizard.xa = +1; 
				    if (xd > 0) e->wizard.xa = -1;
				    if (yd < 0) e->wizard.ya = +1;
				    if (yd > 0) e->wizard.ya = -1;
                }
		    }
		    
		    int wSpeed = (tickCount % 4) == 0 ? 0 : 1;
		    if (!moveMob(e, e->wizard.xa * wSpeed, e->wizard.ya * wSpeed) || (rand()%100) == 0) {
			    e->wizard.randWalkTime = 30;
			    e->wizard.xa = ((rand()%3) - 1) * (rand()%2);
			    e->wizard.ya = ((rand()%3) - 1) * (rand()%2);
		    }
		    
		    if(e->wizard.xa != 0 || e->wizard.ya != 0) e->wizard.walkDist++;
		    
		    if(e->wizard.xa < 0) e->wizard.dir = 2;
		    else if(e->wizard.xa > 0) e->wizard.dir = 3;
		    if(e->wizard.ya < 0) e->wizard.dir = 1;
		    else if(e->wizard.ya > 0) e->wizard.dir = 0;
		    
		    if (e->wizard.randWalkTime > 0) {
			    e->wizard.randWalkTime--;
			    if (e->wizard.randWalkTime == 0) {
				    int xd = player.x - e->x;
				    int yd = player.y - e->y;
				    if (rand()%4 == 0 && xd * xd + yd * yd < 50 * 50) {
					    if (e->wizard.attackDelay == 0 && e->wizard.attackTime == 0) e->wizard.attackDelay = 120;
				    }
			    }
		    }
		    
            return;
        case ENTITY_SPARK: 
            e->spark.age++;
		    if (e->spark.age >= 260) {
			    removeEntityFromList(e,e->level,&eManager);
			    return;
		    }
		    e->spark.xx += e->spark.xa;
		    e->spark.yy += e->spark.ya;
		    e->x = (int) e->spark.xx;
		    e->y = (int) e->spark.yy;
		    
            if(intersects(player, e->x + e->spark.xa - e->xr, e->y + e->spark.ya - e->yr, e->x + e->spark.xa + e->xr, e->y + e->spark.ya + e->yr)){
                EntityVsEntity(e, &player);
			    removeEntityFromList(e,e->level,&eManager);
            }
            return;
        case ENTITY_TEXTPARTICLE: tickEntityTextParticle(e); return;
        case ENTITY_SMASHPARTICLE: 
            ++e->smashParticle.age;
            if(e->smashParticle.age > 10) removeEntityFromList(e,e->level,&eManager);
        return;
    }
}

void trySpawn(int count, int level) {
    int i;
	for (i = 0; i < count; i++) {
        if(eManager.lastSlot[level] > 900) continue;
		Entity e;
		
		int minLevel = 1;
		int maxLevel = 1;
		if (level > 0) maxLevel = level;
		else if (level == 0) {
			minLevel = maxLevel = 4;
		}
		
        int rx = rand()%128;
        int ry = rand()%128;
        int ex = (rx<<4)+8;
        int ey = (ry<<4)+8;
        
        if(level == currentLevel && (ex > player.x-160 && ey > player.y-125 && ex < player.x+160 && ey < player.y+125)) continue;
        
		if (!tileIsSolid(map[level][rx+ry*128],&e)) {
		    int lvl = (rand()%(maxLevel - minLevel + 1)) + minLevel;
		    if ((rand()%2) == 0) e = newSlimeEntity(lvl, ex, ey, level);
		    else e = newZombieEntity(lvl, ex, ey, level);
			addEntityToList(e, &eManager);
		}
	}
}

int getTile(int x, int y){
    if(x < 0 || y < 0 || x > 128 || y > 128) return -1;
    return map[currentLevel][x+y*128];
}

void setTile(int id, int x, int y){
    if(x < 0 || y < 0 || x > 128 || y > 128) return;
    map[currentLevel][x+y*128] = id;
    sf2d_set_pixel(minimap[currentLevel], x, y, getTileColor(id));
}
int getData(int x, int y){
    if(x < 0 || y < 0 || x > 128 || y > 128) return -1;
    return data[currentLevel][x+y*128];
}

void setData(int id, int x, int y){
    if(x < 0 || y < 0 || x > 128 || y > 128) return;
    data[currentLevel][x+y*128] = id;
}

void spawnPlayer(){
    while(true){
        int rx = rand()%128;
        int ry = rand()%128;
        if(getTile(rx,ry) == TILE_GRASS){
            player.x = (rx << 4) + 8;
            player.y = (ry << 4) + 8;
            break;    
        }
    }
        
}

void initPlayer(){
    player.type = ENTITY_PLAYER;
    spawnPlayer();
    player.xr = 4;
    player.yr = 3;
    player.canSwim = true;
    player.p.ax = 0;
    player.p.ay = 0;
    player.p.health = 10;
    player.p.stamina = 10;
    player.p.score = 0;
    player.p.walkDist = 0;
    player.p.attackTimer = 0;
    player.p.dir = 0;
    player.p.inv = &eManager.invs[0];
    eManager.nextInv++;
    player.p.inv->lastSlot = 0;
    player.p.activeItem = &noItem;
    player.p.isDead = false;
    player.p.hasWon = false;
    
    addItemToInventory(newItem(ITEM_WORKBENCH,0), player.p.inv);
    addItemToInventory(newItem(ITEM_POWGLOVE,0), player.p.inv);   
    
    /*
    addItemToInventory(newItem(TOOL_SHOVEL,4), player.p.inv);
    addItemToInventory(newItem(TOOL_HOE,4), player.p.inv);
    addItemToInventory(newItem(TOOL_SWORD,4), player.p.inv);
    addItemToInventory(newItem(TOOL_PICKAXE,4), player.p.inv);
    addItemToInventory(newItem(TOOL_AXE,4), player.p.inv);
    
    addItemToInventory(newItem(ITEM_ANVIL,0), player.p.inv);
    addItemToInventory(newItem(ITEM_CHEST,0), player.p.inv);
    addItemToInventory(newItem(ITEM_OVEN,0), player.p.inv);
    addItemToInventory(newItem(ITEM_FURNACE,0), player.p.inv);
    addItemToInventory(newItem(ITEM_LANTERN,0), player.p.inv);  
    
    int i;
    for (i = 7;i < 28;++i) addItemToInventory(newItem(i,50), player.p.inv);
    //*/
}

void playerHurtTile(int tile, int xt, int yt, int damage, int dir){
    char hurtText[11];
    switch(tile){
        case TILE_TREE:
            if(rand()%120==0)addEntityToList(newItemEntity(newItem(ITEM_APPLE,1), (xt<<4)+8,(yt<<4)+8, currentLevel), &eManager);
            sprintf(hurtText, "%d", damage);
            addEntityToList(newTextParticleEntity(hurtText,0xFF0000FF,xt<<4,yt<<4,currentLevel), &eManager);
            addEntityToList(newSmashParticleEntity(xt<<4,yt<<4,currentLevel), &eManager);
            setData(getData(xt,yt)+damage,xt,yt);
            if(getData(xt,yt) > 20){
                 setTile(TILE_GRASS,xt,yt);
                 addItemsToWorld(newItem(ITEM_WOOD,1),(xt<<4)+8,(yt<<4)+8,rand()%2+1);
                 addItemsToWorld(newItem(ITEM_ACORN,1),(xt<<4)+8,(yt<<4)+8,rand()%2);
            }
        break;
        case TILE_CACTUS:
            sprintf(hurtText, "%d", damage);
            addEntityToList(newTextParticleEntity(hurtText,0xFF0000FF,xt<<4,yt<<4,currentLevel), &eManager);
            addEntityToList(newSmashParticleEntity(xt<<4,yt<<4,currentLevel), &eManager);
            setData(getData(xt,yt)+damage,xt,yt);
            if(getData(xt,yt) > 10){
                 setTile(TILE_SAND,xt,yt);
                 addItemsToWorld(newItem(ITEM_CACTUS,1),(xt<<4)+8,(yt<<4)+8,rand()%2+1);
            }
        break;
        case TILE_ROCK:
            sprintf(hurtText, "%d", damage);
            addEntityToList(newTextParticleEntity(hurtText,0xFF0000FF,xt<<4,yt<<4,currentLevel), &eManager);
            addEntityToList(newSmashParticleEntity(xt<<4,yt<<4,currentLevel), &eManager);
            setData(getData(xt,yt)+damage,xt,yt);
            if(getData(xt,yt) > 50){
                setTile(TILE_DIRT,xt,yt);
                addItemsToWorld(newItem(ITEM_STONE,1),(xt<<4)+8,(yt<<4)+8,rand()%4+1);
                addItemsToWorld(newItem(ITEM_COAL,1),(xt<<4)+8,(yt<<4)+8,rand()%2);
            }
        break;
        case TILE_HARDROCK:
            if(player.p.activeItem->id != TOOL_PICKAXE || player.p.activeItem->countLevel < 4) damage = 0;
            sprintf(hurtText, "%d", damage);
            addEntityToList(newTextParticleEntity(hurtText,0xFF0000FF,xt<<4,yt<<4,currentLevel), &eManager);
            addEntityToList(newSmashParticleEntity(xt<<4,yt<<4,currentLevel), &eManager);
            setData(getData(xt,yt)+damage,xt,yt);
            if(getData(xt,yt) > 200){
                setTile(TILE_DIRT,xt,yt);
                addItemsToWorld(newItem(ITEM_STONE,1),(xt<<4)+8,(yt<<4)+8,rand()%4+1);
                addItemsToWorld(newItem(ITEM_COAL,1),(xt<<4)+8,(yt<<4)+8,rand()%2);
            }
        break;
        case TILE_IRONORE:
            if(player.p.activeItem->id != TOOL_PICKAXE) damage = 0;
            sprintf(hurtText, "%d", damage);
            addEntityToList(newTextParticleEntity(hurtText,0xFF0000FF,xt<<4,yt<<4,currentLevel), &eManager);
            addEntityToList(newSmashParticleEntity(xt<<4,yt<<4,currentLevel), &eManager);
            setData(getData(xt,yt)+damage,xt,yt);
            if(getData(xt,yt) > 0){
                 int count = rand() & 1;
			     if (getData(xt,yt) >= (rand()%10) + 3) {
				    setTile(TILE_DIRT,xt,yt);
				    count += 2;
			     }
                 addItemsToWorld(newItem(ITEM_IRONORE,1),(xt<<4)+8,(yt<<4)+8,count);
            } break;
        case TILE_GOLDORE:
            if(player.p.activeItem->id != TOOL_PICKAXE) damage = 0;
            sprintf(hurtText, "%d", damage);
            addEntityToList(newTextParticleEntity(hurtText,0xFF0000FF,xt<<4,yt<<4,currentLevel), &eManager);
            addEntityToList(newSmashParticleEntity(xt<<4,yt<<4,currentLevel), &eManager);
            setData(getData(xt,yt)+damage,xt,yt);
            if(getData(xt,yt) > 0){
                 int count = rand() & 1;
			     if (getData(xt,yt) >= (rand()%10) + 3) {
				    setTile(TILE_DIRT,xt,yt);
				    count += 2;
			     }
                 addItemsToWorld(newItem(ITEM_GOLDORE,1),(xt<<4)+8,(yt<<4)+8,count);
            } break;
        case TILE_GEMORE:
            if(player.p.activeItem->id != TOOL_PICKAXE) damage = 0;
            sprintf(hurtText, "%d", damage);
            addEntityToList(newTextParticleEntity(hurtText,0xFF0000FF,xt<<4,yt<<4,currentLevel), &eManager);
            addEntityToList(newSmashParticleEntity(xt<<4,yt<<4,currentLevel), &eManager);
            setData(getData(xt,yt)+damage,xt,yt);
            if(getData(xt,yt) > 0){
                 int count = rand() & 1;
			     if (getData(xt,yt) >= (rand()%10) + 3) {
				    setTile(TILE_DIRT,xt,yt);
				    count += 2;
			     }
                 addItemsToWorld(newItem(ITEM_GEM,1),(xt<<4)+8,(yt<<4)+8,count);
            } break;
        case TILE_CLOUDCACTUS:
            if(player.p.activeItem->id != TOOL_PICKAXE) damage = 0;
            sprintf(hurtText, "%d", damage);
            addEntityToList(newTextParticleEntity(hurtText,0xFF0000FF,xt<<4,yt<<4,currentLevel), &eManager);
            addEntityToList(newSmashParticleEntity(xt<<4,yt<<4,currentLevel), &eManager);
            setData(getData(xt,yt)+damage,xt,yt);
            if(getData(xt,yt) > 0){
                 int count = rand() % 3;
			     if (getData(xt,yt) >= (rand()%10) + 3) {
				    setTile(TILE_CLOUD,xt,yt);
				    count += 3;
			     }
                 addItemsToWorld(newItem(ITEM_CLOUD,1),(xt<<4)+8,(yt<<4)+8,count);
            } break;
        case TILE_FARM:
			setTile(TILE_DIRT,xt,yt);
            break;
        case TILE_SAPLING_TREE:
			setTile(TILE_GRASS,xt,yt);
            break;
        case TILE_SAPLING_CACTUS:
			setTile(TILE_SAND,xt,yt);
            break;
        case TILE_WHEAT:
            if(getData(xt,yt) > -1){
                int age = getData(xt,yt);
                int count = (rand() % 2);
                if(age >= 80) count = (rand()%2) + 1;
                addItemsToWorld(newItem(ITEM_SEEDS,1),(xt<<4)+8,(yt<<4)+8,count);
                count = 0;
			    if(age == 100)count = (rand()%3) + 2;
                else if(age >= 80)count = (rand()%2) + 1;
                addItemsToWorld(newItem(ITEM_WHEAT,1),(xt<<4)+8,(yt<<4)+8,count);
				setTile(TILE_DIRT,xt,yt);
            } break;
        case TILE_FLOWER:
			setTile(TILE_GRASS,xt,yt);
			addEntityToList(newItemEntity(newItem(ITEM_FLOWER,1), (xt<<4)+8,(yt<<4)+8, currentLevel), &eManager);
            break;
    }
    
}

bool playerUseEnergy(int amount){
    if(amount > player.p.stamina) return false;
    player.p.stamina -= amount;
    return true;
}

void playerAttack(){
    bool done = false;
	player.p.attackTimer = 5;
	int yo = -2;
    int range = 12;
    
    switch(player.p.dir){
        case 0: if(interact(player.x - 8, player.y + 4 + yo, player.x + 8, player.y + range + yo)) return; break;
        case 1: if(interact(player.x - 8, player.y - range + yo, player.x + 8, player.y - 4 + yo)) return; break;
        case 2: if(interact(player.x - range, player.y - 8 + yo, player.x - 4, player.y + 8 + yo)) return; break;
        case 3: if(interact(player.x + 4, player.y - 8 + yo, player.x + range, player.y + 8 + yo)) return; break;
    }

	int xt = player.x >> 4;
	int yt = (player.y + yo) >> 4;
	int r = 12;
	if (player.p.dir == 0) yt = (player.y + r + yo) >> 4;
	if (player.p.dir == 1) yt = (player.y - r + yo) >> 4;
	if (player.p.dir == 2) xt = (player.x - r) >> 4;
	if (player.p.dir == 3) xt = (player.x + r) >> 4;

	if (xt >= 0 && yt >= 0 && xt < 128 && yt < 128) {
        s8 itract = itemTileInteract(getTile(xt,yt),player.p.activeItem,xt,yt,player.x,player.y,player.p.dir);
        if(itract > 0){
		    if(itract==2)player.p.isCarrying = false;
		    done = true;
        }
		
		if (isItemEmpty(player.p.activeItem)) {
			removeItemFromInventory(player.p.activeItem->slotNum, player.p.inv);
			player.p.activeItem = &noItem;
		}
	}
	
	if(done) return;
    
    if (player.p.activeItem == &noItem || player.p.activeItem->id == TOOL_SWORD || player.p.activeItem->id == TOOL_AXE) {
			xt = player.x >> 4;
			yt = (player.y + yo) >> 4;
			r = 12;
			if (player.p.dir == 0) yt = (player.y + r + yo) >> 4;
			if (player.p.dir == 1) yt = (player.y - r + yo) >> 4;
			if (player.p.dir == 2) xt = (player.x - r) >> 4;
			if (player.p.dir == 3) xt = (player.x + r) >> 4;

			if (xt >= 0 && yt >= 0 && xt < 128 && 128) {
                playerHurtTile(getTile(xt,yt), xt, yt, (rand()%3) + 1, player.p.dir);
			}
		}
}


void switchLevel(s8 change){
    currentLevel+=change;
    if(currentLevel > 4) currentLevel = 0; else if(currentLevel < 0) currentLevel = 4;
    if(currentLevel == 1) sf2d_set_clear_color(RGBA8(0x82, 0x6D, 0x6C, 0xFF));
    else if(currentLevel > 1) sf2d_set_clear_color(RGBA8(0x66, 0x66, 0x66, 0xFF));
    else sf2d_set_clear_color(RGBA8(0x00, 0x1D, 0xC1, 0xFF));
}

bool playerIntersectsEntity(Entity* e){
    return (player.x < e->x + e->xr && player.x + 4 > e->x && player.y < e->y + e->yr && player.y + 4 > e->y);
}

void playerEntityInteract(Entity* e){
    switch(e->type){
        case ENTITY_ITEM:
            if(e->entityItem.age > 30){//30
                addItemToInventory(e->entityItem.item, player.p.inv);
                removeEntityFromList(e,currentLevel,&eManager);
                playSound(snd_pickup);
                player.p.score++;
            }
            break;
        case ENTITY_FURNITURE:
            switch(player.p.dir){
                case 0: if(player.y < e->y) move(e,0,2); break;
                case 1: if(player.y > e->y) move(e,0,-2); break;
                case 2: if(player.x > e->x) move(e,-2,0); break;
                case 3: if(player.x < e->x) move(e,2,0); break;
            }
            break;
            
    }
    
}

void entityTileInteract(Entity*e, int tile, int x, int y){
    switch(tile){
        case TILE_STAIRS_DOWN: 
            if(e->type == ENTITY_PLAYER){
                switchLevel(1); 
                player.x = (x << 4) + 8;
                player.y = (y << 4) + 8;
            }
        return;
        case TILE_STAIRS_UP: 
            if(e->type == ENTITY_PLAYER){
                switchLevel(-1); 
                player.x = (x << 4) + 8;
                player.y = (y << 4) + 8;
            }
        return;
        case TILE_CACTUS: if(e->type == ENTITY_PLAYER)hurtEntity(e,1,-1,0xFF00AFFF); return;
        case TILE_LAVA: if(e->type == ENTITY_PLAYER)hurtEntity(e,1,-1,0xFF00AFFF); return;
        case TILE_WHEAT: 
            if(e->type == ENTITY_PLAYER || e->type == ENTITY_ZOMBIE){
                if(getData(x,y) > -1 && rand()%20 == 0){
                    int age = getData(x,y);
                    int count = (rand() % 2);
                    if(age >= 80) count = (rand()%2) + 1;
                    addItemsToWorld(newItem(ITEM_SEEDS,1),(x<<4)+8,(y<<4)+8,count);
                    count = 0;
			        if(age == 100)count = (rand()%3) + 2;
                    else if(age >= 80)count = (rand()%2) + 1;
                    addItemsToWorld(newItem(ITEM_WHEAT,1),(x<<4)+8,(y<<4)+8,count);
				    setTile(TILE_DIRT,x,y);
                }
            }
         return;
        case TILE_FARM:
            if(e->type == ENTITY_PLAYER || e->type == ENTITY_ZOMBIE){
                if(rand()%20 == 0)setTile(TILE_DIRT,x,y);
            }
         return; 
    }
}

bool intersectsEntity(int x, int y, int r, Entity* e){
    return (x < e->x + e->xr && x + r > e->x && y < e->y + e->yr && y + r > e->y);
}

bool isPlayerInsideEntity(int x, int y){
    int i;
    for(i = 0; i < eManager.lastSlot[currentLevel];++i){
        Entity e = eManager.entities[currentLevel][i];
        if(!e.canPass && intersectsEntity(x-16,y-16,16,&e)){ 
            playerEntityInteract(&eManager.entities[currentLevel][i]);
            return true;
        }
    }
    return false;
}

bool useEntity(Entity* e) {
    if(e->type == ENTITY_FURNITURE){
        switch(e->entityFurniture.itemID){
            case ITEM_WORKBENCH:
                currentRecipes = &workbenchRecipes;
                currentMenu = MENU_CRAFTING; 
                checkCanCraftRecipes(currentRecipes, player.p.inv);
                sortRecipes(currentRecipes);
                return true;
            case ITEM_FURNACE:
                currentRecipes = &furnaceRecipes;
                currentMenu = MENU_CRAFTING; 
                checkCanCraftRecipes(currentRecipes, player.p.inv);
                sortRecipes(currentRecipes);
                return true;
            case ITEM_OVEN:
                currentRecipes = &ovenRecipes;
                currentMenu = MENU_CRAFTING; 
                checkCanCraftRecipes(currentRecipes, player.p.inv);
                sortRecipes(currentRecipes);
                return true;
            case ITEM_ANVIL:
                currentRecipes = &anvilRecipes;
                currentMenu = MENU_CRAFTING; 
                checkCanCraftRecipes(currentRecipes, player.p.inv);
                sortRecipes(currentRecipes);
                return true;
            case ITEM_CHEST:
                curChestEntity = e;
                curInvSel = 0;
                curChestEntity->entityFurniture.r = 0;
                curChestEntity->entityFurniture.oSel = 0;
                currentMenu = MENU_CONTAINER; 
                return true;
        }
    }
    return false;
}

bool use(int x0, int y0, int x1, int y1) {
	Entity * entities[eManager.lastSlot[currentLevel]];
	int i;
    int ae = getEntities(entities, x0, y0, x1, y1);
	for(i = 0; i < ae; ++i){ 
        if(useEntity(entities[i])) return true;
    }
	return false;
}
	
bool playerUse() {
	int yo = -2;
	if (player.p.dir == 0 && use(player.x - 8, player.y + 4 + yo, player.x + 8, player.y + 12 + yo)) return true;
	if (player.p.dir == 1 && use(player.x - 8, player.y - 12 + yo, player.x + 8, player.y - 4 + yo)) return true;
	if (player.p.dir == 3 && use(player.x + 4, player.y - 8 + yo, player.x + 12, player.y + 8 + yo)) return true;
	if (player.p.dir == 2 && use(player.x - 12, player.y - 8 + yo, player.x - 4, player.y + 8 + yo)) return true;
	return false;
}

void tickPlayer(){
    if (player.hurtTime > 0) player.hurtTime--;
    bool swimming = isSwimming();
    if (player.p.stamina <= 0 && player.p.staminaRechargeDelay == 0 && player.p.staminaRecharge == 0) {
			player.p.staminaRechargeDelay = 40;
	}

	if (player.p.staminaRechargeDelay > 0) {
		--player.p.staminaRechargeDelay;
	}

	if (player.p.staminaRechargeDelay == 0) {
		++player.p.staminaRecharge;
		if (swimming) player.p.staminaRecharge = 0;
		
		while (player.p.staminaRecharge > 10) {
			player.p.staminaRecharge -= 10;
			if (player.p.stamina < 10) ++player.p.stamina;
		}
	}
    
	player.p.ax = 0;
	player.p.ay = 0;
	
	if (k_left.down){
        player.p.ax -= 1;
        player.p.dir = 2;
        ++player.p.walkDist;
    }
	if (k_right.down){
        player.p.ax += 1;
        player.p.dir = 3;
        ++player.p.walkDist;
    }
	if (k_up.down){
        player.p.ay -= 1;
        player.p.dir = 1;
        ++player.p.walkDist;
    }
	if (k_down.down){
        player.p.ay += 1;
        player.p.dir = 0;
        ++player.p.walkDist;
    }
    if (player.p.staminaRechargeDelay % 2 == 0) moveMob(&player, player.p.ax, player.p.ay);
	
	
	if (swimming && player.p.swimTimer % 60 == 0) {
		if (player.p.stamina > 0) {
			--player.p.stamina;
		} else {
		    hurtEntity(&player,1,-1,0xFF00AFFF);
		}
	}
	
    if (k_pause.clicked){
        currentSelection = 0;
        currentMenu = MENU_PAUSED; 
    }
    
    if(k_attack.clicked){
        if (player.p.stamina != 0) {
			player.p.stamina--;
			player.p.staminaRecharge = 0;
            playerAttack();
            //addEntityToList(newSlimeEntity(1,200,600,1), &eManager);
		}
    }
    
    if (k_menu.clicked){ 
        curInvSel = 0;
        if(!playerUse()) currentMenu = MENU_INVENTORY; 
    }
    
    if(isSwimming()) ++player.p.swimTimer;
    if(player.p.attackTimer > 0) --player.p.attackTimer;
}

bool isSwimming(){
    return getTile(player.x>>4,player.y>>4)==TILE_WATER;
}

