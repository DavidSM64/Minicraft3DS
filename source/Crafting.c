#include "Crafting.h"

void checkCanCraftRecipes(RecipeManager * rm, Inventory * inv){
    int i, j;
    for(i = 0; i < rm->size; i++){
        rm->recipes[i].canCraft = true;
        for(j = 0; j < rm->recipes[i].numOfCosts; j++){
            if(countItemInv(rm->recipes[i].costs[j].costItem,0,inv) < rm->recipes[i].costs[j].costAmount){
                rm->recipes[i].canCraft = false;
            }
        }
    }
}

int compareCanCraft(const void * ra, const void * rb) {
    Recipe* r1 = (Recipe*)ra;
    Recipe* r2 = (Recipe*)rb;
	if (r1->canCraft && !r2->canCraft) return -1;
    if (!r1->canCraft && r2->canCraft) return 1;
	return r1->order - r2->order; // Needed for stable sorting.
}

void sortRecipes(RecipeManager * rm){
    qsort(rm->recipes,rm->size,sizeof(Recipe),compareCanCraft);
}

void deductCost(Cost c, Inventory * inv){
    Item* item = getItemFromInventory(c.costItem, inv);
    if(!item->onlyOne){
        item->countLevel -= c.costAmount;
        if(item->countLevel < 1) removeItemFromInventory(item->slotNum, inv);
    } else {
        removeItemFromInventory(item->slotNum, inv);
    }
}

bool craftItem(RecipeManager * rm, Recipe* r, Inventory* inv){
    if(r->canCraft){
        int i;
        for(i=0;i<r->numOfCosts;++i) deductCost(r->costs[i], inv);
        Item item = newItem(r->itemResult,r->itemAmountLevel);
        
        if(!item.onlyOne && countItemInv(item.id,item.countLevel,inv) > 0){
            getItemFromInventory(item.id, inv)->countLevel += r->itemAmountLevel;
        } else{ 
            pushItemToInventoryFront(item, inv);
        }
        checkCanCraftRecipes(rm, inv);
        sortRecipes(rm);
        return true;
    }
    return false;
}

Cost newCost(int i, int c){
    Cost nc;
    nc.costItem = i;
    nc.costAmount = c;
    return nc;
}

u8 curPlace = 0;
Recipe defineRecipe(int item, int amountOrLevel, int numArgs, ...){
    Recipe r;
    r.itemResult = item;
    r.itemAmountLevel = amountOrLevel;
    r.numOfCosts = numArgs;
    int i;
    va_list al;
    numArgs <<= 1; // Did this to get rid of a warning.
    va_start(al,numArgs);
    for(i=0;i<r.numOfCosts;++i) r.costs[i] = newCost(va_arg(al,int), va_arg(al,int));
    va_end(al);
    r.order = curPlace;
    curPlace++;
    return r;
}

