#include "Item.h"

char currentName[16];

bool isItemEmpty(Item* item){
    if(item->id < 6 || item->onlyOne == true) return false;
    if(item->countLevel < 1) return true;
    return false;
}

void pushItemToInventoryFront(Item item, Inventory * inv){
    if(inv->lastSlot < 300) ++inv->lastSlot;
    int i;
    for(i = inv->lastSlot;i > 0;--i){ 
        inv->items[i] = inv->items[i-1]; // Move the items up the list.
        inv->items[i].slotNum = i;
    }
    item.invPtr = (int*)inv;
    inv->items[0] = item;
    inv->items[0].slotNum = 0;
    
}

void addItemToInventory(Item item, Inventory * inv){
    if(!item.onlyOne){
        int i;
        for(i = 0;i < inv->lastSlot;++i){ //Search inventory if item already exists.
            if(inv->items[i].id == item.id){
                inv->items[i].countLevel+=item.countLevel;
                return;
            }
        }
    }
    
    item.slotNum = inv->lastSlot;
    item.invPtr = (int*)inv;
    inv->items[inv->lastSlot] = item;
    ++inv->lastSlot;
}

void removeItemFromCurrentInv(Item* item){
    removeItemFromInventory(item->slotNum, (Inventory*)item->invPtr);
}

Item nullItem;
void removeItemFromInventory(int slot, Inventory * inv){
    int i;
    for(i = slot;i < inv->lastSlot - 1;++i){ 
        inv->items[i] = inv->items[i + 1]; // Move the items down.
        inv->items[i].slotNum--;
    }
    --inv->lastSlot;
    inv->items[inv->lastSlot] = nullItem; // Make the last slot null.
}

Item newItem(int id, int cLevel){
    Item item;
    item.id = id;
    if(id != ITEM_NULL){
        if(cLevel > 999) cLevel = 999;
        item.countLevel = cLevel;
        if(id < 7 || id > 27) item.onlyOne = true; // Tools + Furniture.
        else item.onlyOne = false; 
    }
    item.chestPtr = NULL;
    return item;
}

Item* getItemFromInventory(int itemID, Inventory * inv){
    int i;
    for(i = 0;i < inv->lastSlot;++i){ 
        if(inv->items[i].id == itemID){
            return &inv->items[i];
        }
    }
    return (Item*)NULL;
}

int countItemInv(int itemID, int level, Inventory* inv){
    int i, count = 0;
    for(i = 0;i < inv->lastSlot;++i){ 
        if(inv->items[i].id == itemID){ 
            if(inv->items[i].onlyOne){
                if(level == inv->items[i].countLevel) count++;
            } else count += inv->items[i].countLevel;
        }
    }
    return count;
}
    
char* getItemName(int itemID, int countLevel){
    switch(itemID){
        case TOOL_SHOVEL:
            switch(countLevel){
                case 1: return "Rock Shovel";
                case 2: return "Iron Shovel";
                case 3: return "Gold Shovel";
                case 4: return "Gem Shovel";
                default: return "Wood Shovel";
            }
        case TOOL_HOE:
            switch(countLevel){
                case 1: return "Rock Hoe";
                case 2: return "Iron Hoe";
                case 3: return "Gold Hoe";
                case 4: return "Gem Hoe";
                default: return "Wood Hoe";
            }
        case TOOL_SWORD:
            switch(countLevel){
                case 1: return "Rock Sword";
                case 2: return "Iron Sword";
                case 3: return "Gold Sword";
                case 4: return "Gem Sword";
                default: return "Wood Sword";
            }
        case TOOL_PICKAXE:
            switch(countLevel){
                case 1: return "Rock Pickaxe";
                case 2: return "Iron Pickaxe";
                case 3: return "Gold Pickaxe";
                case 4: return "Gem Pickaxe";
                default: return "Wood Pickaxe";
            }
        case TOOL_AXE:
            switch(countLevel){
                case 1: return "Rock Axe";
                case 2: return "Iron Axe";
                case 3: return "Gold Axe";
                case 4: return "Gem Axe";
                default: return "Wood Axe";
            }
        case ITEM_ANVIL: return "Anvil";
        case ITEM_CHEST: return "Chest";
        case ITEM_OVEN: return "Oven";
        case ITEM_FURNACE: return "Furnace";
        case ITEM_WORKBENCH: return "Workbench";
        case ITEM_LANTERN: return "Lantern";
        case ITEM_POWGLOVE: return "Power Glove";
        case ITEM_FLOWER: sprintf(currentName,"%d Flower", countLevel); return currentName;
        case ITEM_WOOD: sprintf(currentName,"%d Wood", countLevel); return currentName;
        case ITEM_STONE: sprintf(currentName,"%d Stone", countLevel); return currentName;
        case ITEM_SAND: sprintf(currentName,"%d Sand", countLevel); return currentName;
        case ITEM_DIRT: sprintf(currentName,"%d Dirt", countLevel); return currentName;
        case ITEM_CLOUD: sprintf(currentName,"%d Cloud", countLevel); return currentName;
        case ITEM_ACORN: sprintf(currentName,"%d Acorn", countLevel); return currentName;
        case ITEM_CACTUS: sprintf(currentName,"%d Cactus", countLevel); return currentName;
        case ITEM_SEEDS: sprintf(currentName,"%d Seeds", countLevel); return currentName;
        case ITEM_WHEAT: sprintf(currentName,"%d Wheat", countLevel); return currentName;
        case ITEM_FLESH: sprintf(currentName,"%d Flesh", countLevel); return currentName;
        case ITEM_BREAD: sprintf(currentName,"%d Bread", countLevel); return currentName;
        case ITEM_APPLE: sprintf(currentName,"%d Apple", countLevel); return currentName;
        case ITEM_COAL: sprintf(currentName,"%d Coal", countLevel); return currentName;
        case ITEM_IRONORE: sprintf(currentName,"%d Iron ore", countLevel); return currentName;
        case ITEM_GOLDORE: sprintf(currentName,"%d Gold ore", countLevel); return currentName;
        case ITEM_IRONINGOT: sprintf(currentName,"%d Iron ingot", countLevel); return currentName;
        case ITEM_GOLDINGOT: sprintf(currentName,"%d Gold ingot", countLevel); return currentName;
        case ITEM_GLASS: sprintf(currentName,"%d Glass", countLevel); return currentName;
        case ITEM_GEM: sprintf(currentName,"%d Gem", countLevel); return currentName;
        case ITEM_SLIME: sprintf(currentName,"%d Slime", countLevel); return currentName;
        default: return ""; // null
    }
}
    
