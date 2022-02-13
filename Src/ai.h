#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "items.h"
#include "app.h"

void ThrowAi(player_t *player, int* state, int area, int suit = 15, int type = 7);
int StateCompareAi(int state[13], int id);

#endif // AI_H_INCLUDED
