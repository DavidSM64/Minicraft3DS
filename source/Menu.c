#include "Menu.h"

char options[][12] = {"Start Game", "How To Play","Settings", "About", "Exit"};
char pOptions[][24] = {"Return to game", "Save Progress", "Exit to title"};
char keybOptions[][24] = {"Exit and Save", "Exit and Don't save","Reset to default"};
char setOptions[][24] = {"Rebind Buttons", "Texture packs", "Debug Text:   ", "N3DS Speedup:   ", "Return to title"};

// Rebind buttons menu (Settings)
int keys[] = {
    KEY_A,KEY_B,KEY_X,KEY_Y,
    KEY_CPAD_UP,KEY_CPAD_DOWN,KEY_CPAD_LEFT,KEY_CPAD_RIGHT,
    KEY_DUP, KEY_DDOWN, KEY_DLEFT, KEY_DRIGHT, 
    KEY_CSTICK_UP,KEY_CSTICK_DOWN,KEY_CSTICK_LEFT,KEY_CSTICK_RIGHT,
    KEY_L,KEY_R,KEY_ZL,KEY_ZR,
    KEY_START,KEY_SELECT
};
int keyProp[12] = {[0 ... 11] = 0};
bool areYouSure = false;
bool areYouSureSave = false;
bool bindOpt = false;
bool left = false;
bool selBut = false;
s8 errorBut = -1;
s8 curSaveSel = 0;

// Load Game Menu (Start Game)
char fileNames[1000][256];
int fileScore[1000];
bool fileWin[1000];
s16 worldFileCount = 0;
bool enteringName = false;
s8 touchDelay = 0;
bool isTouching = false;
int touchX = 0, touchY = 0, touchW = 0, touchH = 0;
s8 errorFileName = 0;

// Load Texturepacks Menu
char tpFileNames[1000][256];
char tpFileComment[1000][60];
s16 tpFileCount = 0;
s8 isLoadingTP = 0;

s16 pauseSaveDisplayTimer = 0;

void readFiles(){
    memset(&fileNames, 0, sizeof(fileNames)); // reset fileNames
    worldFileCount = 0;
    DIR * d;
    struct dirent * dir;
    d = opendir(".");
    if (d){
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".wld") != NULL) { // Check if filename contains ".wld"
                strncpy(fileNames[worldFileCount], dir->d_name, strlen(dir->d_name)-4);
                FILE * file = fopen(dir->d_name, "rb");
                fread(&fileScore[worldFileCount],sizeof(int), 1, file);
                fread(&fileWin[worldFileCount],sizeof(bool), 1, file);
                fclose(file);
                ++worldFileCount;
            }
        }
        closedir(d);
    }
}

void readTPFiles(){
    memset(&tpFileNames, 0, sizeof(tpFileNames)); // reset tp fileNames
    memset(&tpFileComment, 0, sizeof(tpFileComment)); // reset zip comments
    tpFileCount = 1; // 0 = default.
    DIR * d;
    struct dirent * dir;
    d = opendir("./texturepacks/");
    if (d){
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".zip") != NULL) { // Check if filename contains ".zip"
                strncpy(tpFileNames[tpFileCount], dir->d_name, strlen(dir->d_name)-4);
                
                char fullDirName[256];
                sprintf(fullDirName,"./texturepacks/%s",dir->d_name);
                //int err = 
                getTexturePackComment(fullDirName, tpFileComment[tpFileCount]);
                /*
                if(err > 0){
                    char errorText[10];    
                    sprintf(errorText,"err:%d",err);
                    strncpy(tpFileComment[tpFileCount], errorText, strlen(errorText));
                }*/
                ++tpFileCount;
            }
        }
        closedir(d);
    }
}

s8 checkFileNameForErrors(){
    int length = strlen(fileNames[worldFileCount]);
    if(length < 1)return 1; // Error: Length cannot be 0.
    int i;
    bool isGood = false;
    for(i=0; i < length; ++i){ if(isalnum((int)fileNames[worldFileCount][i])) isGood = true; }
    if(!isGood) return 2; // Error: Filename must contain atleast one letter or number.
    
    DIR * d;
    struct dirent * dir;
    d = opendir(".");
    if (d){
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".wld") != NULL) { // Check if filename contains ".wld"
                char cmprFile[256];
                strncpy(cmprFile, dir->d_name, strlen(dir->d_name)-4);
                if(strncmp(fileNames[worldFileCount],cmprFile,strlen(fileNames[worldFileCount])) == 0) return 3; // Error: Filename cannot already exist.
            }
        }
        closedir(d);
    }
    
    return 0; // No errors found!
}

void addToFileName(char * c){
    strncat(fileNames[worldFileCount], c, 1);
}

