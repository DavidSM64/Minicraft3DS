#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>
#include <string.h> 
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "Globals.h"
#include "Render.h"
#include "MapGen.h"
#include "Menu.h"
#include "texturepack.h"

void initMiniMap(bool loadUpWorld) {
	int i, x, y;
	for (i = 0; i < 5; ++i) {
		for (x = 0; x < 128; ++x) {
			for (y = 0; y < 128; ++y) {

				if (!loadUpWorld) { // generate stairs up when making a new world.
					switch (map[i][x + y * 128]) {
					case TILE_STAIRS_DOWN:
						map[i + 1][x + y * 128] = TILE_STAIRS_UP;
						if (i == 0) {
							map[i + 1][(x + 1) + y * 128] = TILE_HARDROCK;
							map[i + 1][x + (y + 1) * 128] = TILE_HARDROCK;
							map[i + 1][(x - 1) + y * 128] = TILE_HARDROCK;
							map[i + 1][x + (y - 1) * 128] = TILE_HARDROCK;
							map[i + 1][(x + 1) + (y + 1) * 128] = TILE_HARDROCK;
							map[i + 1][(x - 1) + (y - 1) * 128] = TILE_HARDROCK;
							map[i + 1][(x - 1) + (y + 1) * 128] = TILE_HARDROCK;
							map[i + 1][(x + 1) + (y - 1) * 128] = TILE_HARDROCK;
						} else {
							map[i + 1][(x + 1) + y * 128] = TILE_DIRT;
							map[i + 1][x + (y + 1) * 128] = TILE_DIRT;
							map[i + 1][(x - 1) + y * 128] = TILE_DIRT;
							map[i + 1][x + (y - 1) * 128] = TILE_DIRT;
							map[i + 1][(x + 1) + (y + 1) * 128] = TILE_DIRT;
							map[i + 1][(x - 1) + (y - 1) * 128] = TILE_DIRT;
							map[i + 1][(x - 1) + (y + 1) * 128] = TILE_DIRT;
							map[i + 1][(x + 1) + (y - 1) * 128] = TILE_DIRT;
						}
					}
				}

				/* Minimaps */
				switch (map[i][x + y * 128]) {
				case TILE_WATER:
					sf2d_set_pixel(minimap[i], x, y, 0xFFFF0000);
					break;
				case TILE_LAVA:
					sf2d_set_pixel(minimap[i], x, y, 0xFF0000FF);
					break;
				case TILE_DIRT:
					sf2d_set_pixel(minimap[i], x, y, 0xFF6C6D82);
					break;
				case TILE_ROCK:
					sf2d_set_pixel(minimap[i], x, y, 0xFF7F7F7F);
					break;
				case TILE_HARDROCK:
					sf2d_set_pixel(minimap[i], x, y, 0xFF7F5F5F);
					break;
				case TILE_GRASS:
					sf2d_set_pixel(minimap[i], x, y, 0xFF00FF00);
					break;
				case TILE_TREE:
					sf2d_set_pixel(minimap[i], x, y, 0xFF007F00);
					break;
				case TILE_SAND:
					sf2d_set_pixel(minimap[i], x, y, 0xFF00FFFF);
					break;
				case TILE_CACTUS:
					sf2d_set_pixel(minimap[i], x, y, 0xFF009F00);
					break;
				case TILE_FLOWER:
					sf2d_set_pixel(minimap[i], x, y, 0xFF00FF3F);
					break;
				case TILE_IRONORE:
					sf2d_set_pixel(minimap[i], x, y, 0xFF9696DC);
					break;
				case TILE_GOLDORE:
					sf2d_set_pixel(minimap[i], x, y, 0xFF9AE8E5);
					break;
				case TILE_GEMORE:
					sf2d_set_pixel(minimap[i], x, y, 0xFFDE98DF);
					break;
				case TILE_CLOUD:
					sf2d_set_pixel(minimap[i], x, y, 0xFFFFFFFF);
					break;
				case TILE_CLOUDCACTUS:
					sf2d_set_pixel(minimap[i], x, y, 0xFFAFAFAF);
					break;
				case TILE_STAIRS_DOWN:
					sf2d_set_pixel(minimap[i], x, y, 0xFF9F9F9F);
					break;
				case TILE_STAIRS_UP:
					sf2d_set_pixel(minimap[i], x, y, 0xFF9F9F9F);
					break;
				case 255:
					sf2d_set_pixel(minimap[i], x, y, 0xFF007F00);
					break;
				default:
					sf2d_set_pixel(minimap[i], x, y, 0xFF111111);
					break;
				}
			}
		}
	}
}

