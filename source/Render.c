#include "Render.h"

void render(s32 xp, s32 yp, u32 xTile, u32 yTile, u8 bits) {
	xp -= offsetX;
	yp -= offsetY;
	int scaleX = 2, scaleY = 2;
	if ((bits & 1) > 0) {
		scaleX = -2;
		xp += 8;
	}
	if ((bits & 2) > 0) {
		scaleY = -2;
		yp += 8;
	}
	sf2d_draw_texture_part_scale(icons, xp << 1, yp << 1, xTile, yTile, 8, 8,
			scaleX, scaleY);
}
void renderb(s32 xp, s32 yp, u32 xTile, u32 yTile, u8 bits, u32 color) {
	xp -= offsetX;
	yp -= offsetY;
	int scaleX = 2, scaleY = 2;
	if ((bits & 1) > 0) {
		scaleX = -2;
		xp += 8;
	}
	if ((bits & 2) > 0) {
		scaleY = -2;
		yp += 8;
	}
	sf2d_draw_texture_part_scale_blend(icons, xp << 1, yp << 1, xTile, yTile, 8,
			8, scaleX, scaleY, color);
}
void renderr(s32 xp, s32 yp, u32 xTile, u32 yTile, u8 bits, float rotate) {
	xp -= offsetX;
	yp -= offsetY;
	int scaleX = 2, scaleY = 2;
	if ((bits & 1) > 0) {
		scaleX = -2;
		xp += 8;
	}
	if ((bits & 2) > 0) {
		scaleY = -2;
		yp += 8;
	}
	sf2d_draw_texture_part_rotate_scale(icons, xp << 1, yp << 1, rotate, xTile,
			yTile, 8, 8, scaleX, scaleY);
}
void renderc(s32 xp, s32 yp, u32 xTile, u32 yTile, int sizeX, int sizeY,
		u8 bits) {
	xp -= offsetX;
	yp -= offsetY;
	int scaleX = 2, scaleY = 2;
	if ((bits & 1) > 0) {
		scaleX = -2;
		xp += sizeX;
	}
	if ((bits & 2) > 0) {
		scaleY = -2;
		yp += sizeY;
	}
	sf2d_draw_texture_part_scale(icons, xp << 1, yp << 1, xTile, yTile, sizeX,
			sizeY, scaleX, scaleY);
}
void renderbc(s32 xp, s32 yp, u32 xTile, u32 yTile, int sizeX, int sizeY,
		u8 bits, u32 color) {
	xp -= offsetX;
	yp -= offsetY;
	int scaleX = 2, scaleY = 2;
	if ((bits & 1) > 0) {
		scaleX = -2;
		xp += sizeX;
	}
	if ((bits & 2) > 0) {
		scaleY = -2;
		yp += sizeY;
	}
	sf2d_draw_texture_part_scale_blend(icons, xp << 1, yp << 1, xTile, yTile,
			sizeX, sizeY, scaleX, scaleY, color);
}
void render16(s32 xp, s32 yp, u32 xTile, u32 yTile, u8 bits) {
	xp -= offsetX;
	yp -= offsetY;
	int scaleX = 2, scaleY = 2;
	if ((bits & 1) > 0) {
		scaleX = -2;
		xp += 16;
	}
	if ((bits & 2) > 0) {
		scaleY = -2;
		yp += 16;
	}
	sf2d_draw_texture_part_scale(icons, xp << 1, yp << 1, xTile, yTile, 16, 16,
			scaleX, scaleY);
}

void render16c(s32 xp, s32 yp, u32 xTile, u32 yTile, u8 bits, float scaleX,float scaleY) {
	xp -= offsetX;
	yp -= offsetY;
	xp *= scaleX;
	yp *= scaleY;
	if ((bits & 1) > 0) {
		xp += 16 * scaleX;
		scaleX = -scaleX;
	}
	if ((bits & 2) > 0) {
		yp += 16 * scaleY;
		scaleY = -scaleY;
	}
	sf2d_draw_texture_part_scale(icons, xp, yp, xTile, yTile,16, 16, scaleX, scaleY);
}

void render16b(s32 xp, s32 yp, u32 xTile, u32 yTile, u8 bits, u32 color) {
	xp -= offsetX;
	yp -= offsetY;
	int scaleX = 2, scaleY = 2;
	if ((bits & 1) > 0) {
		scaleX = -2;
		xp += 16;
	}
	if ((bits & 2) > 0) {
		scaleY = -2;
		yp += 16;
	}
	sf2d_draw_texture_part_scale_blend(icons, xp << 1, yp << 1, xTile, yTile,
			16, 16, scaleX, scaleY, color);
}
void render16s(s32 xp, s32 yp, u32 tile, u8 bits, u32 color) {
	xp -= offsetX;
	yp -= offsetY;
	int xTile = tile & 255;
	int yTile = tile >> 8;
	int scaleX = 2, scaleY = 2;
	if ((bits & 1) > 0) {
		scaleX = -2;
		xp += 16;
	}
	if ((bits & 2) > 0) {
		scaleY = -2;
		yp += 16;
	}
	sf2d_draw_texture_part_scale_blend(icons, xp << 1, yp << 1, xTile, yTile,
			16, 16, scaleX, scaleY, color);
}

void renderTitle(int x, int y) {
	sf2d_draw_texture_part_scale(icons, (x - 26) << 1, y << 1, 0, 240, 104, 16,
			2.0, 2.0); // MINICRAFT
	sf2d_draw_texture_part(icons, x + 48, y + 52, 104, 240, 152, 16); // 3DS HOMEBREW EDITION
}

void renderButtonIcon(u32 keyIcon, int x, int y, float scale) {
	switch (keyIcon) {
	case CIRCLEPAD:
		render16c(x, y, 96, 208, 0, scale, scale);
		break;
	case KEY_CPAD_UP:
		render16c(x, y, 112, 208, 0, scale, scale);
		break;
	case KEY_CPAD_LEFT:
		render16c(x, y, 128, 208, 0, scale, scale);
		break;
	case KEY_CPAD_DOWN:
		render16c(x, y, 144, 208, 0, scale, scale);
		break;
	case KEY_CPAD_RIGHT:
		render16c(x, y, 160, 208, 0, scale, scale);
		break;

		/* New 3DS only */
	case CSTICK:
		render16c(x, y, 176, 208, 0, scale, scale);
		break;
	case KEY_CSTICK_UP:
		render16c(x, y, 192, 208, 0, scale, scale);
		break;
	case KEY_CSTICK_LEFT:
		render16c(x, y, 208, 208, 0, scale, scale);
		break;
	case KEY_CSTICK_DOWN:
		render16c(x, y, 224, 208, 0, scale, scale);
		break;
	case KEY_CSTICK_RIGHT:
		render16c(x, y, 240, 208, 0, scale, scale);
		break;

	case KEY_A:
		render16c(x, y, 0, 224, 0, scale, scale);
		break;
	case KEY_B:
		render16c(x, y, 16, 224, 0, scale, scale);
		break;
	case KEY_X:
		render16c(x, y, 32, 224, 0, scale, scale);
		break;
	case KEY_Y:
		render16c(x, y, 48, 224, 0, scale, scale);
		break;
	case KEY_DUP:
		render16c(x, y, 64, 224, 0, scale, scale);
		break;
	case KEY_DLEFT:
		render16c(x, y, 80, 224, 0, scale, scale);
		break;
	case KEY_DDOWN:
		render16c(x, y, 96, 224, 0, scale, scale);
		break;
	case KEY_DRIGHT:
		render16c(x, y, 112, 224, 0, scale, scale);
		break;
	case KEY_START:
		render16c(x - 8, y, 128, 224, 0, scale, scale);
		render16c(x + 8, y, 144, 224, 0, scale, scale);
		break;
	case KEY_SELECT:
		render16c(x - 8, y, 160, 224, 0, scale, scale);
		render16c(x + 8, y, 176, 224, 0, scale, scale);
		break;
	case KEY_L:
		render16c(x, y, 192, 224, 0, scale, scale);
		break;
	case KEY_R:
		render16c(x, y, 208, 224, 0, scale, scale);
		break;

		/* New 3DS only */
	case KEY_ZL:
		render16c(x, y, 224, 224, 0, scale, scale);
		break;
	case KEY_ZR:
		render16c(x, y, 240, 224, 0, scale, scale);
		break;
	}
}

