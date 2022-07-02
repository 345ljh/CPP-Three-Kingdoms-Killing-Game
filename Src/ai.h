#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "items.h"
#include "app.h"

int ThrowAi(player_t *player, int camp, int area, int add = 0);
int ThrowAiSelf(player_t *recipient, int area, int suit = 15, int type = 7, int add = 0);
int PlayAi(player_t* executor);
int StateCompareAi(int state[12], int id);
int AnswerAi(player_t *recipient, type_e type, int add = 0);
int WuguAi(player_t *recipient, int* buf, int len);
int EquipAi(player_t* executor, type_e type);
#endif // AI_H_INCLUDED