void initNewMap() {
	newSeed();
	createAndValidateSkyMap(128, 128, map[0], data[0]);
	createAndValidateTopMap(128, 128, map[1], data[1]);
	createAndValidateUndergroundMap(128, 128, 1, map[2], data[2]);
	createAndValidateUndergroundMap(128, 128, 2, map[3], data[3]);
	createAndValidateUndergroundMap(128, 128, 3, map[4], data[4]);
}

void setupGame(bool loadUpWorld) {
	currentLevel = 1;

	// Reset entity manager.
	memset(&eManager, 0, sizeof(eManager));
	sf2d_set_clear_color(RGBA8(0x82, 0x6D, 0x6C, 0xFF));

	if (!loadUpWorld) {
		initNewMap();
		initPlayer();
		airWizardHealthDisplay = 2000;
		int i;
		for (i = 0; i < 5; ++i) {
			trySpawn(500, i);
		}
		addEntityToList(newAirWizardEntity(630, 820, 0), &eManager);
	} else {
		initPlayer();
		loadWorld(currentFileName, &eManager, &player, (u8*) map, (u8*) data);
	}

	initMiniMap(loadUpWorld);
	shouldRenderMap = false;
	mScrollX = 0;
	mScrollY = 0;
	zoomLevel = 2;
    sprintf(mapText,"x%d",zoomLevel);
	initGame = 0;
}

int xscr = 0, yscr = 0;
void tick() {
	if (player.p.isDead) {
		if (player.p.endTimer < 1) {
			currentMenu = MENU_LOSE;
		}
		--player.p.endTimer;
		return;
	} else if (player.p.hasWon) {
		if (player.p.endTimer < 1) {
			currentMenu = MENU_WIN;
		}
		--player.p.endTimer;
		return;
	}
	
	tickTouchMap();

	int i;
	for (i = 0; i < 324; ++i) {
		int xx = rand() & 127;
		int yy = rand() & 127;
		tickTile(xx, yy);
	}
	tickPlayer();
	xscr = player.x - 100;
	yscr = player.y - 56;
	if (xscr < 16)
		xscr = 16;
	else if (xscr > 1832)
		xscr = 1832;
	if (yscr < 16)
		yscr = 16;
	else if (yscr > 1912)
		yscr = 1912;

	for (i = 0; i < eManager.lastSlot[currentLevel]; ++i) {
		Entity * e = &eManager.entities[currentLevel][i];
		if ((e->type != ENTITY_ZOMBIE && e->type != ENTITY_SLIME)
				|| (e->x > player.x - 160 && e->y > player.y - 125
						&& e->x < player.x + 160 && e->y < player.y + 125))
			tickEntity(e);
	}

}

void clearScreen(int* data, u8 fill, int size) {
	int i;
	for (i = 0; i < size / 4; ++i)
		data[i] = 0x000000FF;
}