/* Keypad */
void doTouchButton(){
    int xVal = k_touch.px, yVal = k_touch.py;
    int strLength = strlen(fileNames[worldFileCount]);
    if(yVal >= 60 && yVal < 80){ // 0 to 9
        if(xVal >= 4 && xVal < 4+16){ touchX = 4; if(strLength < 24)addToFileName("1");}
        else if(xVal >= 36 && xVal < 4+32+16){ touchX = 36; if(strLength < 24)addToFileName("2");}
        else if(xVal >= 68 && xVal < 4+64+16){ touchX = 68; if(strLength < 24)addToFileName("3");}
        else if(xVal >= 100 && xVal < 4+96+16){ touchX = 100; if(strLength < 24)addToFileName("4");}
        else if(xVal >= 132 && xVal < 4+128+16){ touchX = 132; if(strLength < 24)addToFileName("5");}
        else if(xVal >= 164 && xVal < 4+160+16){ touchX = 164; if(strLength < 24)addToFileName("6");}
        else if(xVal >= 196 && xVal < 4+192+16){ touchX = 196; if(strLength < 24)addToFileName("7");}
        else if(xVal >= 228 && xVal < 4+224+16){ touchX = 228; if(strLength < 24)addToFileName("8");}
        else if(xVal >= 260 && xVal < 4+256+16){ touchX = 260; if(strLength < 24)addToFileName("9");}
        else if(xVal >= 292 && xVal < 4+288+16){ touchX = 292; if(strLength < 24)addToFileName("0");}
        else return;
        touchY = 56;
        touchW = 22;
        touchH = 22;
        touchDelay = 6;
        isTouching = true;
    } else if(yVal >= 80 && yVal < 100){ // Q to P
        if(xVal >= 4 && xVal < 4+16){ touchX = 4; if(strLength < 24)addToFileName("Q");}
        else if(xVal >= 36 && xVal < 4+32+16){ touchX = 36; if(strLength < 24)addToFileName("W");}
        else if(xVal >= 68 && xVal < 4+64+16){ touchX = 68; if(strLength < 24)addToFileName("E");}
        else if(xVal >= 100 && xVal < 4+96+16){ touchX = 100; if(strLength < 24)addToFileName("R");}
        else if(xVal >= 132 && xVal < 4+128+16){ touchX = 132; if(strLength < 24)addToFileName("T");}
        else if(xVal >= 164 && xVal < 4+160+16){ touchX = 164; if(strLength < 24)addToFileName("Y");}
        else if(xVal >= 196 && xVal < 4+192+16){ touchX = 196; if(strLength < 24)addToFileName("U");}
        else if(xVal >= 228 && xVal < 4+224+16){ touchX = 228; if(strLength < 24)addToFileName("I");}
        else if(xVal >= 260 && xVal < 4+256+16){ touchX = 260; if(strLength < 24)addToFileName("O");}
        else if(xVal >= 292 && xVal < 4+288+16){ touchX = 292; if(strLength < 24)addToFileName("P");}
        else return;
        touchY = 76;
        touchW = 22;
        touchH = 22;
        touchDelay = 6;
        isTouching = true;
    } else if(yVal >= 100 && yVal < 120){ // A to L
        if(xVal >= 20 && xVal < 36){ touchX = 20; if(strLength < 24)addToFileName("A");}
        else if(xVal >= 52 && xVal < 68){ touchX = 52; if(strLength < 24)addToFileName("S");}
        else if(xVal >= 84 && xVal < 100){ touchX = 84; if(strLength < 24)addToFileName("D");}
        else if(xVal >= 116 && xVal < 132){ touchX = 116; if(strLength < 24)addToFileName("F");}
        else if(xVal >= 148 && xVal < 164){ touchX = 148; if(strLength < 24)addToFileName("G");}
        else if(xVal >= 180 && xVal < 196){ touchX = 180; if(strLength < 24)addToFileName("H");}
        else if(xVal >= 212 && xVal < 230){ touchX = 212; if(strLength < 24)addToFileName("J");}
        else if(xVal >= 244 && xVal < 262){ touchX = 244; if(strLength < 24)addToFileName("K");}
        else if(xVal >= 276 && xVal < 294){ touchX = 276; if(strLength < 24)addToFileName("L");}
        else return;
        touchY = 96;
        touchW = 22;
        touchH = 22;
        touchDelay = 6;
        isTouching = true;
    } else if(yVal >= 120 && yVal < 140){ // Z to M
        if(xVal >= 52 && xVal < 68){ touchX = 52; if(strLength < 24)addToFileName("Z");}
        else if(xVal >= 84 && xVal < 100){ touchX = 84; if(strLength < 24)addToFileName("X");}
        else if(xVal >= 116 && xVal < 132){ touchX = 116; if(strLength < 24)addToFileName("C");}
        else if(xVal >= 148 && xVal < 164){ touchX = 148; if(strLength < 24)addToFileName("V");}
        else if(xVal >= 180 && xVal < 196){ touchX = 180; if(strLength < 24)addToFileName("B");}
        else if(xVal >= 212 && xVal < 230){ touchX = 212; if(strLength < 24)addToFileName("N");}
        else if(xVal >= 244 && xVal < 262){ touchX = 244; if(strLength < 24)addToFileName("M");}
        else return;
        touchY = 116;
        touchW = 22;
        touchH = 22;
        touchDelay = 6;
        isTouching = true;
    } else if(yVal >= 140 && yVal < 160){ // SPACE and BACKSPACE
        if(xVal >= 32 && xVal < 112){ 
            touchX = 32; 
            touchW = 16*5+14;
            if(strLength < 24)addToFileName("_"); // Underscores don't appear in the game.
        }
        else if(xVal >= 148 && xVal < 262){ 
            if(strLength <= 0) return;
            touchX = 148; 
            touchW = 16*9+8;
            fileNames[worldFileCount][strLength-1] = '\0';
        }
        else return;
        touchY = 136;
        touchH = 22;
        touchDelay = 6;
        isTouching = true;
    }
    errorFileName = 0;
}

void switchGameBut(bool left, int buttonID){
    int id;
    for(id = 0; id < 7; ++id){
        if(keyProp[id] & buttonID){
            keyProp[id] ^= buttonID; // Toggle buttonID bit
            if(left){
                int id2 = id -1;
                if (id2 < 0) return;
                keyProp[id2] ^= buttonID;
            } else { 
                int id2 = id+1;
                if (id2 > 6) return;
                keyProp[id2] ^= buttonID;
            }
            return;
        }
    }
    if(left) keyProp[6] ^= buttonID;
    else keyProp[0] ^= buttonID;
    
}
void switchMenuBut(bool left, int buttonID){
    int id;
    for(id = 0; id < 12; ++id){
        if(id > 3 && id < 7) continue;
        if(keyProp[id] & buttonID){
            keyProp[id] ^= buttonID; // Toggle buttonID bit
            if(left){
                int id2 = id - 1;
                if (id2 == 6) id2 = 3;
                if (id2 < 0) return;
                keyProp[id2] ^= buttonID;
            } else { 
                int id2 = id+1;
                if (id2 == 4) id2 = 7;
                if (id2 > 11) return;
                keyProp[id2] ^= buttonID;
            }
            return;
        }
    }
    if(left) keyProp[11] ^= buttonID;
    else keyProp[0] ^= buttonID;
}

s8 checkPropButtons(){
    if(keyProp[0] == 0) return 0;
    if(keyProp[1] == 0) return 1;
    if(keyProp[2] == 0) return 2;
    if(keyProp[3] == 0) return 3;
    if(keyProp[4] == 0) return 4;
    if(keyProp[5] == 0) return 5;
    if(keyProp[6] == 0) return 6;
    if(keyProp[7] == 0) return 7;
    if(keyProp[8] == 0) return 8;
    if(keyProp[9] == 0) return 9;
    if(keyProp[10] == 0) return 10;
    if(keyProp[11] == 0) return 11;
    return -1;
}

