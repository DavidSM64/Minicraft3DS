#include "Sound.h"

void loadSound(Sound * snd, char * filename){
    FILE *file = fopen(filename, "rb");
    if(file != NULL){
        fseek(file, 0, SEEK_END);
        snd->size = ftell(file);
        fseek(file, 0, SEEK_SET);
        snd->buffer = linearAlloc(snd->size);
        fread(snd->buffer, 1, snd->size, file);
    }
    fclose(file);
}

void playSound(Sound snd){
    csndPlaySound(8, SOUND_FORMAT_16BIT | SOUND_ONE_SHOT, 44100, 1, 0, snd.buffer, snd.buffer, snd.size);
}

void freeSounds(){
    linearFree(snd_playerHurt.buffer);
    linearFree(snd_playerDeath.buffer);
    linearFree(snd_monsterHurt.buffer);
    linearFree(snd_test.buffer);
    linearFree(snd_pickup.buffer);
    linearFree(snd_bossdeath.buffer);
    linearFree(snd_craft.buffer);
}
