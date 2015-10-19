#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <3ds.h>


typedef struct {
    u32 size;
    u8 * buffer;
} Sound;

void loadSound(Sound * snd, char * filename);
void playSound(Sound snd);
void freeSounds();

Sound snd_playerHurt;
Sound snd_playerDeath;
Sound snd_monsterHurt;
Sound snd_test;
Sound snd_pickup;
Sound snd_bossdeath;
Sound snd_craft;
