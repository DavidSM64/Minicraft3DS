#pragma once
#include <stdarg.h>
#include "Item.h"

typedef struct {
    int costItem;
    int costAmount;
} Cost;

typedef struct {
    bool canCraft;
    int itemResult;
    int itemAmountLevel;
    s8 numOfCosts;
    Cost costs[6]; // Up to 6 items for costs
    u8 order; // Used for stable sorting.
} Recipe;

typedef struct {
    int size;
    Recipe * recipes;
} RecipeManager;


RecipeManager workbenchRecipes;
RecipeManager furnaceRecipes;
RecipeManager ovenRecipes;
RecipeManager anvilRecipes;

void checkCanCraftRecipes(RecipeManager * rm, Inventory * inv);
void sortRecipes(RecipeManager * rm);
bool craftItem(RecipeManager * rm, Recipe* r, Inventory* inv);

void initRecipes();
void freeRecipes();