int getFrame(int a, int b, int s) {
	return (a == s) ? 0 : ((a < b - 1) ? 8 : 16);
}

void renderFrame(int x1, int y1, int x2, int y2, u32 bgColor) {
	int startX = x1;
	int startY = y1;
	sf2d_draw_rectangle((x1 << 4) + 4 - (offsetX << 1),
			(y1 << 4) + 4 - (offsetY << 1), ((x2 - x1) << 4) - 8,
			((y2 - y1) << 4) - 8, bgColor);
	while (x1 < x2) {
		y1 = startY;
		while (y1 < y2) {
			int xp = (x1 << 4) - (offsetX << 1);
			int yp = (y1 << 4) - (offsetY << 1);
			sf2d_draw_texture_part_scale(icons, xp, yp,
					getFrame(x1, x2, startX), 200 + getFrame(y1, y2, startY), 8,
					8, 2.0, 2.0);
			++y1;
		}
		++x1;
	}
}

void renderDotsWithColor(int val, int x, int y, u8 bits, u32 color) {
	switch (val) {
	case 3:
		renderb(x, y, 0, 0, bits, color);
		return;
	case 5:
		renderb(x + 8, y, 8, 0, bits, color);
		return;
	case 7:
		renderbc(x, y, 0, 0, 16, 8, bits, color);
		return;
	case 10:
		renderb(x, y + 8, 0, 8, bits, color);
		return;
	case 11:
		renderbc(x, y, 0, 0, 8, 16, bits, color);
		return;
	case 12:
		renderb(x + 8, y + 8, 8, 8, bits, color);
		return;
	case 13:
		renderbc(x + 8, y, 8, 0, 8, 16, bits, color);
		return;
	case 14:
		renderbc(x, y + 8, 0, 8, 16, 8, bits, color);
		return;
	case 15:
		render16b(x, y, 0, 0, bits, color);
		return;
	}
}

void renderRockDotsWithColor(int val, int x, int y, u32 color) {
	switch (val) {
	case 208:
		render16b(x, y, 0, 0, 0, color);
		return;
	case 16:
		renderb(x + 8, y + 8, 8, 8, 0, color);
		return;
	case 32:
		renderb(x, y + 8, 0, 8, 0, color);
		return;
	case 48:
		renderb(x, y + 8, 0, 8, 0, color);
		return;
	case 64:
		renderb(x, y, 0, 0, 0, color);
		return;
	case 80:
		renderbc(x, y, 0, 0, 8, 16, 0, color);
		return;
	case 96:
		renderbc(x + 8, y, 8, 0, 8, 16, 0, color);
		return;
	case 112:
		renderbc(x, y + 8, 0, 8, 16, 8, 0, color);
		return;
	case 128:
		renderbc(x, y, 0, 0, 16, 8, 0, color);
		return;
	case 144:
		renderb(x, y + 8, 0, 8, 0, color);
		renderbc(x + 8, y, 8, 0, 8, 16, 0, color);
		return;
	case 160:
		renderb(x, y, 0, 0, 0, color);
		renderbc(x + 8, y, 8, 0, 8, 16, 0, color);
		return;
	case 176:
		renderb(x + 8, y, 8, 0, 0, color);
		renderbc(x, y, 0, 0, 8, 16, 0, color);
		return;
	case 192:
		renderb(x + 8, y + 8, 8, 8, 0, color);
		renderbc(x, y, 0, 0, 8, 16, 0, color);
		return;
	case 4112:
		renderbc(x, y, 0, 0, 8, 16, 0, color);
		return;
	case 4128:
		renderbc(x, y + 8, 0, 8, 8, 16, 0, color);
		return;
	case 4192:
		renderb(x, y, 0, 0, 0, color);
		return;
	case 8192:
		renderb(x, y + 8, 0, 8, 0, color);
		return;
	case 8208:
		renderb(x + 8, y + 8, 8, 8, 0, color);
		return;
	case 8224:
		renderb(x + 8, y + 8, 8, 8, 0, color);
		return;
	case 8240:
		renderb(x + 8, y, 8, 0, 0, color);
		return;
	case 8256:
		renderb(x, y + 8, 0, 8, 0, color);
		return;
	case 8272:
		renderb(x + 8, y, 8, 0, 0, color);
		return;
	case 8288:
		renderb(x, y, 0, 0, 0, color);
		return;
	case 8304:
		renderb(x + 8, y, 8, 0, 0, color);
		renderb(x, y + 8, 0, 8, 0, color);
		return;
	case 8320:
		renderbc(x + 8, y, 8, 0, 8, 16, 0, color);
		return;
	case 8336:
		renderbc(x, y, 0, 0, 8, 16, 0, color);
		return;
	case 8352:
		renderb(x, y, 0, 0, 0, color);
		renderb(x + 8, y + 8, 8, 8, 0, color);
		return;
	case 8368:
		renderb(x + 8, y, 8, 0, 0, color);
		return;
	case 8384:
		renderb(x, y + 8, 0, 8, 0, color);
		return;
	case 8400:
		renderb(x, y, 0, 0, 0, color);
		return;
	case 8416:
		renderb(x + 8, y + 8, 8, 8, 0, color);
		return;
	}
}

