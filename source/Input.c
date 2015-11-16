#include "Input.h"

void toggleKey(Key* key, bool held, bool down){
    key->down = held;
	key->clicked = down;
}
    
void tickKeys(u32 held, u32 down){
    hidTouchRead(&k_touch); // Update touch position
    toggleKey(&k_up, held & k_up.input, down & k_up.input);
    toggleKey(&k_down, held & k_down.input, down & k_down.input);
    toggleKey(&k_left, held & k_left.input, down & k_left.input);
    toggleKey(&k_right, held & k_right.input, down & k_right.input);
    toggleKey(&k_pause, held & k_pause.input, down & k_pause.input);
    toggleKey(&k_attack, held & k_attack.input, down & k_attack.input);
    toggleKey(&k_menu, held & k_menu.input, down & k_menu.input);
    toggleKey(&k_accept, held & k_accept.input, down & k_accept.input);
    toggleKey(&k_decline, held & k_decline.input, down & k_decline.input);
    toggleKey(&k_delete, held & k_delete.input, down & k_delete.input);
    toggleKey(&k_menuNext, held & k_menuNext.input, down & k_menuNext.input);
    toggleKey(&k_menuPrev, held & k_menuPrev.input, down & k_menuPrev.input);
}

