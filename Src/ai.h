#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "items.h"
#include "app.h"

int ThrowAi(player_t *player, int* state, int area, int suit = 15, int type = 7);
int GetAi(player_t *executor, player_t* player, int* state, int area);
int StateCompareAi(int state[13], int id);
int AnswerAi(player_t *recepient, type_e type, int add = 0);
int WuguAi(player_t *recipient, int* buf, int len);
#endif // AI_H_INCLUDED