void initRecipes(){
    
    curPlace = 0;
    workbenchRecipes.size = 16;
    workbenchRecipes.recipes = (Recipe*)malloc(sizeof(Recipe) * (workbenchRecipes.size));
    workbenchRecipes.recipes[0] = defineRecipe(ITEM_WORKBENCH,1,1,ITEM_WOOD,20);
    workbenchRecipes.recipes[1] = defineRecipe(ITEM_FURNACE,1,1,ITEM_STONE,20);
    workbenchRecipes.recipes[2] = defineRecipe(ITEM_OVEN,1,1,ITEM_STONE,20);
    workbenchRecipes.recipes[3] = defineRecipe(ITEM_CHEST,1,1,ITEM_WOOD,20);
    workbenchRecipes.recipes[4] = defineRecipe(ITEM_ANVIL,1,1,ITEM_IRONINGOT,5);
    workbenchRecipes.recipes[5] = defineRecipe(ITEM_LANTERN,1,3,ITEM_WOOD,5,ITEM_SLIME,10,ITEM_GLASS,4);
    workbenchRecipes.recipes[6] = defineRecipe(TOOL_SWORD,0,1,ITEM_WOOD,5);
    workbenchRecipes.recipes[7] = defineRecipe(TOOL_AXE,0,1,ITEM_WOOD,5);
    workbenchRecipes.recipes[8] = defineRecipe(TOOL_HOE,0,1,ITEM_WOOD,5);
    workbenchRecipes.recipes[9] = defineRecipe(TOOL_PICKAXE,0,1,ITEM_WOOD,5);
    workbenchRecipes.recipes[10] = defineRecipe(TOOL_SHOVEL,0,1,ITEM_WOOD,5);
    workbenchRecipes.recipes[11] = defineRecipe(TOOL_SWORD,1,2,ITEM_WOOD,5,ITEM_STONE,5);
    workbenchRecipes.recipes[12] = defineRecipe(TOOL_AXE,1,2,ITEM_WOOD,5,ITEM_STONE,5);
    workbenchRecipes.recipes[13] = defineRecipe(TOOL_HOE,1,2,ITEM_WOOD,5,ITEM_STONE,5);
    workbenchRecipes.recipes[14] = defineRecipe(TOOL_PICKAXE,1,2,ITEM_WOOD,5,ITEM_STONE,5);
    workbenchRecipes.recipes[15] = defineRecipe(TOOL_SHOVEL,1,2,ITEM_WOOD,5,ITEM_STONE,5);

    anvilRecipes.size = 15;
    anvilRecipes.recipes = (Recipe*)malloc(sizeof(Recipe) * (anvilRecipes.size));
    anvilRecipes.recipes[0] = defineRecipe(TOOL_SWORD,2,2,ITEM_WOOD,5,ITEM_IRONINGOT,5);
    anvilRecipes.recipes[1] = defineRecipe(TOOL_AXE,2,2,ITEM_WOOD,5,ITEM_IRONINGOT,5);
    anvilRecipes.recipes[2] = defineRecipe(TOOL_HOE,2,2,ITEM_WOOD,5,ITEM_IRONINGOT,5);
    anvilRecipes.recipes[3] = defineRecipe(TOOL_PICKAXE,2,2,ITEM_WOOD,5,ITEM_IRONINGOT,5);
    anvilRecipes.recipes[4] = defineRecipe(TOOL_SHOVEL,2,2,ITEM_WOOD,5,ITEM_IRONINGOT,5);
    anvilRecipes.recipes[5] = defineRecipe(TOOL_SWORD,3,2,ITEM_WOOD,5,ITEM_GOLDINGOT,5);
    anvilRecipes.recipes[6] = defineRecipe(TOOL_AXE,3,2,ITEM_WOOD,5,ITEM_GOLDINGOT,5);
    anvilRecipes.recipes[7] = defineRecipe(TOOL_HOE,3,2,ITEM_WOOD,5,ITEM_GOLDINGOT,5);
    anvilRecipes.recipes[8] = defineRecipe(TOOL_PICKAXE,3,2,ITEM_WOOD,5,ITEM_GOLDINGOT,5);
    anvilRecipes.recipes[9] = defineRecipe(TOOL_SHOVEL,3,2,ITEM_WOOD,5,ITEM_GOLDINGOT,5);
    anvilRecipes.recipes[10] = defineRecipe(TOOL_SWORD,4,2,ITEM_WOOD,5,ITEM_GEM,50);
    anvilRecipes.recipes[11] = defineRecipe(TOOL_AXE,4,2,ITEM_WOOD,5,ITEM_GEM,50);
    anvilRecipes.recipes[12] = defineRecipe(TOOL_HOE,4,2,ITEM_WOOD,5,ITEM_GEM,50);
    anvilRecipes.recipes[13] = defineRecipe(TOOL_PICKAXE,4,2,ITEM_WOOD,5,ITEM_GEM,50);
    anvilRecipes.recipes[14] = defineRecipe(TOOL_SHOVEL,4,2,ITEM_WOOD,5,ITEM_GEM,50);
       	
    furnaceRecipes.size = 3;
    furnaceRecipes.recipes = (Recipe*)malloc(sizeof(Recipe) * (furnaceRecipes.size));
    furnaceRecipes.recipes[0] = defineRecipe(ITEM_IRONINGOT,1,2,ITEM_IRONORE,4,ITEM_COAL,1);
    furnaceRecipes.recipes[1] = defineRecipe(ITEM_GOLDINGOT,1,2,ITEM_GOLDORE,4,ITEM_COAL,1);
    furnaceRecipes.recipes[2] = defineRecipe(ITEM_GLASS,1,2,ITEM_SAND,4,ITEM_COAL,1);
    	
    ovenRecipes.size = 1;
    ovenRecipes.recipes = (Recipe*)malloc(sizeof(Recipe) * (ovenRecipes.size));
    ovenRecipes.recipes[0] = defineRecipe(ITEM_BREAD,1,1,ITEM_WHEAT,4);
}

/* Free up allocated memory */
void freeRecipes(){
    free(workbenchRecipes.recipes);
    free(ovenRecipes.recipes);
    free(furnaceRecipes.recipes);
    free(anvilRecipes.recipes);
}
