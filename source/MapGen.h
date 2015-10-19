#pragma once
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <3ds.h>

#include "Globals.h"

float nextFloat();
double sample(double * values, int x, int y);
double * Noise(int w, int h, int featureSize);
void newSeed();
void createAndValidateTopMap(int w, int h, u8 * map, u8 * data);
void createTopMap(int w, int h, u8 * map, u8 * data);
void createAndValidateUndergroundMap(int w, int h,int depthLevel, u8 * map, u8 * data);
void createUndergroundMap(int w, int h,int depthLevel, u8 * map, u8 * data);
void createAndValidateSkyMap(int w, int h, u8 * map, u8 * data);
void createSkyMap(int w, int h, u8 * map, u8 * data);