Item median;
void tickMenu(int menu){
    switch(menu){
        case MENU_SETTINGS_REBIND:
        if(!bindOpt){
            if(!selBut){
		        if (k_up.clicked){ --currentSelection; if(currentSelection < 0)currentSelection=21;}
		        if (k_down.clicked){ ++currentSelection; if(currentSelection > 21)currentSelection=0;}
		        if (k_left.clicked){ left = true;}
		        if (k_right.clicked){ left = false;}
            } else {
                if (k_left.clicked){ 
                    if(left)switchGameBut(true,keys[currentSelection]);
                    else switchMenuBut(true,keys[currentSelection]);
                } else if (k_right.clicked) { 
                    if(left)switchGameBut(false,keys[currentSelection]);
                    else switchMenuBut(false,keys[currentSelection]);
                }
            }
		    
            if (k_accept.clicked) selBut = !selBut;
            if (k_decline.clicked){ 
                bindOpt = true;
                curSaveSel = 0;
            }
        } else {
		    if (k_up.clicked){ --curSaveSel; if(curSaveSel < 0)curSaveSel=2;}
		    if (k_down.clicked){ ++curSaveSel; if(curSaveSel > 2)curSaveSel=0;}
            if (k_decline.clicked){ 
                bindOpt = false;
                errorBut = -1;
            }
            if (k_accept.clicked){
                switch(curSaveSel){
                    case 0: // Exit and save  
                        if(checkPropButtons() == -1){
                            k_up.input = keyProp[0];
                            k_down.input = keyProp[1];
                            k_left.input = keyProp[2];
                            k_right.input = keyProp[3];
                            k_attack.input = keyProp[4];
                            k_menu.input = keyProp[5];
                            k_pause.input = keyProp[6];
                            k_accept.input = keyProp[7];
                            k_decline.input = keyProp[8];
                            k_delete.input = keyProp[9];
                            k_menuNext.input = keyProp[10];
                            k_menuPrev.input = keyProp[11];
                            
                            FILE *fs=fopen("btnSave.bin","wb");
                            fwrite(keyProp,sizeof(int),12,fs);
                            fclose(fs);
                            
                            currentSelection = 0;
                            currentMenu = MENU_SETTINGS;
                            errorBut = -1;
                        } else errorBut = checkPropButtons();
                        break;
                    case 1: // Exit and DON'T save  
                        currentSelection = 0;
                        currentMenu = MENU_SETTINGS;
                        errorBut = -1;
                        break;
                    case 2: // reset defaults
	                    keyProp[0] = KEY_DUP | KEY_CPAD_UP | KEY_CSTICK_UP;
	                    keyProp[1] = KEY_DDOWN | KEY_CPAD_DOWN | KEY_CSTICK_DOWN;
	                    keyProp[2] = KEY_DLEFT | KEY_CPAD_LEFT | KEY_CSTICK_LEFT;
	                    keyProp[3] = KEY_DRIGHT | KEY_CPAD_RIGHT | KEY_CSTICK_RIGHT;
	                    keyProp[4] = KEY_A | KEY_B | KEY_L | KEY_ZR;
	                    keyProp[5] = KEY_X | KEY_Y | KEY_R | KEY_ZL;
	                    keyProp[6] = KEY_START;
                        keyProp[7] = KEY_A;
                        keyProp[8] = KEY_B;
                        keyProp[9] = KEY_X;
                        keyProp[10] = KEY_R;
                        keyProp[11] = KEY_L;
	                    bindOpt = false;
                        errorBut = -1;
                        break;
                    }
                }
            }
        break;
        case MENU_PAUSED:
            if(!areYouSure && !areYouSureSave){
                if(pauseSaveDisplayTimer > 0) --pauseSaveDisplayTimer;
                if (k_pause.clicked) currentMenu = MENU_NONE;
		        if (k_up.clicked){ --currentSelection; if(currentSelection < 0)currentSelection=2;}
		        if (k_down.clicked){ ++currentSelection; if(currentSelection > 2)currentSelection=0;}
                if (k_accept.clicked){
                    switch(currentSelection){
                    case 0:      
                        currentMenu = MENU_NONE;
                        break;
                    case 1:
	                    areYouSureSave = true;
                        break;
                    case 2:
	                    areYouSure = true;
                        break;
                    }
                }
            } else if(areYouSureSave) {
                if (k_accept.clicked){
                    pauseSaveDisplayTimer = 60;
                    saveCurrentWorld(currentFileName, &eManager, &player, (u8*)map, (u8*)data);
                    areYouSureSave = false;
                    areYouSure = false;
                } else if (k_decline.clicked){
                    areYouSureSave = false;
                    areYouSure = false;
                }
            } else {
                if (k_accept.clicked){
                    areYouSure = false;
                    areYouSureSave = false;
	                sf2d_set_clear_color(0xFF000000);
                    currentSelection = 0;
                    currentMenu = MENU_TITLE;
                } else if (k_decline.clicked){
                    areYouSure = false;
                    areYouSureSave = false;
                }
            }
        break;
        case MENU_INVENTORY:
            if (k_menu.clicked || k_decline.clicked){
                 currentMenu = MENU_NONE;
                 player.p.activeItem = &noItem;
                 player.p.isCarrying = false;
            }
            if (k_accept.clicked){ // Select item from inventory
                if(player.p.inv->lastSlot!=0){
                    median = player.p.inv->items[curInvSel]; // create copy of item.
                    removeItemFromInventory(curInvSel, player.p.inv); // remove original
                    pushItemToInventoryFront(median, player.p.inv); // add copy to front
                    player.p.activeItem = &player.p.inv->items[0]; // active item = copy.
                    if(player.p.activeItem->id > 27) player.p.isCarrying = true;
                    else player.p.isCarrying = false;
                }
                currentMenu = MENU_NONE;
            }
		    if (k_up.clicked){ --curInvSel; if(curInvSel < 0)curInvSel=player.p.inv->lastSlot-1;}
		    if (k_down.clicked){ ++curInvSel; if(curInvSel > player.p.inv->lastSlot-1)curInvSel=0;}
        break;
        
        case MENU_CRAFTING:
        if (k_menu.clicked || k_decline.clicked) currentMenu = MENU_NONE;
        if (k_accept.clicked){
            int newslot = player.p.activeItem->slotNum + 1;
            if(craftItem(currentRecipes, &currentRecipes->recipes[curInvSel], player.p.inv)){
                playSound(snd_craft);
                if(player.p.activeItem != &noItem)player.p.activeItem = &player.p.inv->items[newslot];
            }
        }
		if (k_up.clicked){ --curInvSel; if(curInvSel < 0)curInvSel=currentRecipes->size-1;}
		if (k_down.clicked){ ++curInvSel; if(curInvSel > currentRecipes->size-1)curInvSel=0;}
        break;
        
        case MENU_WIN:
            if (k_accept.clicked){ 
	            sf2d_set_clear_color(0xFF000000);
                currentSelection = 0;
                currentMenu = MENU_TITLE;
                saveCurrentWorld(currentFileName, &eManager, &player, (u8*)map, (u8*)data);
            }
        break;
        case MENU_LOSE:
            if (k_accept.clicked){ 
	            sf2d_set_clear_color(0xFF000000);
                currentSelection = 0;
                currentMenu = MENU_TITLE;
            }
        break;
        case MENU_ABOUT:
            if (k_decline.clicked) currentMenu = MENU_TITLE;
        break;
        
        case MENU_CONTAINER:
        if (k_menu.clicked || k_decline.clicked)currentMenu = MENU_NONE;
        
        if (k_left.clicked) {
			curChestEntity->entityFurniture.r = 0;
			int tmp = curInvSel;
			curInvSel = curChestEntity->entityFurniture.oSel;
			curChestEntity->entityFurniture.oSel = tmp;
		}
		if (k_right.clicked) {
			curChestEntity->entityFurniture.r = 1;
			int tmp = curInvSel;
			curInvSel = curChestEntity->entityFurniture.oSel;
			curChestEntity->entityFurniture.oSel = tmp;
		}
		
		Inventory* i1 = curChestEntity->entityFurniture.r == 1 ? player.p.inv : curChestEntity->entityFurniture.inv;
		Inventory* i2 = curChestEntity->entityFurniture.r == 0 ? player.p.inv : curChestEntity->entityFurniture.inv;
		int len = i1->lastSlot;
		if (curInvSel < 0) curInvSel = 0;
		if (curInvSel >= len) curInvSel = len - 1;
		if (k_up.clicked) --curInvSel;
		if (k_down.clicked) ++curInvSel;
		if (len == 0) curInvSel = 0;
		if (curInvSel < 0) curInvSel += len;
		if (curInvSel >= len) curInvSel -= len;
		
		if(k_accept.clicked && len > 0){
            Item* pullItem = &i1->items[curInvSel];
            Item pushItem = newItem(pullItem->id,pullItem->countLevel);
            pushItem.chestPtr = pullItem->chestPtr;
            pushItemToInventoryFront(pushItem, i2);
            if(i2 == player.p.inv){
                int newslot = player.p.activeItem->slotNum + 1;
                player.p.activeItem = &player.p.inv->items[newslot];
            } else if(pullItem == player.p.activeItem){
                player.p.activeItem = &noItem;
            }
            removeItemFromCurrentInv(pullItem);
			if (curInvSel >= i1->lastSlot) curInvSel = i1->lastSlot - 1;
        }
		
        break;
        
        case MENU_LOADGAME:
            if(!enteringName && !areYouSure){ // World select
                if (k_decline.clicked){
                    currentMenu = MENU_TITLE;
                    currentSelection = 0;
                }
                if (k_up.clicked){ --currentSelection; if(currentSelection < 0)currentSelection = worldFileCount;}
                if (k_down.clicked){ ++currentSelection; if(currentSelection > worldFileCount)currentSelection=0;}
            
                if(k_delete.clicked){
                    if(currentSelection < worldFileCount) areYouSure = true;
                }
            
                if(k_accept.clicked){
                    if(currentSelection == worldFileCount){
                        
                        enteringName = true;
                    } else {
                        memset(&currentFileName, 0, 255); // reset currentFileName
                            sprintf(currentFileName,"%s.wld",fileNames[currentSelection]);
                            playSound(snd_test);
                            initGame = 1;
                            currentMenu = MENU_NONE;
                    }
                }
            } else if (areYouSure){
                if (k_decline.clicked || k_delete.clicked) areYouSure = false;
                if (k_accept.clicked){
                    sprintf(currentFileName,"%s.wld",fileNames[currentSelection]);
                    remove(currentFileName);
                    readFiles();
                    enteringName = false;
                    areYouSure = false;
                    memset(&currentFileName, 0, 255); // reset currentFileName
                }
            }else { // Enter new world name.
                if(k_decline.clicked) enteringName = false;
                if(k_accept.clicked && errorFileName == 0){
                    errorFileName = checkFileNameForErrors();
                    if(errorFileName == 0){ // If no errors are found with the filename, then start a new game!
                        memset(&currentFileName, 0, 255); // reset currentFileName
                        sprintf(currentFileName,"%s.wld",fileNames[worldFileCount]);
                        currentMenu = MENU_NONE;
                        playSound(snd_test);
                        initGame = 2;
                        ++worldFileCount;
                    }
                }
                if((k_touch.px != 0 || k_touch.py != 0) && touchDelay == 0){ 
                    if(!isTouching)doTouchButton(); 
                }
                else if(k_touch.px == 0 || k_touch.py == 0) isTouching = false;
                if(touchDelay > 0) --touchDelay;
            }
        break;
        case MENU_SETTINGS_TP:
            
            if (k_up.clicked){ --currentSelection; if(currentSelection < 0)currentSelection = tpFileCount-1;}
            if (k_down.clicked){ ++currentSelection; if(currentSelection > tpFileCount-1)currentSelection=0;}
            if (k_decline.clicked){
                if(isLoadingTP < 1){
                    currentMenu = MENU_SETTINGS;
                    currentSelection = 1;
                }
            }
            if (k_accept.clicked){
                
                if(currentSelection > 0){
                    isLoadingTP = 4;
                } else {
	                icons = sfil_load_PNG_buffer(icons2_png, SF2D_PLACE_RAM);
	                dirtColor[0] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 0)); 
	                dirtColor[1] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 1)); 
	                dirtColor[2] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 2)); 
	                dirtColor[3] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 3)); 
	                dirtColor[4] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 4)); 
	                font = sfil_load_PNG_buffer(Font_png, SF2D_PLACE_RAM);
	                bottombg = sfil_load_PNG_buffer(bottombg_png, SF2D_PLACE_RAM);
                    currentMenu = MENU_SETTINGS;
                    currentSelection = 1;
                    remove("lastTP.bin");
                }
            }
        break;
        
        case MENU_SETTINGS:
		    if (k_up.clicked){ 
                --currentSelection; 
                if(currentSelection == 3 && !((MODEL_3DS & 6) != 0)) --currentSelection; 
                if(currentSelection < 0)currentSelection=4;
            }
		    if (k_down.clicked){ 
                ++currentSelection; 
                if(currentSelection == 3 && !((MODEL_3DS & 6) != 0)) ++currentSelection; 
                if(currentSelection > 4)currentSelection=0;
            }
            if(k_decline.clicked){
                currentMenu = MENU_TITLE;
                currentSelection = 2;
            }
		    if(k_accept.clicked){
                switch(currentSelection){
                    case 0:
                        keyProp[0] = k_up.input;
                        keyProp[1] = k_down.input;
                        keyProp[2] = k_left.input;
                        keyProp[3] = k_right.input;
                        keyProp[4] = k_attack.input;
                        keyProp[5] = k_menu.input;
                        keyProp[6] = k_pause.input;
                        keyProp[7] = k_accept.input;
                        keyProp[8] = k_decline.input;
                        keyProp[9] = k_delete.input;
                        keyProp[10] = k_menuNext.input;
                        keyProp[11] = k_menuPrev.input;
                        left = true;
                        selBut = false;
                        bindOpt = false;
                        currentSelection = 0;
                        currentMenu = MENU_SETTINGS_REBIND;
                        break;
                    case 1:
                        readTPFiles();
                        currentMenu = MENU_SETTINGS_TP;
                        currentSelection = 0;
                        break;
                    case 2:
                        shouldRenderDebug = !shouldRenderDebug; // toggle option
                        break;
                    case 3:
                        if((MODEL_3DS & 6) != 0){ // detect if user is using a New 3DS
                            shouldSpeedup = !shouldSpeedup; // toggle option
                            osSetSpeedupEnable(shouldSpeedup);
                        }
                        break;
                    case 4:
                        if(true){
                            FILE * fset = fopen("settings.bin","wb");
                            fwrite(&shouldRenderDebug,sizeof(bool),1,fset);
                            fwrite(&shouldSpeedup,sizeof(bool),1,fset);
                            fclose(fset);
                        }
                        currentMenu = MENU_TITLE;
                        currentSelection = 2;
                        break;
                }
            }
        break;
        case MENU_TITLE:
		    if (k_up.clicked){ --currentSelection; if(currentSelection < 0)currentSelection=4;}
		    if (k_down.clicked){ ++currentSelection; if(currentSelection > 4)currentSelection=0;}
		    
		    if(k_accept.clicked){
                switch(currentSelection){
                    case 0:
                        currentMenu = MENU_LOADGAME;
                        readFiles();
                        currentSelection = 0;
                        enteringName = false;
                        areYouSure = false;
                    break;
                    case 1:
                        sprintf(pageText,"Page: %d/%d",pageNum+1,maxPageNum+1);
                        currentMenu = MENU_TUTORIAL;
                    break;
                    case 2:
                        currentSelection = 0;
                        currentMenu = MENU_SETTINGS;
                    break;
                    case 3: 
                        currentMenu = MENU_ABOUT;
                    break;
                    case 4: 
                        quitGame = true;
                    break;
                }
                
            }
        break;
        case MENU_TUTORIAL:
            if(k_decline.clicked){ 
                currentSelection = 1;
                currentMenu = MENU_TITLE;
            }
            if(k_menuNext.clicked){
                if(pageNum < maxPageNum){ 
                    ++pageNum;
                    sprintf(pageText,"Page: %d/%d",pageNum+1,maxPageNum+1);
                }
            }
            if(k_menuPrev.clicked){
                if(pageNum > 0){
                    --pageNum;
                    sprintf(pageText,"Page: %d/%d",pageNum+1,maxPageNum+1);
                }
            }
            
        break;
    }
    
}