char debugText[34];
char bossHealthText[34];
int main() {
    initCfgu();
    CFGU_GetSystemModel(&MODEL_3DS);
	FILE * file;
	shouldRenderDebug = true;
	if ((file = fopen("settings.bin", "r"))) {
        fread(&shouldRenderDebug,sizeof(bool),1,file);
        fread(&shouldSpeedup,sizeof(bool),1,file);
        osSetSpeedupEnable(shouldSpeedup);
		fclose(file);
	}
    
	sf2d_init();
	csndInit();
	noItem = newItem(ITEM_NULL, 0);
	
	currentMenu = MENU_TITLE;
	currentSelection = 0;
	quitGame = false;

	icons = sfil_load_PNG_buffer(icons2_png, SF2D_PLACE_RAM);
	font = sfil_load_PNG_buffer(Font_png, SF2D_PLACE_RAM);
	bottombg = sfil_load_PNG_buffer(bottombg_png, SF2D_PLACE_RAM);

	loadSound(&snd_playerHurt, "resources/playerhurt.raw");
	loadSound(&snd_playerDeath, "resources/playerdeath.raw");
	loadSound(&snd_monsterHurt, "resources/monsterhurt.raw");
	loadSound(&snd_test, "resources/test.raw");
	loadSound(&snd_pickup, "resources/pickup.raw");
	loadSound(&snd_bossdeath, "resources/bossdeath.raw");
	loadSound(&snd_craft, "resources/craft.raw");

	bakeLights();
	

	int i;
	for (i = 0; i < 5; ++i) {
		minimap[i] = sf2d_create_texture(128, 128, TEXFMT_RGBA8,
				SF2D_PLACE_RAM);
		sf2d_texture_tile32(minimap[i]);
	}
	
	dirtColor[0] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 0)); 
	dirtColor[1] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 1)); 
	dirtColor[2] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 2)); 
	dirtColor[3] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 3)); 
	dirtColor[4] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 4)); 


	sf2d_set_vblank_wait(true);

	sf2d_set_clear_color(0xFF);

	/* Default inputs */
	k_up.input = KEY_DUP | KEY_CPAD_UP | KEY_CSTICK_UP;
	k_down.input = KEY_DDOWN | KEY_CPAD_DOWN | KEY_CSTICK_DOWN;
	k_left.input = KEY_DLEFT | KEY_CPAD_LEFT | KEY_CSTICK_LEFT;
	k_right.input = KEY_DRIGHT | KEY_CPAD_RIGHT | KEY_CSTICK_RIGHT;
	k_attack.input = KEY_A | KEY_B | KEY_L | KEY_ZR;
	k_menu.input = KEY_X | KEY_Y | KEY_R | KEY_ZL;
	k_pause.input = KEY_START;
	k_accept.input = KEY_A;
	k_decline.input = KEY_B;
	k_delete.input = KEY_X;
	k_menuNext.input = KEY_R;
	k_menuPrev.input = KEY_L;

	/* If btnSave exists, then use that. */
	if ((file = fopen("btnSave.bin", "rb"))) {
		fread(&k_up.input, sizeof(int), 1, file);
		fread(&k_down.input, sizeof(int), 1, file);
		fread(&k_left.input, sizeof(int), 1, file);
		fread(&k_right.input, sizeof(int), 1, file);
		fread(&k_attack.input, sizeof(int), 1, file);
		fread(&k_menu.input, sizeof(int), 1, file);
		fread(&k_pause.input, sizeof(int), 1, file);
		fread(&k_accept.input, sizeof(int), 1, file);
		fread(&k_decline.input, sizeof(int), 1, file);
		fread(&k_delete.input, sizeof(int), 1, file);
		fread(&k_menuNext.input, sizeof(int), 1, file);
		fread(&k_menuPrev.input, sizeof(int), 1, file);
		fclose(file);
	}
	
	/* If lastTP exists, then use that. */
	if ((file = fopen("lastTP.bin", "r"))) {
		char fnbuf[256];
		fgets(fnbuf, 256, file); // get directory to texturepack
		loadTexturePack(fnbuf);   
		fclose(file);
	}

	tickCount = 0;
	initRecipes();
	defineTables();
	while (aptMainLoop()) {
		++tickCount;
		hidScanInput();
		tickKeys(hidKeysHeld(), hidKeysDown());

		if (quitGame) break;

		if (initGame > 0) setupGame(initGame == 1 ? true : false);

		if (currentMenu == 0) {
			tick();
			sf2d_start_frame(GFX_TOP, GFX_LEFT);

			offsetX = xscr;
			offsetY = yscr;
			sf2d_draw_rectangle(0, 0, 400, 240, RGBA8(12, 12, 12, 255)); //You might think "real" black would be better, but it actually looks better that way
			renderLightsToStencil();

			renderBackground(xscr, yscr);
			renderEntities(player.x, player.y, &eManager);
			renderPlayer();
			
			resetStencilStuff();
			offsetX = 0;
			offsetY = 0;
			
			if(shouldRenderDebug){
			    sprintf(fpsstr, " FPS: %.0f, X:%d, Y:%d, E:%d", sf2d_get_fps(), player.x, player.y, eManager.lastSlot[currentLevel]);
			    drawText(fpsstr, 2, 225);
            }
			
			sf2d_end_frame();

            sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
                if(!shouldRenderMap){
                    sf2d_draw_texture(bottombg, 0, 0);
                    renderGui();
                } else {
                    renderZoomedMap();
                }
            sf2d_end_frame();
		} else {
			tickMenu(currentMenu);
			renderMenu(currentMenu, xscr, yscr);
		}

		sf2d_swapbuffers();
	}

	freeRecipes();

	freeLightBakes();
	sf2d_free_texture(icons);
	sf2d_free_texture(minimap[0]);
	sf2d_free_texture(minimap[1]);
	sf2d_free_texture(minimap[2]);
	sf2d_free_texture(minimap[3]);
	sf2d_free_texture(minimap[4]);
	freeSounds();
	csndExit();
    exitCfgu();
	sf2d_fini();
	return 0;
}
