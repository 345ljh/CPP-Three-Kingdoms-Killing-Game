#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include "items.h"

extern card_t card_inf[160];
extern game_t game;

void PileInit(void);
void Shuffle(void);
void Takecard(player_t player, int amount);
void Throwcard(player_t executor, player_t player, int amount);
void Showcard(player_t executor, player_t player, int amount);
void Getcard(player_t executor, player_t player, int amount, int type);
void Damage(player_t executor, player_t player, int amount, damage_e type);
void Neardeath(player_t player);
void Death(player_t player);
void VictoryJudge(void);
#endif // ACTIONS_H_INCLUDED