void bakeLights() {
	playerLightBake = sf2d_create_texture(64, 64, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	lanternLightBake = sf2d_create_texture(128, 128, TEXFMT_RGBA8,
			SF2D_PLACE_RAM);

	bakeLight(playerLightBake, 32, 32, 32);
	bakeLight(lanternLightBake, 64, 64, 64);
}

void freeLightBakes() {
	sf2d_free_texture(playerLightBake);
	sf2d_free_texture(lanternLightBake);
}

void renderLightsToStencil() {
	if (currentLevel > 1) {
		GPU_SetDepthTestAndWriteMask(true, GPU_NEVER, 0);
		GPU_SetStencilTest(true, GPU_NEVER, 1, 0xFF, 0xFF);
		GPU_SetStencilOp(GPU_STENCIL_REPLACE, GPU_STENCIL_KEEP,
				GPU_STENCIL_KEEP);
		GPU_SetAlphaTest(true, GPU_GREATER, 0);

        if(player.p.activeItem->id == ITEM_LANTERN) renderLight(player.x, player.y, lanternLightBake);
        else renderLight(player.x, player.y, playerLightBake);
        
		int i;
		for (i = 0; i < eManager.lastSlot[currentLevel]; ++i) {
			Entity e = eManager.entities[currentLevel][i];
			if (e.type != ENTITY_FURNITURE)continue;
			if (e.entityFurniture.itemID == ITEM_LANTERN && e.x > player.x - 160 
            && e.y > player.y - 125 && e.x < player.x + 160 && e.y < player.y + 125)
			renderLight(e.x, e.y, lanternLightBake);
		}
		
		int xo = offsetX >> 4;
		int yo = offsetY >> 4;
		int x, y;
		for (x = xo; x <= 13 + xo; ++x) {
			for (y = yo; y <= 8 + yo; ++y)
			    if(getTile(x, y) == TILE_LAVA) renderLight(x << 4, y << 4, playerLightBake);
		}
		
		
		GPU_SetDepthTestAndWriteMask(true, GPU_GEQUAL, GPU_WRITE_ALL);
		GPU_SetStencilTest(true, GPU_EQUAL, 1, 0xFF, 0x0);
		GPU_SetAlphaTest(false, GPU_ALWAYS, 0x00);
		GPU_SetStencilOp(GPU_STENCIL_KEEP, GPU_STENCIL_KEEP,
				GPU_STENCIL_REPLACE);
	}
}

void resetStencilStuff() {
	if (currentLevel > 1) {
		GPU_SetStencilTest(false, GPU_ALWAYS, 0x00, 0xFF, 0x00);
		GPU_SetStencilOp(GPU_STENCIL_KEEP, GPU_STENCIL_KEEP, GPU_STENCIL_KEEP);
	}
}

void renderLight(int x, int y, sf2d_texture* texture) {
	sf2d_draw_texture_scale(texture, (x - (texture->width / 2) - offsetX) * 2,
			(y - (texture->height / 2) - offsetY) * 2, 2.f, 2.f);
}

/* The original software rendered version, it's bad for the framerate :( */
void bakeLight(sf2d_texture* texture, int x, int y, int r) {
	int x0 = x - r;
	int x1 = x + r;
	int y0 = y - r;
	int y1 = y + r;

	// The game's resolution is actually 200x120, the textures are all scaled up by 2.
	if (x0 < 0)
		x0 = 0;
	if (y0 < 0)
		y0 = 0;
	if (x1 > texture->width)
		x1 = texture->width;
	if (y1 > texture->height)
		y1 = texture->height;

	int xx, yy;
	for (yy = y0; yy < y1; yy++) {
		int yd = yy - y;
		yd = yd * yd;
		for (xx = x0; xx < x1; xx++) {
			int xd = xx - x;
			int dist = xd * xd + yd;
			if (dist <= r * r)
				if ((dist >= (r - (r / 6)) * (r - (r / 5))) ? rand() % 4 != 0 : true)
					sf2d_set_pixel(texture, xx, yy, RGBA8(0, 0, 0, 255)); // set transparent pixel
		}
	}

	sf2d_texture_tile32(texture);
}

u8 checkSurrTiles8(int xt, int yt, int id) {
	u8 vt = 0;
	if (getTile(xt, yt - 1) == id)
		vt |= 1;
	if (getTile(xt - 1, yt) == id)
		vt |= 2;
	if (getTile(xt + 1, yt) == id)
		vt |= 4;
	if (getTile(xt, yt + 1) == id)
		vt |= 8;
	if (getTile(xt - 1, yt - 1) == id)
		vt |= 16;
	if (getTile(xt + 1, yt - 1) == id)
		vt |= 32;
	if (getTile(xt - 1, yt + 1) == id)
		vt |= 64;
	if (getTile(xt + 1, yt + 1) == id)
		vt |= 128;
	return vt;
}
u8 checkSurrTiles4(int xt, int yt, int id) {
	u8 vt = 0;
	if (getTile(xt, yt - 1) == id)
		vt |= 1;
	if (getTile(xt - 1, yt) == id)
		vt |= 2;
	if (getTile(xt + 1, yt) == id)
		vt |= 4;
	if (getTile(xt, yt + 1) == id)
		vt |= 8;
	return vt;
}

u8 v = 0;
u8 tData = 0;
void renderTile(int i, int x, int y) {
	int age = 0;
	switch (i) {
	case TILE_GRASS:
		v = checkSurrTiles4(x >> 4, y >> 4, TILE_GRASS)
				| checkSurrTiles4(x >> 4, y >> 4, TILE_TREE)
				| checkSurrTiles4(x >> 4, y >> 4, TILE_FLOWER)
				| checkSurrTiles4(x >> 4, y >> 4, TILE_SAPLING_TREE);
		render16b(x, y, grassTable[v], 80, 0, 0x69B569FF);
		renderDotsWithColor(v, x, y, 0, 0x8ED38EFF);
		break;
	case TILE_TREE:
		render16(x, y, treeTable[checkSurrTiles8(x >> 4, y >> 4, TILE_TREE)],
				16, 0);
		break;
	case TILE_ROCK:
		v = checkSurrTiles8(x >> 4, y >> 4, TILE_ROCK);
		render16s(x, y, rockTable[v] + 8192, 0, 0xFFFFFFFF);
		renderRockDotsWithColor(rockTable[v], x, y, 0x949494FF);
		break;
	case TILE_HARDROCK:
		v = checkSurrTiles8(x >> 4, y >> 4, TILE_HARDROCK);
		render16s(x, y, rockTable[v] + 8192, 0, 0xCFCFFFFF);
		renderRockDotsWithColor(rockTable[v], x, y, 0x9494FFFF);
		break;
	case TILE_DIRT: // render dots.
		if (currentLevel > 1)
			render16b(x, y, 0, 0, 0, 0x383838FF);
		else
			render16b(x, y, 0, 0, 0, 0xA88F8FFF);
		break;
	case TILE_SAND:
		v = checkSurrTiles4(x >> 4, y >> 4, TILE_SAND)
				| checkSurrTiles4(x >> 4, y >> 4, TILE_CACTUS)
				| checkSurrTiles4(x >> 4, y >> 4, TILE_SAPLING_CACTUS);
		render16b(x, y, grassTable[v], 80, 0, 0xF7F77BFF);
		renderDotsWithColor(v, x, y, 0, 0xB7B75BFF);
		break;
	case TILE_WATER:
		v = checkSurrTiles4(x >> 4, y >> 4, TILE_WATER)
				| checkSurrTiles4(x >> 4, y >> 4, TILE_HOLE);
		render16b(x, y, grassTable[v], 96, 0, 0x001DC1FF);
		srand((tickCount + (x / 2 - y) * 4311) / 10);
		renderDotsWithColor(v, x, y, rand() & 3, 0x6B6BFFFF);
		break;
	case TILE_LAVA:
		v = checkSurrTiles4(x >> 4, y >> 4, TILE_LAVA)
				| checkSurrTiles4(x >> 4, y >> 4, TILE_HOLE);
		render16b(x, y, grassTable[v], 96, 0, 0xC11D00FF);
		srand((tickCount + (x / 2 - y) * 4311) / 10);
		renderDotsWithColor(v, x, y, rand() & 3, 0xFF6B6BFF);
		break;
	case TILE_HOLE:
		render16b(x, y,
				grassTable[checkSurrTiles4(x >> 4, y >> 4, TILE_HOLE)
						| checkSurrTiles4(x >> 4, y >> 4, TILE_WATER)
						| checkSurrTiles4(x >> 4, y >> 4, TILE_LAVA)], 96, 0,
				0x383838FF);
		break;
	case TILE_CACTUS:
		render16(x, y, 48, 0, 0);
		break;
	case TILE_FLOWER:
		render16(x, y, 64, 0, getData(x >> 4, y >> 4));
		break;
	case TILE_STAIRS_DOWN:
		if (currentLevel == 0)
			renderTile(TILE_CLOUD, x, y);
		render16(x, y, 96, 0, 0);
		break;
	case TILE_STAIRS_UP:
		render16(x, y, 112, 0, 0);
		break;
	case TILE_IRONORE:
		render16b(x, y, 80, 0, 0, 0xDFC8C8FF);
		break;
	case TILE_GOLDORE:
		render16b(x, y, 80, 0, 0, 0xE5E8B9FF);
		break;
	case TILE_GEMORE:
		render16b(x, y, 80, 0, 0, 0xDF98DEFF);
		break;
	case TILE_CLOUD:
		render16b(x, y,
				grassTable[checkSurrTiles4(x >> 4, y >> 4, TILE_CLOUD)
						| checkSurrTiles4(x >> 4, y >> 4, TILE_STAIRS_DOWN)
						| checkSurrTiles4(x >> 4, y >> 4, TILE_CLOUDCACTUS)],
				80, 0, 0xFFFFFFFF);
		break;
	case TILE_CLOUDCACTUS:
		renderTile(TILE_CLOUD, x, y);
		render16(x, y, 80, 0, 0);
		break;
	case TILE_SAPLING_TREE:
		renderTile(TILE_GRASS, x, y);
		render16(x, y, 32, 0, 0);
		break;
	case TILE_SAPLING_CACTUS:
		renderTile(TILE_SAND, x, y);
		render16(x, y, 32, 0, 0);
		break;
	case TILE_FARM:
		render16(x, y, 144, 0, 0);
		break;
	case TILE_WHEAT:
		age = getData(x >> 4, y >> 4) / 20;
		if (age > 5)
			age = 5;
		render16(x, y, 160 + (age << 4), 0, 0);
		break;
	}

}

void renderZoomedMap() {
    int mx = mScrollX;
    int my = mScrollY;
    if(zoomLevel == 2) mx = 32;
	sf2d_draw_texture_scale(minimap[currentLevel], mx, my, zoomLevel, zoomLevel); // zoomed map
	if(currentLevel == 0){
        render16c(
        (mx+((awX/16)*zoomLevel)-16)/2, 
        (my+((awY/16)*zoomLevel)-16)/2, 
        160, 112, 
        ((player.p.walkDist >> 6) & 1) == 0 ? 0 : 1, 
        2, 2
        ); // Airwizard on zoomed map
    }
    render16c(
    (mx+((player.x/16)*zoomLevel)-16)/2, 
    (my+((player.y/16)*zoomLevel)-16)/2, 
    0, 112, 
    ((player.p.walkDist >> 6) & 1) == 0 ? 0 : 1, 
    2, 2
    ); // Player on zoomed map
    drawText(mapText,224, 214); // "x2"/"x4"/"x6"
    render16(142, 2, 72, 208, 0); // Exit button
    renderc(126, 102, 40, 208, 32, 16, 0); // Plus/Minus zoom buttons
    if(zoomLevel < 3) sf2d_draw_rectangle(258, 210, 26, 20, 0x4F4F4F7F); // gray out minus button
    else if(zoomLevel > 5) sf2d_draw_rectangle(284, 210, 26, 20, 0x4F4F4F7F); // gray out minus button
}

char scoreT[32];
void renderGui() {
	int i;
	for (i = 0; i < 10; ++i) {
		if (i < player.p.health)
			render(i * 8 + 6, 5, 168, 152, 0);
		else
			render(i * 8 + 6, 5, 176, 152, 0);
		if (i < player.p.stamina)
			render(i * 8 + 6, 14, 184, 152, 0);
		else
			render(i * 8 + 6, 14, 191, 152, 0);
	}
	sf2d_draw_texture(minimap[currentLevel], 96, 102);
	renderItemWithTextCentered(player.p.activeItem, 320, 66);
	itoa(player.p.score, scoreT, 10); // integer to base10 string
	drawText("Score:",214,12);
	drawText(scoreT,(140-(strlen(scoreT)*12))/2 + 180,29);
	if(currentLevel == 0){
        renderc(44 + (awX/32), 47 + (awY/32), 88, 216, 8, 8, 0); // Mini-AWizard head.
    }
	renderc(44 + (player.x/32), 47 + (player.y/32), 88, 208, 8, 8, 0); // Mini-Player head.
}

void renderPlayer() {
	if (player.p.isDead)
		return;
	int xo = player.x - 8;
	int yo = player.y - 8;

	if (player.p.attackTimer > 0 && player.p.dir == 1) {
		renderc(xo, yo - 4, 16, 160, 16, 8, 0);
		renderItemIcon(player.p.activeItem->id, player.p.activeItem->countLevel,
				xo + 4, yo - 4);
	}
	u8 walk = (player.p.walkDist >> 4) & 1;
	bool swimming = isSwimming();
	switch (player.p.dir) {
	case 0: // down
		if (swimming)
			renderc(xo, yo + 4, 48,
					160 + (((player.p.swimTimer >> 4) & 1) << 3), 16, 8, 0);
		else
			renderc(xo, yo + 8, 0, 120 + (player.p.isCarrying ? 16 : 0), 16, 8,
					walk == 0 ? 0 : 1);
		renderc(xo, yo, 0, 112 + (player.p.isCarrying ? 16 : 0), 16, 8,
				walk == 0 ? 0 : 1);
		break;
	case 1: // up
		if (swimming)
			renderc(xo, yo + 4, 48,
					160 + (((player.p.swimTimer >> 4) & 1) << 3), 16, 8, 0);
		else
			renderc(xo, yo + 8, 16, 120 + (player.p.isCarrying ? 16 : 0), 16, 8,
					walk == 0 ? 0 : 1);
		renderc(xo, yo, 16, 112 + (player.p.isCarrying ? 16 : 0), 16, 8,
				walk == 0 ? 0 : 1);
		break;
	case 2: // left
		if (swimming)
			renderc(xo, yo + 4, 48,
					160 + (((player.p.swimTimer >> 4) & 1) << 3), 16, 8, 0);
		else
			renderc(xo, yo + 8, 32 + (walk << 4),
					120 + (player.p.isCarrying ? 16 : 0), 16, 8, 1);
		renderc(xo, yo, 32 + (walk << 4), 112 + (player.p.isCarrying ? 16 : 0),
				16, 8, 1);
		break;
	case 3: // right
		if (swimming)
			renderc(xo, yo + 4, 48,
					160 + (((player.p.swimTimer >> 4) & 1) << 3), 16, 8, 0);
		else
			renderc(xo, yo + 8, 32 + (walk << 4),
					120 + (player.p.isCarrying ? 16 : 0), 16, 8, 0);
		renderc(xo, yo, 32 + (walk << 4), 112 + (player.p.isCarrying ? 16 : 0),
				16, 8, 0);
		break;
	}

	if (player.p.isCarrying) {
		renderFurniture(player.p.activeItem->id, xo, yo - 12);
	}

	if (player.p.attackTimer > 0) {
		switch (player.p.dir) {
		case 0:
			renderc(xo - player.p.ax, yo - player.p.ay + 12, 16, 168, 16, 8, 0);
			renderItemIcon(player.p.activeItem->id,
					player.p.activeItem->countLevel, xo + 4, yo + 12);
			break;
		case 2:
			renderc(xo - player.p.ax - 4, yo - player.p.ay, 32, 160, 8, 16, 0);
			renderItemIcon(player.p.activeItem->id,
					player.p.activeItem->countLevel, xo - 4, yo + 4);
			break;
		case 3:
			renderc(xo - player.p.ax + 12, yo - player.p.ay, 40, 160, 8, 16, 0);
			renderItemIcon(player.p.activeItem->id,
					player.p.activeItem->countLevel, xo + 12, yo + 4);
			break;
		}
	}
}

void renderMenuBackground(int xScroll, int yScroll) {
	sf2d_draw_rectangle(0, 0, 400, 240, RGBA8(12, 12, 12, 255)); //You might think "real" black would be better, but it actually looks better that way
	renderLightsToStencil();
	renderBackground(xScroll, yScroll);
	resetStencilStuff();
}

void renderBackground(int xScroll, int yScroll) {
    if(currentLevel > 0) sf2d_draw_rectangle(0, 0, 400, 240, dirtColor[currentLevel]); // dirt color
	else {
		sf2d_draw_texture_part_scale(minimap[1], (-xScroll / 3) - 256, (-yScroll / 3) - 32, 0, 0, 128, 128, 12.5, 7.5);
		sf2d_draw_rectangle(0, 0, 400, 240, 0xDFDFDFAF);
	}
	int xo = xScroll >> 4;
	int yo = yScroll >> 4;
	int x, y;
	for (x = xo; x <= 13 + xo; ++x) {
		for (y = yo; y <= 8 + yo; ++y)
			renderTile(getTile(x, y), x << 4, y << 4);
	}
}

char * fontChars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ      0123456789.,!?'\"-+=/\\%()<>:;     ";

void drawText(char * msg, u32 x, u32 y) {
	int i = 0;
	x -= offsetX << 1;
	y -= offsetY << 1;
	for (i = 0; i < strlen(msg); ++i) {
		int ix = strchr(fontChars, toupper((unsigned char) msg[i])) - fontChars;
		int iy = ix >> 5;
		if (ix >= 0) {
			sf2d_draw_texture_part(font, x + i * 12, y, (ix & 31) * 12,
					16 + (iy * 12), 12, 12);
		}
	}
}

void drawSizedText(char * msg, u32 x, u32 y, float size) {
	int i = 0;
	for (i = 0; i < strlen(msg); ++i) {
		int ix = strchr(fontChars, toupper((unsigned char) msg[i])) - fontChars;
		int iy = ix >> 5;
		if (ix >= 0) {
			sf2d_draw_texture_part_scale(font, (x + i * 8) * size, y,
					(ix & 31) << 3, iy << 3, 8, 8, size, size);
		}
	}
}

void drawTextColor(char * msg, u32 x, u32 y, u32 color) {
	int i = 0;
	x -= offsetX << 1;
	y -= offsetY << 1;
	for (i = 0; i < strlen(msg); ++i) {
		int ix = strchr(fontChars, toupper((unsigned char) msg[i])) - fontChars;
		int iy = ix >> 5;
		if (ix >= 0) {
			sf2d_draw_texture_part_blend(font, x + i * 12, y, (ix & 31) * 12,
					16 + (iy * 12), 12, 12, color);
		}
	}
}

// Changes text color after the first space.
void drawTextColorSpecial(char * msg, u32 x, u32 y, u32 color, u32 color2) {
	int i = 0;
	x -= offsetX << 1;
	y -= offsetY << 1;
	bool sOver = false;
	for (i = 0; i < strlen(msg); ++i) {
		int ix = strchr(fontChars, toupper((unsigned char) msg[i])) - fontChars;
		if (msg[i] == 32)
			sOver = true;
		int iy = ix >> 5;
		if (ix >= 0) {
			if (sOver)
				sf2d_draw_texture_part_blend(font, x + i * 12, y,
						(ix & 31) * 12, 16 + (iy * 12), 12, 12, color2);
			else
				sf2d_draw_texture_part_blend(font, x + i * 12, y,
						(ix & 31) * 12, 16 + (iy * 12), 12, 12, color);
		}
	}
}

void drawSizedTextColor(char * msg, int x, int y, float size, u32 color) {
	int i;
	for (i = 0; i < strlen(msg); ++i) {
		int ix = strchr(fontChars, toupper((unsigned char) msg[i])) - fontChars;
		int iy = ix >> 5;
		if (ix >= 0)
			sf2d_draw_texture_part_scale_blend(font, (x + i * 8) * size,
					y * size, (ix & 31) << 3, iy << 3, 8, 8, size, size, color);
	}
}

void renderFurniture(int itemID, int x, int y) {
	switch (itemID) {
	case ITEM_ANVIL:
		render16(x, y, 64, 128, 0);
		break;
	case ITEM_CHEST:
		render16(x, y, 80, 128, 0);
		break;
	case ITEM_OVEN:
		render16(x, y, 96, 128, 0);
		break;
	case ITEM_FURNACE:
		render16(x, y, 112, 128, 0);
		break;
	case ITEM_WORKBENCH:
		render16(x, y, 128, 128, 0);
		break;
	case ITEM_LANTERN:
		render16(x, y, 144, 128, 0);
		break;
	}
}

char ertxt[20];
void renderEntity(Entity* e, int x, int y) {
	switch (e->type) {
	case ENTITY_ITEM:
		if (e->entityItem.age >= 520)
			if (e->entityItem.age / 6 % 2 == 0)
				return;
		renderItemIcon2(e->entityItem.item.id, e->entityItem.item.countLevel,
				x - 4, y - 4, (int) e->entityItem.zz);
		break;
	case ENTITY_FURNITURE:
		renderFurniture(e->entityFurniture.itemID, x - 8, y - 8);
		break;
	case ENTITY_ZOMBIE:
		switch (e->zombie.dir) {
		case 0: // down
			render16b(x - 8, y - 8, 64, 112,
					((e->zombie.walkDist >> 4) & 1) == 0 ? 0 : 1,
					e->zombie.color);
			break;
		case 1: // up
			render16b(x - 8, y - 8, 80, 112,
					((e->zombie.walkDist >> 4) & 1) == 0 ? 0 : 1,
					e->zombie.color);
			break;
		case 2: // left
			render16b(x - 8, y - 8, 96 + (((e->zombie.walkDist >> 4) & 1) << 4),
					112, 1, e->zombie.color);
			break;
		case 3: // right
			render16b(x - 8, y - 8, 96 + (((e->zombie.walkDist >> 4) & 1) << 4),
					112, 0, e->zombie.color);
			break;
		}
		break;
	case ENTITY_SLIME:
		render16b(x - 8, y - 8 - (e->slime.jumpTime > 0 ? 4 : 0),
				128 + (e->slime.jumpTime > 0 ? 16 : 0), 112, 0, e->slime.color);
		break;
	case ENTITY_AIRWIZARD:
		e->wizard.spriteAdjust = 0;
		if (e->wizard.health < 200) {
			if (tickCount / 4 % 3 < 2)
				e->wizard.spriteAdjust = 16;
		} else if (e->wizard.health < 1000) {
			if (tickCount / 5 % 4 < 2)
				e->wizard.spriteAdjust = 16;
		}
		switch (e->wizard.dir) {
		case 0: // down
			render16(x - 8, y - 8, 160, 112 + e->wizard.spriteAdjust,
					((e->wizard.walkDist >> 4) & 1) == 0 ? 0 : 1);
			break;
		case 1: // up
			render16(x - 8, y - 8, 176, 112 + e->wizard.spriteAdjust,
					((e->wizard.walkDist >> 4) & 1) == 0 ? 0 : 1);
			break;
		case 2: // left
			render16(x - 8, y - 8, 192 + (((e->wizard.walkDist >> 4) & 1) << 4),
					112 + e->wizard.spriteAdjust, 1);
			break;
		case 3: // right
			render16(x - 8, y - 8, 192 + (((e->wizard.walkDist >> 4) & 1) << 4),
					112 + e->wizard.spriteAdjust, 0);
			break;
		}
		break;
	case ENTITY_TEXTPARTICLE:
		x -= offsetX;
		y -= offsetY;
		drawSizedTextColor(e->textParticle.text, x + 1,
				y - (int) e->textParticle.zz + 1, 2, 0xFF);
		drawSizedTextColor(e->textParticle.text, x,
				y - (int) e->textParticle.zz, 2, e->textParticle.color);
		break;
	case ENTITY_SMASHPARTICLE:
		render16(x, y, 0, 160, 0);
		break;
	case ENTITY_SPARK:
		if (e->spark.age >= 200)
			if (e->spark.age / 6 % 2 == 0)
				return;
		renderr(x, y, 200, 152, 0, e->spark.age * 0.0349);
		break;
	}
}

void renderEntities(int x, int y, EntityManager* em) {
	int i;
	for (i = 0; i < em->lastSlot[currentLevel]; ++i) {
		Entity e = em->entities[currentLevel][i];
		if (e.x > x - 200 && e.y > y - 125 && e.x < x + 200 && e.y < y + 125)
			renderEntity(&e, e.x, e.y);
	}
}

void renderItemList(Inventory * inv, int xo, int yo, int x1, int y1,
		int selected) {
	// If lastSlot is 0, then there are no items are in the inventory.
	bool drawCursor = true;
	if (selected < 0) {
		drawCursor = false;
		selected = 0;
	}
	int w = x1 - xo;
	int h = y1 - yo - 2;
	int i1 = inv->lastSlot;
	if (i1 > h)
		i1 = h;
	int io = selected - h / 2;
	if (io > inv->lastSlot - h)
		io = inv->lastSlot - h;
	if (io < 0)
		io = 0;

	int i;
	for (i = 0; i < i1; ++i)
		renderItemWithText(&inv->items[i + io], (1 + xo) << 4,
				(i + 1 + yo) << 4);

	if (drawCursor) {
		int yy = selected + 1 - io + yo;
		sf2d_draw_rectangle((xo << 4) - (offsetX << 1),
				(yy << 4) - (offsetY << 1), 12, 12, 0xFF);
		drawText(">", (xo << 4), yy << 4);
		sf2d_draw_rectangle(((xo + w) << 4) - 12 - (offsetX << 1),
				(yy << 4) - (offsetY << 1), 12, 12, 0xFF);
		drawText("<", ((xo + w) << 4) - 12, yy << 4);
	}
}

void renderRecipes(RecipeManager * r, int xo, int yo, int x1, int y1,
		int selected) {
	int size = r->size;
	if (size < 1)
		return;
	if (selected < 0)
		selected = 0;
	int w = x1 - xo;
	int h = y1 - yo - 2;
	int i1 = size;
	if (i1 > h)
		i1 = h;
	int io = selected - h / 2;
	if (io > size - h)
		io = size - h;
	if (io < 0)
		io = 0;

	int i, col;
	for (i = 0; i < i1; ++i) {
		int x = (1 + xo) << 4, y = (i + 1 + yo) << 4;
		renderItemIcon(r->recipes[i + io].itemResult,
				r->recipes[i + io].itemAmountLevel, x >> 1, y >> 1);
		if (r->recipes[i + io].canCraft)
			col = 0xFFFFFFFF;
		else
			col = 0x7F7F7FFF;
		drawTextColor(
				getBasicItemName(r->recipes[i + io].itemResult,
						r->recipes[i + io].itemAmountLevel), x + 18, y + 2,
				col);
	}

	int yy = selected + 1 - io + yo;
	sf2d_draw_rectangle(xo << 4, yy << 4, 12, 12, 0xFF);
	drawText(">", xo << 4, yy << 4);
	sf2d_draw_rectangle(((xo + w) << 4) - 12, yy << 4, 12, 12, 0xFF);
	drawText("<", ((xo + w) << 4) - 12, yy << 4);
}

void renderItemWithText(Item* item, int x, int y) {
	renderItemIcon(item->id, item->countLevel, x >> 1, y >> 1);
	if (item->onlyOne)
		drawText(getItemName(item->id, item->countLevel), x + 18, y + 2);
	else
		drawTextColorSpecial(getItemName(item->id, item->countLevel), x + 18,
				y + 2, 0xD2D2D2FF, 0xFFFFFFFF);
}

void renderItemStuffWithText(int itemID, int itemCL, bool onlyOne, int x, int y) {
	renderItemIcon(itemID, itemCL, x >> 1, y >> 1);
	if (onlyOne)
		drawText(getItemName(itemID, itemCL), x + 18, y + 2);
	else
		drawTextColorSpecial(getItemName(itemID, itemCL), x + 18,
				y + 2, 0xD2D2D2FF, 0xFFFFFFFF);
}

/* For bottom screen */ 
void renderItemWithTextCentered(Item* item, int width, int y) {
	char * tn = getItemName(item->id, item->countLevel);
    int x = (width - ((strlen(tn) + 2) * 12))/2;
    
	renderItemIcon(item->id, item->countLevel, x >> 1, y >> 1);
	
	if (item->onlyOne)
		drawText(getItemName(item->id, item->countLevel), x + 18, y + 2);
	else
		drawTextColorSpecial(getItemName(item->id, item->countLevel), x + 18,
				y + 2, 0xD2D2D2FF, 0xFFFFFFFF);
}

void renderItemIcon2(int itemID, int countLevel, int x, int y, int z) {
	switch (itemID) {
	case ITEM_NULL:
		return;
	case TOOL_SHOVEL:
		renderb(x, y, countLevel * 8, 144, 0, 0xFF);
		break;
	case TOOL_HOE:
		renderb(x, y, 40 + countLevel * 8, 144, 0, 0xFF);
		break;
	case TOOL_SWORD:
		renderb(x, y, 80 + countLevel * 8, 144, 0, 0xFF);
		break;
	case TOOL_PICKAXE:
		renderb(x, y, 120 + countLevel * 8, 144, 0, 0xFF);
		break;
	case TOOL_AXE:
		renderb(x, y, 160 + countLevel * 8, 144, 0, 0xFF);
		break;
	case ITEM_ANVIL:
		renderb(x, y, 120, 152, 0, 0xFF);
		break;
	case ITEM_CHEST:
		renderb(x, y, 128, 152, 0, 0xFF);
		break;
	case ITEM_OVEN:
		renderb(x, y, 136, 152, 0, 0xFF);
		break;
	case ITEM_FURNACE:
		renderb(x, y, 144, 152, 0, 0xFF);
		break;
	case ITEM_WORKBENCH:
		renderb(x, y, 152, 152, 0, 0xFF);
		break;
	case ITEM_LANTERN:
		renderb(x, y, 160, 152, 0, 0xFF);
		break;
	case ITEM_POWGLOVE:
		renderb(x, y, 56, 152, 0, 0xFF);
		break;
	case ITEM_FLOWER:
		renderb(x, y, 0, 152, 0, 0xFF);
		break;
	case ITEM_WOOD:
		renderb(x, y, 8, 152, 0, 0xFF);
		break;
	case ITEM_STONE:
		renderb(x, y, 16, 152, 0, 0xFF);
		break;
	case ITEM_SAND:
		renderb(x, y, 16, 152, 0, 0xFF);
		break;
	case ITEM_DIRT:
		renderb(x, y, 16, 152, 0, 0xFF);
		break;
	case ITEM_CLOUD:
		renderb(x, y, 16, 152, 0, 0xFF);
		break;
	case ITEM_ACORN:
		renderb(x, y, 24, 152, 0, 0xFF);
		break;
	case ITEM_CACTUS:
		renderb(x, y, 32, 152, 0, 0xFF);
		break;
	case ITEM_SEEDS:
		renderb(x, y, 40, 152, 0, 0xFF);
		break;
	case ITEM_WHEAT:
		renderb(x, y, 48, 152, 0, 0xFF);
		break;
	case ITEM_FLESH:
		renderb(x, y, 64, 152, 0, 0xFF);
		break;
	case ITEM_BREAD:
		renderb(x, y, 72, 152, 0, 0xFF);
		break;
	case ITEM_APPLE:
		renderb(x, y, 80, 152, 0, 0xFF);
		break;
	case ITEM_SLIME:
		renderb(x, y, 88, 152, 0, 0xFF);
		break;
	case ITEM_COAL:
		renderb(x, y, 88, 152, 0, 0xFF);
		break;
	case ITEM_IRONORE:
		renderb(x, y, 88, 152, 0, 0xFF);
		break;
	case ITEM_GOLDORE:
		renderb(x, y, 88, 152, 0, 0xFF);
		break;
	case ITEM_IRONINGOT:
		renderb(x, y, 96, 152, 0, 0xFF);
		break;
	case ITEM_GOLDINGOT:
		renderb(x, y, 96, 152, 0, 0xFF);
		break;
	case ITEM_GLASS:
		renderb(x, y, 104, 152, 0, 0xFF);
		break;
	case ITEM_GEM:
		renderb(x, y, 112, 152, 0, 0xFF);
		break;
	}
	y -= z;
	renderItemIcon(itemID, countLevel, x, y);
}

void renderItemIcon(int itemID, int countLevel, int x, int y) {
	switch (itemID) {
	case ITEM_NULL:
		return;
	case TOOL_SHOVEL:
		render(x, y, countLevel * 8, 144, 0);
		break;
	case TOOL_HOE:
		render(x, y, 40 + countLevel * 8, 144, 0);
		break;
	case TOOL_SWORD:
		render(x, y, 80 + countLevel * 8, 144, 0);
		break;
	case TOOL_PICKAXE:
		render(x, y, 120 + countLevel * 8, 144, 0);
		break;
	case TOOL_AXE:
		render(x, y, 160 + countLevel * 8, 144, 0);
		break;
	case ITEM_ANVIL:
		render(x, y, 120, 152, 0);
		break;
	case ITEM_CHEST:
		render(x, y, 128, 152, 0);
		break;
	case ITEM_OVEN:
		render(x, y, 136, 152, 0);
		break;
	case ITEM_FURNACE:
		render(x, y, 144, 152, 0);
		break;
	case ITEM_WORKBENCH:
		render(x, y, 152, 152, 0);
		break;
	case ITEM_LANTERN:
		render(x, y, 160, 152, 0);
		break;
	case ITEM_POWGLOVE:
		render(x, y, 56, 152, 0);
		break;
	case ITEM_FLOWER:
		render(x, y, 0, 152, 0);
		break;
	case ITEM_WOOD:
		render(x, y, 8, 152, 0);
		break;
	case ITEM_STONE:
		renderb(x, y, 16, 152, 0, 0xCFCFCFFF);
		break;
	case ITEM_SAND:
		renderb(x, y, 16, 152, 0, 0xF7F77BFF);
		break;
	case ITEM_DIRT:
		renderb(x, y, 16, 152, 0, 0xAF9781FF);
		break;
	case ITEM_CLOUD:
		renderb(x, y, 16, 152, 0, 0xFFFFFFFF);
		break;
	case ITEM_ACORN:
		render(x, y, 24, 152, 0);
		break;
	case ITEM_CACTUS:
		render(x, y, 32, 152, 0);
		break;
	case ITEM_SEEDS:
		render(x, y, 40, 152, 0);
		break;
	case ITEM_WHEAT:
		render(x, y, 48, 152, 0);
		break;
	case ITEM_FLESH:
		render(x, y, 64, 152, 0);
		break;
	case ITEM_BREAD:
		render(x, y, 72, 152, 0);
		break;
	case ITEM_APPLE:
		render(x, y, 80, 152, 0);
		break;
	case ITEM_SLIME:
		renderb(x, y, 88, 152, 0, 0x4DC04DFF);
		break;
	case ITEM_COAL:
		renderb(x, y, 88, 152, 0, 0x383838FF);
		break;
	case ITEM_IRONORE:
		renderb(x, y, 88, 152, 0, 0xBC9999FF);
		break;
	case ITEM_GOLDORE:
		renderb(x, y, 88, 152, 0, 0xCECE77FF);
		break;
	case ITEM_IRONINGOT:
		renderb(x, y, 96, 152, 0, 0xDFC8C8FF);
		break;
	case ITEM_GOLDINGOT:
		renderb(x, y, 96, 152, 0, 0xEAEABCFF);
		break;
	case ITEM_GLASS:
		render(x, y, 104, 152, 0);
		break;
	case ITEM_GEM:
		render(x, y, 112, 152, 0);
		break;
	}
}

void defineTables() {

	int i = 0;
	for (i = 256; i > 0; --i) {
		// Creates the lookup table for the tree tile.
		if ((i & 255) == 255)
			treeTable[i] = 208;
		else if ((i & 239) == 239)
			treeTable[i] = 144;
		else if ((i & 191) == 191)
			treeTable[i] = 160;
		else if ((i & 127) == 127)
			treeTable[i] = 176;
		else if ((i & 223) == 223)
			treeTable[i] = 192;
		else if ((i & 206) == 206)
			treeTable[i] = 112;
		else if ((i & 55) == 55)
			treeTable[i] = 128;
		else if ((i & 173) == 173)
			treeTable[i] = 96;
		else if ((i & 91) == 91)
			treeTable[i] = 80;

		else if ((i & 159) == 159)
			treeTable[i] = 224;
		else if ((i & 111) == 111)
			treeTable[i] = 240;

		else if ((i & 19) == 19)
			treeTable[i] = 64;
		else if ((i & 37) == 37)
			treeTable[i] = 48;
		else if ((i & 74) == 74)
			treeTable[i] = 32;
		else if ((i & 140) == 140)
			treeTable[i] = 16;
	}

	/*
	 boolean up = i & 1
	 boolean left = i & 2
	 boolean right = i & 4
	 boolean down = i & 8
	 boolean up-left = i & 16
	 boolean up-right = i & 32
	 boolean down-left = i & 64
	 boolean down-right = i & 128
	 */

	for (i = 256; i > 0; --i) {
		// Creates the lookup table for the rock tile.
		if ((i & 255) == 255)
			rockTable[i] = 208;
		else if ((i & 239) == 239)
			rockTable[i] = 144;
		else if ((i & 191) == 191)
			rockTable[i] = 160;
		else if ((i & 127) == 127)
			rockTable[i] = 176;
		else if ((i & 223) == 223)
			rockTable[i] = 192;
		else if ((i & 207) == 207)
			rockTable[i] = 256 * 16 + 32;
		else if ((i & 63) == 63)
			rockTable[i] = 256 * 16 + 16;
		else if ((i & 206) == 206)
			rockTable[i] = 112;
		else if ((i & 95) == 95)
			rockTable[i] = 256 * 32 + 144;
		else if ((i & 159) == 159)
			rockTable[i] = 256 * 32 + 160;
		else if ((i & 31) == 31)
			rockTable[i] = 256 * 32 + 208;
		else if ((i & 55) == 55)
			rockTable[i] = 128;
		else if ((i & 175) == 175)
			rockTable[i] = 256 * 32 + 128;
		else if ((i & 143) == 143)
			rockTable[i] = 256 * 32 + 224;
		else if ((i & 173) == 173)
			rockTable[i] = 96;
		else if ((i & 111) == 111)
			rockTable[i] = 256 * 32 + 112;
		else if ((i & 47) == 47)
			rockTable[i] = 256 * 32 + 48;
		else if ((i & 45) == 45)
			rockTable[i] = 256 * 32 + 176;
		else if ((i & 79) == 79)
			rockTable[i] = 256 * 32 + 192;
		else if ((i & 23) == 23)
			rockTable[i] = 256 * 32 + 96;
		else if ((i & 91) == 91)
			rockTable[i] = 80;
		else if ((i & 27) == 27)
			rockTable[i] = 256 * 16 + 96;
		else if ((i & 19) == 19)
			rockTable[i] = 64;
		else if ((i & 75) == 75)
			rockTable[i] = 256 * 32;
		else if ((i & 141) == 141)
			rockTable[i] = 256 * 32 + 16;
		else if ((i & 142) == 142)
			rockTable[i] = 256 * 32 + 32;
		else if ((i & 78) == 78)
			rockTable[i] = 256 * 32 + 64;
		else if ((i & 39) == 39)
			rockTable[i] = 256 * 32 + 80;
		else if ((i & 37) == 37)
			rockTable[i] = 48;
		else if ((i & 74) == 74)
			rockTable[i] = 32;
		else if ((i & 140) == 140)
			rockTable[i] = 16;
		else if ((i & 15) == 15)
			rockTable[i] = 256 * 16 + 112;
		else if ((i & 11) == 11)
			rockTable[i] = 256 * 16 + 192;
		else if ((i & 13) == 13)
			rockTable[i] = 256 * 16 + 208;
		else if ((i & 14) == 14)
			rockTable[i] = 256 * 16 + 224;
		else if ((i & 7) == 7)
			rockTable[i] = 256 * 16 + 240;
		else if ((i & 12) == 12)
			rockTable[i] = 256 * 16 + 128;
		else if ((i & 10) == 10)
			rockTable[i] = 256 * 16 + 144;
		else if ((i & 5) == 5)
			rockTable[i] = 256 * 16 + 160;
		else if ((i & 3) == 3)
			rockTable[i] = 256 * 16 + 176;
		else if ((i & 9) == 9)
			rockTable[i] = 256 * 16;
		else if ((i & 6) == 6)
			rockTable[i] = 256 * 16 + 48;
		else if ((i & 8) == 8)
			rockTable[i] = 224;
		else if ((i & 4) == 4)
			rockTable[i] = 256 * 16 + 64;
		else if ((i & 2) == 2)
			rockTable[i] = 256 * 16 + 80;
		else if ((i & 1) == 1)
			rockTable[i] = 240;

	}

	// Lookup table for the grass/sand tile.
	grassTable[1] = 192;
	grassTable[2] = 160;
	grassTable[3] = 64;
	grassTable[4] = 144;
	grassTable[5] = 48;
	grassTable[6] = 224;
	grassTable[7] = 128;
	grassTable[8] = 176;
	grassTable[9] = 240;
	grassTable[10] = 32;
	grassTable[11] = 80;
	grassTable[12] = 16;
	grassTable[13] = 96;
	grassTable[14] = 112;
	grassTable[15] = 208;

}
