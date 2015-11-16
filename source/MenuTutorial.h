#pragma once

#include <3ds.h>
#include <string.h> 
#include <sf2d.h>
#include <dirent.h>
#include <ctype.h>

#include "Render.h"

extern u8 pageNum;
extern u8 maxPageNum;

char pageText[18];

void renderTutorialPage(bool topScreen);