u8 opacity = 255;
bool rev = true;
char scoreText[15];

char * getButtonFunctionGame(int key){
    if(keyProp[0] & key) return "Move up";
    if(keyProp[1] & key) return "Move down";
    if(keyProp[2] & key) return "Move left";
    if(keyProp[3] & key) return "Move right";
    if(keyProp[4] & key) return "Attack";
    if(keyProp[5] & key) return "Toggle Menu";
    if(keyProp[6] & key) return "Pause";
    return "Nothing";
}
char * getButtonFunctionMenu(int key){
    if(keyProp[0] & key) return "Up";
    if(keyProp[1] & key) return "Down";
    if(keyProp[2] & key) return "Left";
    if(keyProp[3] & key) return "Right";
    if(keyProp[7] & key) return "Accept";
    if(keyProp[8] & key) return "Decline";
    if(keyProp[9] & key) return "Delete";
    if(keyProp[10] & key) return "Next";
    if(keyProp[11] & key) return "Previous";
    return "Nothing";
}


char guiText0[] = "1 2 3 4 5 6 7 8 9 0";
char guiText1[] = "Q W E R T Y U I O P";
char guiText2[] = "A S D F G H J K L";
char guiText3[] = "Z X C V B N M";
char guiText4[] = " SPACE  BACKSPACE";