char* getBasicItemName(int itemID, int countLevel){
    switch(itemID){
        case TOOL_SHOVEL:
            switch(countLevel){
                case 1: return "Rock Shovel";
                case 2: return "Iron Shovel";
                case 3: return "Gold Shovel";
                case 4: return "Gem Shovel";
                default: return "Wood Shovel";
            }
        case TOOL_HOE:
            switch(countLevel){
                case 1: return "Rock Hoe";
                case 2: return "Iron Hoe";
                case 3: return "Gold Hoe";
                case 4: return "Gem Hoe";
                default: return "Wood Hoe";
            }
        case TOOL_SWORD:
            switch(countLevel){
                case 1: return "Rock Sword";
                case 2: return "Iron Sword";
                case 3: return "Gold Sword";
                case 4: return "Gem Sword";
                default: return "Wood Sword";
            }
        case TOOL_PICKAXE:
            switch(countLevel){
                case 1: return "Rock Pickaxe";
                case 2: return "Iron Pickaxe";
                case 3: return "Gold Pickaxe";
                case 4: return "Gem Pickaxe";
                default: return "Wood Pickaxe";
            }
        case TOOL_AXE:
            switch(countLevel){
                case 1: return "Rock Axe";
                case 2: return "Iron Axe";
                case 3: return "Gold Axe";
                case 4: return "Gem Axe";
                default: return "Wood Axe";
            }
        case ITEM_ANVIL: return "Anvil";
        case ITEM_CHEST: return "Chest";
        case ITEM_OVEN: return "Oven";
        case ITEM_FURNACE: return "Furnace";
        case ITEM_WORKBENCH: return "Workbench";
        case ITEM_LANTERN: return "Lantern";
        case ITEM_POWGLOVE: return "Power Glove";
        case ITEM_FLOWER: return "Flower";
        case ITEM_WOOD: return "Wood";
        case ITEM_STONE: return "Stone";
        case ITEM_SAND: return "Sand";
        case ITEM_DIRT: return "Dirt";
        case ITEM_CLOUD: return "Cloud";
        case ITEM_ACORN: return "Acorn";
        case ITEM_CACTUS: return "Cactus";
        case ITEM_SEEDS: return "Seeds";
        case ITEM_WHEAT: return "Wheat";
        case ITEM_FLESH: return "Flesh";
        case ITEM_BREAD: return "Bread";
        case ITEM_APPLE: return "Apple";
        case ITEM_COAL: return "Coal";
        case ITEM_IRONORE: return "Iron ore";
        case ITEM_GOLDORE: return "Gold ore";
        case ITEM_IRONINGOT: return "Iron ingot";
        case ITEM_GOLDINGOT: return "Gold ingot";
        case ITEM_GLASS: return "Glass";
        case ITEM_GEM: return "Gem";
        case ITEM_SLIME: return "Slime";
        default: return ""; // null
    }
    
}
