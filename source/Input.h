#include <3ds.h>

typedef struct {
	bool down, clicked;
	int input;
} Key;

Key k_null;
Key k_up;
Key k_down;
Key k_left;
Key k_right;
Key k_attack;
Key k_menu;
Key k_pause;
Key k_accept;
Key k_decline;
Key k_delete;
Key k_menuNext;
Key k_menuPrev;
touchPosition k_touch;

void tickKeys(u32 held, u32 down);
bool clicked(Key key);