void renderMenu(int menu,int xscr,int yscr){
    int i = 0;
    switch(menu){
        case MENU_TUTORIAL:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
		        renderTutorialPage(true);
		    sf2d_end_frame();
		    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);	
		        renderTutorialPage(false);
		    sf2d_end_frame();
            break;
        case MENU_SETTINGS_TP:
            offsetX = 0;offsetY = (currentSelection * 40) - 48;
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
		        drawText("Texture Packs",122,-16);
		        for(i = 0; i < tpFileCount; ++i){
                    int color = 0xFF923232;
                    char * text = tpFileNames[i];
                    char * cmtText = tpFileComment[i];
                    if(i == 0){
                        text = "Default";
                        cmtText = "Regular look of the game";
                        color = 0xFF921060;
                    }
                    if(i != currentSelection) color &= 0x7FFFFFFF; // Darken color.
                    else if(areYouSure)color = 0xFF1010DF;
                    
                    renderFrame(1,i*5,24,(i*5)+5,color);
                    drawText(text,(400-(strlen(text)*12))/2,i*80+16);
                    
                    if(strlen(cmtText) > 29){
                        char cmtTxt1[30],cmtTxt2[30];
                        strncpy(cmtTxt1, cmtText, 29);
                        strncpy(cmtTxt2, cmtText + 29, strlen(cmtText)-29);
                        drawTextColor(cmtTxt1,(400-(strlen(cmtTxt1)*12))/2,i*80+36,0xFFAFAFAF);
                        drawTextColor(cmtTxt2,(400-(strlen(cmtTxt2)*12))/2,i*80+50,0xFFAFAFAF);
                    } else {
                        drawTextColor(cmtText,(400-(strlen(cmtText)*12))/2,i*80+43,0xFFAFAFAF);
                    }
                }
            offsetX = 0;offsetY = 0;
                if(isLoadingTP > 0){
                    --isLoadingTP;
                    renderFrame(1,5,24,9,0xFF666666);
                    drawTextColor("Loading Texture pack...",(400-(23*12))/2,108,0xFF10FFFF);
                    if(isLoadingTP == 0){
                        char fullDirName[256];
                        sprintf(fullDirName,"texturepacks/%s.zip",tpFileNames[currentSelection]);
                        loadedtp = loadTexturePack(fullDirName);   
                        
                        FILE * fs=fopen("lastTP.bin","w");
                        fprintf(fs,"%s", fullDirName);
                        fclose(fs);
                        
                        currentMenu = MENU_SETTINGS;
                        currentSelection = 1; 
                    }
                }
		    sf2d_end_frame();
		    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);	
                drawText("Press   to select", 58, 100);
                renderButtonIcon(k_accept.input & -k_accept.input, 128, 98, 1);
                drawText("Press   to return", 58, 150);
                renderButtonIcon(k_decline.input & -k_decline.input, 128, 148, 1);
		    sf2d_end_frame();
        break;
        case MENU_LOADGAME:
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
        if(!enteringName){ // World select
            offsetX = 0;offsetY = (currentSelection * 32) - 48;
            drawText("Select a file",122,-16);
            for(i = 0; i < worldFileCount + 1; ++i){
                int color = 0xFF921020;
                char * text = fileNames[i];
                if(i == worldFileCount){
                    text = "Generate New World";
                    color = 0xFF209210;
                }
                if(i != currentSelection) color &= 0xFF7F7F7F; // Darken color.
                else {
                    if(areYouSure)color = 0xFF1010DF;
                }
                
                char scoreText[24];
                sprintf(scoreText,"Score: %d",fileScore[i]);
                
                renderFrame(1,i*4,24,(i*4)+4,color);
                if(i != worldFileCount){
                    drawText(text,(400-(strlen(text)*12))/2,i*64+12);
                    drawText(scoreText,(400-(strlen(scoreText)*12))/2,i*64+32);
                } else {
                    drawText(text,(400-(strlen(text)*12))/2,i*64+24);
                }
                if(fileWin[i] && i != worldFileCount) render16(18,i*32+8,24,208,0); // Render crown
            }
            offsetX = 0;offsetY = 0;
        } else { // Enter new world name.
            drawText("Enter a name",128,16);
            drawText(fileNames[worldFileCount],(400-(strlen(fileNames[worldFileCount])*12))/2, 48);
            
            if(errorFileName > 0){
                switch(errorFileName){// Error: Filename cannot already exist.
                    case 1: drawTextColor("ERROR: Length cannot be 0!",(400-26*12)/2,200,0xFF1010AF); break;    
                    case 2: drawTextColor("ERROR: You need Letters/Numbers!",(400-32*12)/2,200,0xFF1010AF); break;    
                    case 3: drawTextColor("ERROR: Filename already exists!",(400-31*12)/2,200,0xFF1010AF); break;    
                }    
            }
        }
		sf2d_end_frame();
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        if(!enteringName){ // World select
            if(!areYouSure){
                drawTextColor("Load World",100,12,0xFF3FFFFF);
                drawText("Press   or   to scroll", 28, 50);
                renderButtonIcon(k_up.input & -k_up.input, 98, 48, 1);
                renderButtonIcon(k_down.input & -k_down.input, 160, 48, 1);
                drawText("Press   to load world", (320-21*12)/2, 100);
                renderButtonIcon(k_accept.input & -k_accept.input, 104, 98, 1);
                drawText("Press   to return", 58, 150);
                renderButtonIcon(k_decline.input & -k_decline.input, 128, 148, 1);
                if(currentSelection != worldFileCount){
                    drawText("Press   to delete",(320-17*12)/2, 200);
                    renderButtonIcon(k_delete.input & -k_delete.input, 128, 198, 1);
                }
            } else {
                drawTextColor("Delete File?",88,12,0xFF3F3FFF);
                drawText("Press   to confirm", (320-18*12)/2, 100);
                renderButtonIcon(k_accept.input & -k_accept.input, 122, 98, 1);
                drawText("Press   to return", 58, 150);
                renderButtonIcon(k_decline.input & -k_decline.input, 128, 148, 1);
            }
            
        } else { // Draw the "keyboard"
            drawTextColor("Touch the keypad below",(320-22*12)/2,12,0xFF33FFFF);
            
            sf2d_draw_rectangle(0, 50, 320, 110, 0xFF7F7FBF);
            drawSizedText(guiText0,4, 60, 2);
            drawSizedText(guiText1,4, 80, 2);
            drawSizedText(guiText2,12, 100, 2);
            drawSizedText(guiText3,28, 120, 2);
            drawSizedText(guiText4,12, 140, 2);
            
            if(touchDelay > 0){
                sf2d_draw_rectangle(touchX, touchY, touchW, touchH, 0xAF000000);
            }
            
            drawText("Press   to confirm", (320-18*12)/2, 180);
            renderButtonIcon(k_accept.input & -k_accept.input, 122, 178, 1);
            drawText("Press   to return", 58, 210);
            renderButtonIcon(k_decline.input & -k_decline.input, 128, 208, 1);
        }
            
		sf2d_end_frame();
        break;
        case MENU_SETTINGS_REBIND:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
		        drawTextColor("Rebind Buttons",116,12,0xFF00AFAF);
                drawText("Button",16,32);
                drawText("Game",140,32);
                drawText("Menus",280,32);
		    
                char gameButText[34];
                char menuButText[34];
                    
                for(i = 0; i < 5; ++i){
                    if((currentSelection-2) + i > 21 || (currentSelection-2) + i < 0) continue;
                    renderButtonIcon(keys[(currentSelection-2) + i], 16, (i * 18) + 30, 2);
                    int ccol = 0xFF7F7F7F;
                    
                    sprintf(gameButText,"%s",getButtonFunctionGame(keys[(currentSelection-2) + i]));
                    sprintf(menuButText,"%s",getButtonFunctionMenu(keys[(currentSelection-2) + i]));
                    
                    if(i == 2){ 
                        if(!selBut)ccol = 0xFFFFFFFF;
                        else{ 
                            ccol = 0xFF00FF00;
                            if(left)sprintf(gameButText,"<%s>",getButtonFunctionGame(keys[(currentSelection-2) + i]));
                            else sprintf(menuButText,"<%s>",getButtonFunctionMenu(keys[(currentSelection-2) + i]));
                        }
                    }
                    if(left){
                        drawTextColor(gameButText, 112, (i * 33) + 80, ccol);
                        drawTextColor(menuButText, 280, (i * 33) + 80, 0xFF7F7F7F);
                    } else {
                        drawTextColor(gameButText, 112, (i * 33) + 80, 0xFF7F7F7F);
                        drawTextColor(menuButText, 280, (i * 33) + 80, ccol);
                    }
                }
                if(bindOpt){
                    renderFrame(1,1,24,14,0xFFBF1010);
                    drawTextColor("Save changes?",122,32,0xFF00AFAF);
                    for(i = 2; i >= 0; --i){
                        char* msg = keybOptions[i];
                        u32 color = 0xFF4F4F4F;
                        if(i == curSaveSel) color = 0xFFFFFFFF;
                        drawTextColor(msg,(400 - (strlen(msg) * 12))/2, (i * 24) + 92, color);    
                    }
                    drawText("Press   to return", 98, 190);
                    renderButtonIcon(k_decline.input & -k_decline.input, 168, 188, 1);
                    
                    if(errorBut >= 0 && errorBut < 12){
                        char errorText[30];
                        switch(errorBut){
                            case 0: sprintf(errorText, "Error: Missing 'Move up'"); break;
                            case 1: sprintf(errorText, "Error: Missing 'Move down'"); break;
                            case 2: sprintf(errorText, "Error: Missing 'Move right'"); break;
                            case 3: sprintf(errorText, "Error: Missing 'Move left'"); break;
                            case 4: sprintf(errorText, "Error: Missing 'Attack'"); break;
                            case 5: sprintf(errorText, "Error: Missing 'Toggle Menu'"); break;
                            case 6: sprintf(errorText, "Error: Missing 'Pause'"); break;
                            case 7: sprintf(errorText, "Error: Missing 'Accept'"); break;
                            case 8: sprintf(errorText, "Error: Missing 'Decline'"); break;
                            case 9: sprintf(errorText, "Error: Missing 'Delete'"); break;
                            case 10: sprintf(errorText, "Error: Missing 'Next'"); break;
                            case 11: sprintf(errorText, "Error: Missing 'Previous'"); break;
                        }
                        drawTextColor(errorText,(400 - (strlen(errorText) * 12))/2,50,0xFF0000FF);
                    }
                    
                }
		    sf2d_end_frame();
		    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);	
		    if(!selBut){
                drawText("Press   to select", 58, 80);
                renderButtonIcon(k_accept.input & -k_accept.input, 128, 78, 1);
                drawText("Press   to return", 58, 130);
                renderButtonIcon(k_decline.input & -k_decline.input, 128, 128, 1);
            } else {
                drawText("Press   or   to scroll", 28, 50);
                renderButtonIcon(k_left.input & -k_left.input, 98, 48, 1);
                renderButtonIcon(k_right.input & -k_right.input, 160, 48, 1);
                drawText("Press   to unselect", 46, 100);
                renderButtonIcon(k_accept.input & -k_accept.input, 118, 98, 1);
                drawText("Press   to return", 58, 150);
                renderButtonIcon(k_decline.input & -k_decline.input, 128, 148, 1);
            }
		    sf2d_end_frame();
        break;
        
        case MENU_PAUSED:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
                if(currentLevel == 0){ 
                    sf2d_draw_texture_part_scale(minimap[1],(-xscr/3)-256,(-yscr/3)-32,0,0,128,128,12.5,7.5);
                    sf2d_draw_rectangle(0,0,400,240, 0xAFDFDFDF);
                }
	            offsetX = xscr;offsetY = yscr;
		            renderMenuBackground(xscr,yscr);
	            offsetX = 0;offsetY = 0;
                renderFrame(1,1,24,14,0xFF1010AF);
                drawText("Paused",164,32);
                for(i = 2; i >= 0; --i){
                    char* msg = pOptions[i];
                    u32 color = 0xFF7F7F7F;
                    if(i == currentSelection) color = 0xFFFFFFFF;
                    drawTextColor(msg,(400 - (strlen(msg) * 12))/2, (i * 24) + 100, color);    
                }
                
                if(pauseSaveDisplayTimer > 0) drawTextColor("Game Saved!", (400-(11*12))/2, 64,0xFF20FF20);
                
                if(areYouSure || areYouSureSave){
                    if(areYouSure)renderFrame(6,5,19,10,0xFF10108F);
                    else renderFrame(6,5,19,10,0xFF108F10);
                    
                    drawText("Are you sure?",122,96);
                    drawText("   Yes", 164, 117);
                    renderButtonIcon(k_accept.input & -k_accept.input, 166, 114, 1);
                    drawText("   No", 170, 133);
                    renderButtonIcon(k_decline.input & -k_decline.input, 166, 130, 1);
                }
                
		    sf2d_end_frame();
        break;  
        case MENU_WIN:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
                if(currentLevel == 0){ 
                    sf2d_draw_texture_part_scale(minimap[1],(-xscr/3)-256,(-yscr/3)-32,0,0,128,128,12.5,7.5);
                    sf2d_draw_rectangle(0,0,400,240, 0xAFDFDFDF);
                }
	            offsetX = xscr;offsetY = yscr;
		            renderMenuBackground(xscr,yscr);
	            offsetX = 0;offsetY = 0;
                renderFrame(5,3,21,12,0xFFFF1010);
                if(!rev){ opacity+=5; if(opacity == 255) rev = true; }
                else { opacity-=5; if(opacity == 100) rev = false; }
                sprintf(scoreText,"Score: %d", player.p.score);
                drawTextColor("You Win!",158,76,0xAFAF0000 + opacity);
                drawText(scoreText, 200-((strlen(scoreText)-1)*6), 100);
                drawText("Press   to continue", 96, 150);
                renderButtonIcon(k_attack.input & -k_attack.input, 166, 148, 1);
                
                //printf("0x%08X",k_attack.input & -k_attack.input);
		    sf2d_end_frame();
        break;  
        case MENU_LOSE:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
                if(currentLevel == 0){ 
                    sf2d_draw_texture_part_scale(minimap[1],(-xscr/3)-256,(-yscr/3)-32,0,0,128,128,12.5,7.5);
                    sf2d_draw_rectangle(0,0,400,240, 0xAFDFDFDF);
                }
	            offsetX = xscr;offsetY = yscr;
		            renderMenuBackground(xscr,yscr);
	            offsetX = 0;offsetY = 0;
                renderFrame(5,3,21,12,0xFFFF1010);
                if(!rev){ opacity+=5; if(opacity == 255) rev = true; }
                else { opacity-=5; if(opacity == 100) rev = false; }
                sprintf(scoreText,"Score: %d", player.p.score);
                drawTextColor("You DIED!",158,76,0xAF000000 + opacity);
                drawText(scoreText, 200-((strlen(scoreText)-1)*6), 100);
                drawText("Press   to continue", 96, 150);
                renderButtonIcon(k_attack.input & -k_attack.input, 166, 148, 1);
                //printf("0x%08X",k_attack.input & -k_attack.input);
		    sf2d_end_frame();
        break;  
        case MENU_INVENTORY:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
                if(currentLevel == 0){ 
                    sf2d_draw_texture_part_scale(minimap[1],(-xscr/3)-256,(-yscr/3)-32,0,0,128,128,12.5,7.5);
                    sf2d_draw_rectangle(0,0,400,240, 0xAFDFDFDF);
                }
	            offsetX = xscr;offsetY = yscr;
		            renderMenuBackground(xscr,yscr);
	            offsetX = 0;offsetY = 0;
                renderFrame(1,1,24,14,0xFFFF1010);
                renderItemList(player.p.inv, 1,1,24,14, curInvSel);
		    sf2d_end_frame();
        break;  
        case MENU_CRAFTING:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
                if(currentLevel == 0){ 
                    sf2d_draw_texture_part_scale(minimap[1],(-xscr/3)-256,(-yscr/3)-32,0,0,128,128,12.5,7.5);
                    sf2d_draw_rectangle(0,0,400,240, 0xAFDFDFDF);
                }
	            offsetX = xscr;offsetY = yscr;
		            renderMenuBackground(xscr,yscr);
	            offsetX = 0;offsetY = 0;
	            
                renderFrame(15,1,24,4,0xFFFF1010);
                renderFrame(15,5,24,14,0xFFFF1010);
                renderFrame(1,1,14,14,0xFFFF1010);
                renderRecipes(currentRecipes, 1, 1, 14, 14, curInvSel);
                
                Recipe* rec = &currentRecipes->recipes[curInvSel];
                renderItemIcon(rec->itemResult,rec->itemAmountLevel,128,16);
                char craftText[12];
                sprintf(craftText,"%d",countItemInv(rec->itemResult,rec->itemAmountLevel, player.p.inv));
                drawText(craftText,274,34);
                
                if(rec->numOfCosts > 0){
                    int i;
                    for(i = 0; i < rec->numOfCosts; i++){
                        int amnt = countItemInv(rec->costs[i].costItem,0, player.p.inv);
                        int ttlCst = rec->costs[i].costAmount;
                        int col = 0xFFFFFFFF; if(amnt<ttlCst) col = 0xFF7F7F7F;
                        renderItemIcon(rec->costs[i].costItem,1,128,48+(i*8));
                        sprintf(craftText,"%d/%d",amnt,ttlCst);
                        drawTextColor(craftText,274,96+(i*18),col);
                    }
                }
                
		    sf2d_end_frame();
        break;  
        
        case MENU_CONTAINER:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
                if(currentLevel == 0){ 
                    sf2d_draw_texture_part_scale(minimap[1],(-xscr/3)-256,(-yscr/3)-32,0,0,128,128,12.5,7.5);
                    sf2d_draw_rectangle(0,0,400,240, 0xAFDFDFDF);
                }
	            offsetX = xscr;offsetY = yscr;
		            renderMenuBackground(xscr,yscr);
		        if (curChestEntity->entityFurniture.r == 1){ offsetX = 48;offsetY = 0;}
		        else {offsetX = 0;offsetY = 0;}
		        
		        renderFrame(1,1,14,14,0xFFFF1010);
		        renderItemList(curChestEntity->entityFurniture.inv,1,1,14,14,
                curChestEntity->entityFurniture.r == 0 ? curInvSel : -curChestEntity->entityFurniture.oSel - 1);
		        renderFrame(15,1,28,14,0xFFFF1010);
		        renderItemList(player.p.inv,15,1,28,14,
                curChestEntity->entityFurniture.r == 1 ? curInvSel : -curChestEntity->entityFurniture.oSel - 1);
		        offsetX = 0;offsetY = 0;
		    sf2d_end_frame();
        break;
        case MENU_ABOUT:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
		        drawText("About Minicraft",110,12);
		        drawSizedText("Minicraft was made by Markus Persson for the",24,36,1.0);
		        drawSizedText("22'nd ludum dare competition in december 2011.",16,48,1.0);
		        drawSizedText("it is dedicated to my father. <3",72,60,1.0);
		        drawSizedText("- Markus \"Notch\" Persson",104,76,1.0);
		        
		        drawTextColor("3DS Homebrew Edition",74,120,0xFF00FF00);
		        drawSizedTextColor("This port was made by David Benepe (Davideesk)",16,144,1.0,0xFF00FF00);
		        drawSizedTextColor("just for fun in September/October 2015.",44,156,1.0,0xFF00FF00);
		        drawSizedTextColor("TY Notch for creating a fun game to remake!",28,180,1.0,0xFF00FF00);
		    sf2d_end_frame();
		    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		        drawTextColor("Special Thanks to:",52,12,0xFF7F7FFF);
		        drawTextColor("Smea",136,60,0xFF2020FF);
		        drawSizedTextColor("for ctrulib",116,80,1.0,0xFF2020FF);
		        drawTextColor("Xerpi",130,120,0xFFFF2020);
		        drawSizedTextColor("for sf2dlib",116,140,1.0,0xFFFF2020);
                drawText("Press   to return", 58, 220);
                renderButtonIcon(k_decline.input & -k_decline.input, 128, 218, 1);
		    sf2d_end_frame();
        break;
        case MENU_SETTINGS:
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
		        drawText("Settings",(400-(8*12))/2,30);
		        for(i = 4; i >= 0; --i){
                    char* msg = setOptions[i];
                    u32 color = 0xFF7F7F7F;
                    if(i == currentSelection) color = 0xFFFFFFFF;
                    if(i == 2){
                        if(shouldRenderDebug) drawSizedTextColor("On",142, ((8 + i) * 32 - 190) >> 1,2.0, 0xFF00DF00);    
                        else  drawSizedTextColor("Off",142, ((8 + i) * 32 - 190) >> 1,2.0, 0xFF0000DF);   
                    } else if(i == 3){
                        
                        if((MODEL_3DS & 6) != 0){ // detect if user is using a New 3DS
                            if(shouldSpeedup) drawSizedTextColor("On",142, ((8 + i) * 32 - 190) >> 1,2.0, 0xFF00DF00);    
                            else  drawSizedTextColor("Off",142, ((8 + i) * 32 - 190) >> 1,2.0, 0xFF0000DF); 
                        } else {
                            color = 0xFF3F3F3F;
                            drawSizedTextColor("Off",142, ((8 + i) * 32 - 190) >> 1,2.0, 0xFF3F3F3F); 
                        }
                    }
                    drawSizedTextColor(msg,(200 - (strlen(msg) * 8))/2, ((8 + i) * 32 - 190) >> 1,2.0, color);    
                }
		    sf2d_end_frame();
		    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);	
		        switch(currentSelection){
                    case 0:
                        drawTextColor("Change the controls",(320 - (19 * 12))/2,24,0xFF7FFFFF);
                        break;
                    case 1:
                        drawTextColor("Change the game's art",(320 - (21 * 12))/2,24,0xFF7FFFFF);
                        break;
                    case 2:
                        drawTextColor("Show FPS/Pos/Entities",(320 - (22 * 12))/2,24,0xFF7FFFFF);
                        break;
                    case 3:
                        drawTextColor("Use the N3DS 804mhz mode",(320 - (24 * 12))/2,24,0xFF7FFFFF);
                        break;
                    case 4:
                        drawTextColor("Back to the titlescreen",(320 - (23 * 12))/2,24,0xFF7FFFFF);
                        break;
                }
                drawText("Press   to select", 58, 100);
                renderButtonIcon(k_accept.input & -k_accept.input, 128, 98, 1);
                drawText("Press   to return", 58, 150);
                renderButtonIcon(k_decline.input & -k_decline.input, 128, 148, 1);
		    sf2d_end_frame();
        break;
        case MENU_TITLE:
            /* Top Screen */
		    sf2d_start_frame(GFX_TOP, GFX_LEFT);
		        renderTitle(76,16);
		    
		        for(i = 4; i >= 0; --i){
                    char* msg = options[i];
                    u32 color = 0xFF7F7F7F;
                    if(i == currentSelection) color = 0xFFFFFFFF;
                    drawSizedTextColor(msg,(200 - (strlen(msg) * 8))/2, ((8 + i) * 20 - 50) >> 1,2.0, color);    
                }
                
		        drawText(versionText,2,225);
		    sf2d_end_frame();
		    
            /* Bottom Screen */
		    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);	
		      int startX = 0, startY = 0;// relative coordinates ftw
		        switch(currentSelection){
                    case 0: // "Start Game"
                        startX = 20;startY = 50;
                        render16(startX,startY+12,0,128,0);//Player(Carrying)
                        render16(startX,startY,128,128,0);//Workbench
                        startX = 120;startY = 20;
                        render16b(startX,startY,16,96,0,0xFFC11D00);// water pit
                        render16b(startX+16,startY,32,96,0,0xFFC11D00);
                        render16b(startX,startY+16,48,96,0,0xFFC11D00);
                        render16b(startX+16,startY+16,64,96,0,0xFFC11D00);
                        renderc  (startX+8,startY+12,48,160,16,8,0);//Waves
                        renderc  (startX+8,startY+8,0,112,16,8,0);//Player (Top-Half)
                        startX = 110;startY = 76;
                        render16 (startX,startY,48,112,0);//Player
                        renderc  (startX+12,startY,40,160,8,16,0);//Slash
                        render   (startX+14,startY+4,152,144,0);//Pickaxe
                        render16b(startX+18,startY,80,0,0,0xFFAEC6DC);//Iron ore
                        startX = 40;startY = 90;
                        render16b (startX,startY,128,112,0,0xFFADFFAD);//Slime
                        render16 (startX+18,startY,48,112,1);//Player (Mirrored)
                        renderc  (startX+14,startY,32,160,8,16,0);//Slash
                        render   (startX+12,startY+4,104,144,1);//Sword
                        startX = 64;startY = 40;
                        render16b(startX,startY,16,80,0,0xFF69B569);// grass pit
                        render16b(startX+16,startY,32,80,0,0xFF69B569);
                        render16b(startX,startY+16,48,80,0,0xFF69B569);
                        render16b(startX+16,startY+16,64,80,0,0xFF69B569);
                        render16 (startX+8,startY+4,0,16,0);//Tree
                        render   (startX+1,startY+14,80,152,0);// Apple
                        render16 (startX+9,startY+18,16,112,0);//Player
                        renderc  (startX+9,startY+14,16,160,16,8,0);//Slash
                        drawTextColor("Play minicraft",24,24,0xFF7FFFFF);
                        break;
                    case 1: // "How To Play"
                        startX = 72;startY = 54;
                        render16(startX,startY,96,208,0);//C-PAD
                        startX = 72;startY = 37;
                        render16(startX,startY-16,16,112,0);//Player
                        render16(startX,startY,112,208,0);//C-PAD up
                        startX = 72;startY = 71;
                        render16(startX,startY+16,0,112,0);//Player
                        render16(startX,startY,144,208,0);//C-PAD down
                        startX = 39;startY = 54;
                        render16(startX,startY,48,112,1);//Player
                        render16(startX+16,startY,128,208,0);//C-PAD left
                        startX = 89;startY = 54;
                        render16(startX+16,startY,48,112,0);//Player
                        render16(startX,startY,160,208,0);//C-PAD right
                        
                        drawTextColor("Learn the basics",64,24,0xFF7FFFFF);
                        break;
                    case 2: // "Settings"
                        drawTextColor("Modify the game's feel",(320 - (22 * 12))/2,24,0xFF7FFFFF);
                        renderc(48,48,0,112,64,32,0);
                        break;
                    case 3: // "About"
                        drawTextColor("Who made this game?",(320 - (19 * 12))/2,24,0xFF7FFFFF);
                        break;
                    case 4: // "Exit"
                        drawTextColor("Exit to the homebrew menu",(320 - (25 * 12))/2,24,0xFF7FFFFF);
                        drawTextColor("(bye-bye)",(320 - (9 * 12))/2,100,0xFF7FFFFF);
                        break;
                }
		    sf2d_end_frame();
            break;
    }

}
